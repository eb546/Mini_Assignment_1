#include <stdio.h> // This includes the standard library for input/output.
#include <stdlib.h> // This includes the standard library for memory allocation, process control, and conversions.
#include <unistd.h> // This includes the various constants, types and functions relates to the process control pointers like fork(),exec(), getpid().
#include <sys/wait.h> // This includes macros and declaration to handle child processes for process control.
#include <sys/ipc.h> // This includes definitions for inter-process communication or IPC such as shared memory, queues and semaphores.
#include <sys/shm.h> // This includes the shared memory API functios in the Unix systems like shmget(),shmat(), and shmdt().
#include <semaphore.h> // This includes the functions for managing semaphores.
#include <fcntl.h> // This includes mode functions for file control operations such as O_CREAT.
#include <string.h> // This includes functions for string manipulation.
#include <time.h> // This includes functions for manipulating time and date.

#define Max_Jobs 8 // Set the max number of jobs the program can progress.
#define Max_Priority 10 // Set the max priority level in the scheduling or task queue system.
#define Job_Duration 2 // Set the duration in seconds as a timer for a job takes to execute.

// This typedef struct function is for managing jobs.
typedef struct 
{
    int Job_ID; //Using the int function to identifying the job as a variable.

    int Priority; //Using the int function to priorities each jobs from low number to high as a variable.
} Job;

// This typedef struct function is for scheduling jobs.
typedef struct
{
    Job *Queue; // Set the job queue by using a pointer to the dynamically allocated array.

    sem_t *Mutex; //Set a semaphore function with mutual exclusion when a queue is executing.

    sem_t *Slot; //Set a semaphore function to limit the number of jobs in slots while being processed.

    int Job_Count; // Set a counter of jobs in a queue. 

} JobScheduler;

// This void function allows to use priority to compare jobs from lowest to highest.
int Compare_Priority(const void *a, const void *b)
{
    return ((Job*)a)->Priority - ((Job*)b)->Priority; // Set to compare job priorities.
}

// This void function allows to initialise the job scheduler.
void Initialize_Scheduler(JobScheduler * Scheduler)
{
    Scheduler->Queue = (Job*)malloc(Max_Jobs * sizeof(Job)); // Set the dynamic memory allocation for job queue by malloc().
    
    Scheduler->Job_Count = 0; // Executing the job count to 0.

    Scheduler->Mutex = sem_open("/Mutex", O_CREAT, 0666, 1); // Set to execute a semaphore with mutual exclusion function prevents race conditions during the job queue.
    
    Scheduler->Slot = sem_open("/Slot", O_CREAT, 0666, Max_Jobs); // Set to execute a semaphore with slot function to control the jobs whilst being limit of maximum of jobs.

    // Set to check if the semaphore function fails.
    if (Scheduler->Mutex == SEM_FAILED || Scheduler->Slot == SEM_FAILED)
    {
        perror("Status: Semaphore failed"); // Print an status message if the program fails along with a decription of the error. 

        exit(1); // Terminating the program. 
    }
}

// This void function allows to clean up the scheduler and semaphores.
void Cleanup_Scheduler(JobScheduler * Scheduler)
{
    free(Scheduler->Queue); // Set to free the dynamic memory allocation for job queue.

    sem_close(Scheduler->Mutex); // Set to close the mutex semaphore. 

    sem_close(Scheduler->Slot); // Set to close the slot semaphore. 

    sem_unlink("/Mutex"); // Set to unlink the mutex semaphore.

    sem_unlink("/Slot"); // Set to unlink job slot semaphore. 

}

// This void function allows to execute the jobs.
void Execute_Job(int Job_ID, int jobduration, int Pipe_Fd)
{
    printf("Job %d is now executing for %d seconds \n", Job_ID, jobduration); // Prints out the message the which job in the queue is processing in how many seconds.

    sleep(jobduration); // Set to sleep or pause the job's execution by the specified duration.

    write(Pipe_Fd, "Job completed", strlen("Job completed")+1); // Set to write or prints out the completion message to the pipe.
    
    close(Pipe_Fd); // Set to close the write function. 
}

// This void function schedule each jobs based on their priority. 
void Schedule_Job(JobScheduler * Scheduler, int Job_ID, int Priority, int Pipe_Fd)
{
    sem_wait(Scheduler->Slot); // Set for the job to wait for an available slot. 

    sem_wait(Scheduler->Mutex); // Set to lock the job queue to add a new job. 

    Scheduler->Queue[Scheduler->Job_Count].Job_ID = Job_ID; // Set to add a new job in the queue by its ID.

    Scheduler->Queue[Scheduler->Job_Count].Priority = Priority; // Set to add a new job in the queue by its priority.

    Scheduler->Job_Count++; // Set to increase the job count until it reaches the maximum amount of jobs in the program.

    qsort(Scheduler->Queue, Scheduler->Job_Count, sizeof(Job), Compare_Priority); // Arranging the jobs based on its priority.

    sem_post(Scheduler->Mutex); // Release the mutex after the queue is finished executing.

    pid_t pid = fork(); // Using the fork() function to execute the job for a child process. 
    if (pid == 0) 
    {
        Execute_Job(Job_ID, Job_Duration, Pipe_Fd); // Executing the job from the child process.
        exit(0); // Exiting the child process after the job is complete.
    }
}

// This main function to finalize the job scheduling program system. 
int main()
{
    srand(time(NULL)); // Uses to seed random number generator. 

    int Shm_Id = shmget(IPC_PRIVATE, sizeof(JobScheduler), IPC_CREAT | 0666); // Set to create a shared memory for the job scheduler structure. 
    if (Shm_Id == -1)
    {
        perror("Creating shared memory failed"); // Print an status message if the program fails along with a decription of the error. 
        exit(1); // Exiting the child process after the job is complete.
    }


    JobScheduler *Scheduler = (JobScheduler *)shmat(Shm_Id, NULL, 0); // Set to attach the shared memory to the process. 
    if (Scheduler == (void *)-1)
    {
        perror("Attaching shared memory failed"); // Print a status message if the program fails along with a decription of the error. 
        exit(1); // Exiting the child process after the job is complete.
    }

    Initialize_Scheduler(Scheduler); // Set to initialize the job scheduler program.

    for (int i = 0; i < Max_Jobs; i++) // Using the for loop array to process the maximum job interations until it has completed.
    {
        int Pipe_Fd[2]; // Declares an array to store the file descriptors for the pipe. 

        if (pipe(Pipe_Fd) == -1) // Returns -1 if it fails to create the pipe.
        {
            perror("Creating pipe failed"); // Print a status message if the program fails along with a decription of the error. 
            exit(1); // Exiting the child process after the job is complete.
        }

        int Priority = rand() % Max_Priority +1; // Set to generate a random job priority. 

        Schedule_Job(Scheduler, i + 1, Priority, Pipe_Fd[1]); // Set to schedule and write every new job with the generated priority.

        char buffer[256]; // Using the char function to describe the job status from the pipe.
    
        read(Pipe_Fd[0], buffer, sizeof(buffer)); // Using the read function to read the completion message from the pipe.

        printf("Manager received: %s for Job %d!\n", buffer, i +1); // Prints out the message saying that this specific job is complete. 
    
        close(Pipe_Fd[0]); // close the read end of the pipe. 
    }

    for (int i = 0; i < Max_Jobs; i++) // Set a for loop array for all the jobs to execute.
    {
        wait(NULL); // Wait for each job process to finish executing. 
    }

    Cleanup_Scheduler(Scheduler); //Clean up the schedule's resources.
    
    shmctl(Shm_Id, IPC_RMID, NULL); // Remove the shared memory segment.

    return 0; // Return from the main function

}
