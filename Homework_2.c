#include <stdio.h> // This includes the standard libary for input/output.
#include <stdlib.h> // This includes the standard library for memory allocation, process control, and conversions.
#include <unistd.h> // This includes the various constants, types and functions relates to the process control pointers like fork(),exec(), getpid().
#include <sys/wait.h> // This includes macros and declaration to handle child processes for process conntrol.
#include <sys/ipc.h> // This includes definitions for inter-process communication or IPC such as shared memory, queues and semaphores.
#include <sys/shm.h> // This includes the shared memory API functios in the Unix systems like shmget(),shmat(), and shmdt().
#include <semaphore.h> // This includes the functions for managing semaphores.
#include <fcntl.h> // This includes mode functions for file control operations such as O_CREAT.
#include <string.h> // This includes functions for string manipulation.
#include <time.h> // This includes fucntions for manipulating time and date.

#define Max_Jobs 8 // Set the max number of jobs the program can progress.
#define Max_Priority 10 // Set the max priority level in the scheduling or task queue system.
#define Job_Duration 2 // Set the durationion seconds as a timner for a job takes to execute.

// This typedef struct function is for managing jobs.
typedef struct 
{
    int Job_ID; //Using the int function to identifying the job as a variable
    int Priority; //Using the int function to prioritise each jobs from low number to high as a variable.
} Job;

// This typedef struct function is for scheduling jobs.
typedef struct
{
    Job *Queue; // Set the job queue by dynamically allocated array.
    sem_t *Mutex; //Set a semaphore function for mutual exclusion when a queue is executing.
    sem_t *Slot; //Set a semaphore function to limit the number of jobs in slots while being processed.
    int Job_Count; // Set a counter of jobs in a queue. 
} JobScheduler;

// This void function allpws to use priority to compare jobs from lowest to highest.
int Compare_Priority(const void *a, const void *b)
{
    return ((Job*)a)->Priority - ((Job*)b)->Priority; // Set to compare job priorities.
}

// This void function allows to intialize the job scheduler.
void Initialize_Scheduler(JobScheduler * Scheduler)
{
    Scheduler->Queue = (Job*)malloc(Max_Jobs * sizeof(Job)); // Set the dynamic memory allocation for job queue by malloc().
    Scheduler->Job_Count = 0; // Executing the job count to 0.

    Scheduler->Mutex = sem_open("/Mutex", O_CREAT, 0666, 1); // Set to execute a semaphore with mutual exclusion function prevents race conditions during the job queue.
    Scheduler->Slot = sem_open("/Slot", O_CREAT, 0666, Max_Jobs); // Set to execute a semaphore with slot function to control the jobs whilst being limit of maximum of jobs.

    // Set to check if the semaphore function fails.
    if (Scheduler->Mutex == SEM_FAILED || Scheduler->Slot == SEM_FAILED)
    {
        perror("Status: Semaphore failed");
        exit(1);
    }
}

void Cleanup_Scheduler(JobScheduler * Scheduler)
{
    free(Scheduler->Queue);
    sem_close(Scheduler->Mutex);
    sem_close(Scheduler->Slot);
    sem_unlink("/Mutex");
    sem_unlink("/Slot");

}

void Execute_Job(int Job_ID, int jobduration, int Pipe_Fd)
{
    printf("Job %d is now executing for %d seconds \n", Job_ID, jobduration);
    sleep(jobduration);

    write(Pipe_Fd, "Job Completed!", strlen("Job Completed!")+1);
    close(Pipe_Fd);
}

void Schedule_Job(JobScheduler * Scheduler, int Job_ID, int Priority, int Pipe_Fd)
{
    sem_wait(Scheduler->Slot);

    sem_wait(Scheduler->Mutex);

    Scheduler->Queue[Scheduler->Job_Count].Job_ID = Job_ID;

    Scheduler->Queue[Scheduler->Job_Count].Priority = Priority;

    Scheduler->Job_Count++;

    qsort(Scheduler->Queue, Scheduler->Job_Count, sizeof(Job), Compare_Priority);

    sem_post(Scheduler->Mutex);

    pid_t pid = fork();

    if (pid == 0)
    {
        Execute_Job(Job_ID, Job_Duration, Pipe_Fd);
        exit(0);
    }
}

int main()
{
    srand(time(NULL));

    int Shm_Id = shmget(IPC_PRIVATE, sizeof(JobScheduler), IPC_CREAT | 0666);

    if (Shm_Id == -1)
    {
        perror("Creating shared memory failed");
        exit(1);
    }

    JobScheduler *Scheduler = (JobScheduler *)shmat(Shm_Id, NULL, 0);

    if (Scheduler == (void *)-1)
    {
        perror("Attaching shared memory failed");
        exit(1);
    }

    Initialize_Scheduler(Scheduler);

    for (int i = 0; i < Max_Jobs; i++)
    {
        int Pipe_Fd[2];

        if (pipe(Pipe_Fd) == -1)
        {
            perror("Creating pipe failed");

            exit(1);
        }

        int Priority = rand() % Max_Priority +1;

        Schedule_Job(Scheduler, i + 1, Priority, Pipe_Fd[1]);

        char buffer[256];
    
        read(Pipe_Fd[0], buffer, sizeof(buffer));

        printf("Manager received: %s for Job %d\n", buffer, i +1);
    
        close(Pipe_Fd[0]);
    }

    for (int i = 0; i < Max_Jobs; i++)
    {
        wait(NULL);
    }

    Cleanup_Scheduler(Scheduler);
    
    shmctl(Shm_Id, IPC_RMID, NULL);

    return 0;

}
