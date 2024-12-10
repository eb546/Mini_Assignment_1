//#include <stdio.h>
//#include <stdlib.h>



//void swap(int *a, int *b)
//{
    //int temp = *a;
    //*a = *b;
    //*b = temp;
//}

//void* calloc(size_t num_elements, size_t element_size);

//int main() 
//{
    //int x = 5, y = 10;
    //swap(&x, &y); // x is now 10, y is now 5

    //printf("%d, %d", x, y);

    //return 0;

    //int *ptr = (int*)calloc(5, sizeof(int));

    //printf("%d", *ptr);

//}

//int main() 
//{
    // Task a: Declare an integer variable num and initialize it with the value 42.
    //int num = 42;

    // Task b: Declare a pointer ptr that points to an integer.
    //int *ptr;

    // Task c: Assign the address of num to ptr.
    //ptr = &num;

    // Task d: Print the value of num, the address of num, and the value stored in ptr.
    //printf("Value of num: %d\n", num);
    //printf("Address of num: %p\n", (void*)&num);
    //printf("Value stored in ptr (Address of num): %p\n", (void*)ptr);

    // Task e: Use the pointer to modify the value of num. Set it to 100.
    //*//ptr = 100;

    // Task f: Print the new value of num to verify the change.
    //printf("New value of num: %d\n", num);

    // Task g: Declare an integer array arr with 5 elements: {10, 20, 30, 40, 50}.
    //int arr[5] = {10, 20, 30, 40, 50};

    // Task h: Create a pointer arrPtr that points to the first element of arr.
    //int *arrPtr = arr;

    // Task i: Use pointer arithmetic to print each element of the array.
    //printf("Elements of the array using pointer arithmetic:\n");
    //for (int i = 0; i < 5; i++) {
        //printf("arr[%d] = %d\n", i, *(arrPtr + i));
    //}

    // Task j: Use pointer arithmetic to double the value of each element in the array.
    //for (int i = 0; i < 5; i++) {
        //*(arrPtr + i) *= 2;
    //}

    // Task k: Print the modified array to verify the changes.
    //printf("Modified array after doubling the values:\n");
    //for (int i = 0; i < 5; i++) {
        //printf("arr[%d] = %d\n", i, *(arrPtr + i));
    //}

    //printf("\n");
    //return 0;
//}

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
#define PRIORITY_THRESHOLD 5

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
    int priority;
} ClientRequest;

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
            int priority = rand() % 10;  // Random priority
            clientProcess(i + 1, priority, pipeFd[0]);

            // Read from pipe to simulate resource allocation
            char buffer[BUFFER_SIZE];
            read(pipeFd[0], buffer, sizeof(buffer));
            printf("Client %d received: %s\n", i + 1, buffer);

            // Release the resource after using it
            releaseResource(manager, i + 1, pipeFd[0]);

            close(pipeFd[0]);
            exit(0);
        } else {
            close(pipeFd[0]);  // Parent process doesn't need to read
        }
    }

    // Resource manager - Process client requests based on priority
    for (int i = 0; i < NUM_CLIENTS; i++) {
        int pipeFd[2];
        pipe(pipeFd);
        if (fork() == 0) {
            // Simulate resource allocation and release based on priority
            int priority = rand() % 10;
            allocateResource(manager, i + 1, priority, pipeFd[1]);
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