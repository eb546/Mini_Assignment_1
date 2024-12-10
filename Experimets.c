#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define MAX_RESOURCES 5
#define NUM_CLIENTS 10
#define BUFFER_SIZE 256
#define MAX_PRIORITY 10

// Resource Manager shared data structure
typedef struct {
    int *resourcePool;  // Resource pool (dynamically allocated)
    int resourceCount;  // Number of resources available
    sem_t *mutex;       // Semaphore for mutual exclusion
    sem_t *resourceSem; // Semaphore to access resources
    int *clientPipes[NUM_CLIENTS];  // Pipes for communication with clients
} ResourceManager;

// Structure for client request
typedef struct {
    int clientId;
    int priority; // Higher priority clients have lower numbers (1 is highest)
} ClientRequest;

// Compare function for sorting clients by priority
int comparePriority(const void *a, const void *b) {
    return ((ClientRequest*)a)->priority - ((ClientRequest*)b)->priority;
}

// Function to initialize resources and semaphores
void initializeManager(ResourceManager *manager) {
    // Dynamically allocate resource pool
    manager->resourcePool = (int *)malloc(sizeof(int) * MAX_RESOURCES);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        manager->resourcePool[i] = 0;  // 0 means available
    }

    // Initialize semaphores
    manager->mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    manager->resourceSem = sem_open("/resourceSem", O_CREAT, 0666, MAX_RESOURCES);

    if (manager->mutex == SEM_FAILED || manager->resourceSem == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(1);
    }

    manager->resourceCount = MAX_RESOURCES;
}

// Function to clean up resources and semaphores
void cleanupManager(ResourceManager *manager) {
    free(manager->resourcePool);
    sem_close(manager->mutex);
    sem_close(manager->resourceSem);
    sem_unlink("/mutex");
    sem_unlink("/resourceSem");
}

// Function to simulate resource allocation for a client
void allocateResource(ResourceManager *manager, int clientId, int priority, int pipeFd) {
    sem_wait(manager->resourceSem);  // Wait for available resource
    sem_wait(manager->mutex);        // Lock mutex for resource allocation

    // Simulate resource allocation (e.g., assign a resource to the client)
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (manager->resourcePool[i] == 0) {  // Resource is available
            manager->resourcePool[i] = clientId;  // Allocate resource to client
            printf("Client %d (Priority %d) allocated resource %d\n", clientId, priority, i);
            write(pipeFd, "Resource allocated", strlen("Resource allocated") + 1);  // Notify client
            break;
        }
    }

    sem_post(manager->mutex);  // Unlock mutex
}

// Function to simulate resource release for a client
void releaseResource(ResourceManager *manager, int clientId, int pipeFd) {
    sem_wait(manager->mutex);  // Lock mutex for resource release

    // Simulate resource release (find and free the resource)
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (manager->resourcePool[i] == clientId) {  // Resource belongs to the client
            manager->resourcePool[i] = 0;  // Free resource
            printf("Client %d released resource %d\n", clientId, i);
            write(pipeFd, "Resource released", strlen("Resource released") + 1);  // Notify client
            break;
        }
    }

    sem_post(manager->mutex);  // Unlock mutex
    sem_post(manager->resourceSem);  // Release the resource back
}

// Client function that requests and releases resources
void clientProcess(int clientId, int priority, int pipeFd) {
    // Simulate resource request
    sleep(rand() % 3);
    printf("Client %d (Priority %d) requesting resource...\n", clientId, priority);
    write(pipeFd, "Requesting resource", strlen("Requesting resource") + 1);
}

int main() {
    srand(time(NULL));

    // Shared memory and resource manager
    int shmId = shmget(IPC_PRIVATE, sizeof(ResourceManager), IPC_CREAT | 0666);
    if (shmId == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    ResourceManager *manager = (ResourceManager *)shmat(shmId, NULL, 0);
    if (manager == (void *)-1) {
        perror("Shared memory attach failed");
        exit(1);
    }

    initializeManager(manager);

    // Create client processes
    pid_t pids[NUM_CLIENTS];
    ClientRequest requests[NUM_CLIENTS];

    // Initialize client requests with random priorities
    for (int i = 0; i < NUM_CLIENTS; i++) {
        requests[i].clientId = i + 1;
        requests[i].priority = rand() % MAX_PRIORITY + 1;  // Priority between 1 and MAX_PRIORITY
    }

    // Sort client requests by priority (higher priority clients first)
    qsort(requests, NUM_CLIENTS, sizeof(ClientRequest), comparePriority);

    for (int i = 0; i < NUM_CLIENTS; i++) {
        int pipeFd[2];
        if (pipe(pipeFd) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }

        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pids[i] == 0) {  // Client process
            close(pipeFd[1]);  // Close write end

            // Request a resource with priority
            clientProcess(requests[i].clientId, requests[i].priority, pipeFd[0]);

            // Read from pipe to simulate resource allocation
            char buffer[BUFFER_SIZE];
            read(pipeFd[0], buffer, sizeof(buffer));
            printf("Client %d received: %s\n", requests[i].clientId, buffer);

            // Release the resource after using it
            releaseResource(manager, requests[i].clientId, pipeFd[0]);

            close(pipeFd[0]);
            exit(0);
        } else {
            close(pipeFd[0]);  // Parent process doesn't need to read
        }
    }

    // Resource manager - Process client requests in priority order
    for (int i = 0; i < NUM_CLIENTS; i++) {
        int pipeFd[2];
        pipe(pipeFd);
        if (fork() == 0) {
            // Simulate resource allocation and release based on priority
            int priority = requests[i].priority;
            allocateResource(manager, requests[i].clientId, priority, pipeFd[1]);
            wait(NULL);
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_CLIENTS; i++) {
        wait(NULL);
    }

    cleanupManager(manager);
    shmctl(shmId, IPC_RMID, NULL);  // Clean up shared memory

    return 0;
}