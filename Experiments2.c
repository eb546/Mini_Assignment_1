#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#define NUM_TASKS 3

// Structure to represent a task
typedef struct {
    int *array;         // Pointer to the array
    int size;           // Size of the array
    int priority;       // Priority of the task (lower value means higher priority)
    int pipe_fd[2];     // Pipe for communication with parent
} Task;

// Semaphore for synchronizing processes
sem_t *semaphore;

void compute_task(Task *task) {
    int sum = 0;
    for (int i = 0; i < task->size; i++) {
        sum += task->array[i];
    }
    // Send the result to the parent process through the pipe
    write(task->pipe_fd[1], &sum, sizeof(sum));
    close(task->pipe_fd[1]);
    printf("Task with priority %d completed. Sum = %d\n", task->priority, sum);
}

int compare_tasks(const void *a, const void *b) {
    return ((Task*)a)->priority - ((Task*)b)->priority;
}

int main() {
    // Initialize a semaphore (binary semaphore for synchronization)
    semaphore = sem_open("/task_semaphore", O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    Task tasks[NUM_TASKS];
    pid_t pids[NUM_TASKS];
    int result, status;
    
    // Create tasks with different priorities
    for (int i = 0; i < NUM_TASKS; i++) {
        tasks[i].size = 10;
        tasks[i].priority = NUM_TASKS - i; // Higher priority for smaller numbers
        tasks[i].array = (int *)malloc(tasks[i].size * sizeof(int));
        
        // Initialize array with random numbers
        for (int j = 0; j < tasks[i].size; j++) {
            tasks[i].array[j] = rand() % 100;
        }
        
        pipe(tasks[i].pipe_fd); // Create pipe for each task
    }

    // Sort tasks based on priority (higher priority tasks come first)
    qsort(tasks, NUM_TASKS, sizeof(Task), compare_tasks);

    // Fork processes to execute the tasks
    for (int i = 0; i < NUM_TASKS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process: Compute the task
            sem_wait(semaphore); // Wait for semaphore before computing
            compute_task(&tasks[i]);
            sem_post(semaphore); // Release the semaphore
            exit(0); // Exit child process
        }
    }

    // Parent process: Wait for all child processes to finish and collect results
    for (int i = 0; i < NUM_TASKS; i++) {
        waitpid(pids[i], &status, 0);
        read(tasks[i].pipe_fd[0], &result, sizeof(result));
        printf("Parent received result from task with priority %d: Sum = %d\n", tasks[i].priority, result);
        close(tasks[i].pipe_fd[0]);
        free(tasks[i].array); // Free dynamically allocated memory
    }

    // Clean up the semaphore
    sem_close(semaphore);
    sem_unlink("/task_semaphore");

    return 0;
}

