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

#define MAX_JOBS 4  // Max number of jobs
#define MAX_PRIORITY 10  // Max priority level (1 = highest)
#define JOB_DURATION 2  // Simulate job execution duration in seconds

// Job structure for managing jobs
typedef struct {
    int jobId;
    int priority;  // Lower value means higher priority
} Job;

// Resource Manager for job scheduling
typedef struct {
    Job *jobQueue;         // Dynamically allocated queue for jobs
    sem_t *mutex;          // Semaphore for mutual exclusion
    sem_t *jobSlotSem;     // Semaphore to limit the number of jobs being processed
    int jobCount;          // Number of active jobs
} JobScheduler;

// Function to compare jobs by priority (used for sorting)
int comparePriority(const void *a, const void *b) {
    return ((Job*)a)->priority - ((Job*)b)->priority;
}

// Function to initialize job scheduler and resources
void initializeScheduler(JobScheduler *scheduler) {
    scheduler->jobQueue = (Job *)malloc(MAX_JOBS * sizeof(Job));  // Dynamically allocate job queue
    scheduler->jobCount = 0;

    // Initialize semaphores
    scheduler->mutex = sem_open("/job_mutex", O_CREAT, 0666, 1);   // Mutex for job scheduling
    scheduler->jobSlotSem = sem_open("/job_slot", O_CREAT, 0666, MAX_JOBS);  // Semaphore to limit job execution slots

    if (scheduler->mutex == SEM_FAILED || scheduler->jobSlotSem == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(1);
    }
}

// Function to clean up scheduler and semaphores
void cleanupScheduler(JobScheduler *scheduler) {
    free(scheduler->jobQueue);  // Free dynamically allocated memory for job queue
    sem_close(scheduler->mutex);
    sem_close(scheduler->jobSlotSem);
    sem_unlink("/job_mutex");
    sem_unlink("/job_slot");
}

// Function to simulate job execution
void executeJob(int jobId, int jobDuration, int pipeFd) {
    printf("Job %d is executing for %d seconds...\n", jobId, jobDuration);
    sleep(jobDuration);  // Simulate work by sleeping for job duration

    // Notify the manager about job completion via the pipe
    write(pipeFd, "Job Completed", strlen("Job Completed") + 1);
    close(pipeFd);  // Close pipe after sending completion message
}

// Function to handle job scheduling based on priority
void scheduleJob(JobScheduler *scheduler, int jobId, int priority, int pipeFd) {
    sem_wait(scheduler->jobSlotSem);  // Wait for an available job slot

    // Lock the job queue for modification
    sem_wait(scheduler->mutex);
    
    // Add the new job to the queue
    scheduler->jobQueue[scheduler->jobCount].jobId = jobId;
    scheduler->jobQueue[scheduler->jobCount].priority = priority;
    scheduler->jobCount++;
    
    // Sort the jobs based on priority (lowest priority number = highest priority)
    qsort(scheduler->jobQueue, scheduler->jobCount, sizeof(Job), comparePriority);
    
    sem_post(scheduler->mutex);  // Release the mutex after modifying the queue

    // Fork a child process to simulate job execution
    pid_t pid = fork();
    if (pid == 0) {
        // Child process (job execution)
        executeJob(jobId, JOB_DURATION, pipeFd);
        exit(0);  // Exit child process
    }
}

// Main function to simulate the Job Scheduling System
int main() {
    srand(time(NULL));

    // Create shared memory for the scheduler
    int shmId = shmget(IPC_PRIVATE, sizeof(JobScheduler), IPC_CREAT | 0666);
    if (shmId == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    JobScheduler *scheduler = (JobScheduler *)shmat(shmId, NULL, 0);
    if (scheduler == (void *)-1) {
        perror("Shared memory attach failed");
        exit(1);
    }

    // Initialize the job scheduler
    initializeScheduler(scheduler);

    // Simulate job submission by different clients
    for (int i = 0; i < MAX_JOBS; i++) {
        int pipeFd[2];
        if (pipe(pipeFd) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }

        // Random job priority (1 to MAX_PRIORITY)
        int priority = rand() % MAX_PRIORITY + 1;

        // Schedule job based on priority and create child processes
        scheduleJob(scheduler, i + 1, priority, pipeFd[1]);

        // Read the job completion status from the pipe
        char buffer[256];
        read(pipeFd[0], buffer, sizeof(buffer));
        printf("Manager received: %s for Job %d\n", buffer, i + 1);

        close(pipeFd[0]);  // Close the read end of the pipe
    }

    // Wait for all jobs to finish (child processes)
    for (int i = 0; i < MAX_JOBS; i++) {
        wait(NULL);  // Wait for each child job process
    }

    // Clean up resources
    cleanupScheduler(scheduler);
    shmctl(shmId, IPC_RMID, NULL);  // Clean up shared memory

    return 0;
}