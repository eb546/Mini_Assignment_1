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
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 10
#define ITEM_COUNT 20

// Shared structure
typedef struct {
    int *buffer;   // Dynamically allocated buffer
    int in;         // Index for producer to add items
    int out;        // Index for consumer to consume items
    sem_t *mutex;   // Semaphore for mutual exclusion
    sem_t *empty;   // Semaphore to count empty spaces in buffer
    sem_t *full;    // Semaphore to count full items in buffer
} SharedData;

// Producer function
void producer(SharedData *data) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        // Wait for an empty slot
        sem_wait(data->empty);
        
        // Lock the mutex for exclusive access to the buffer
        sem_wait(data->mutex);
        
        // Produce an item (just a number here)
        data->buffer[data->in] = i;
        printf("Produced: %d\n", i);
        data->in = (data->in + 1) % BUFFER_SIZE;
        
        // Unlock the mutex
        sem_post(data->mutex);
        
        // Signal that there is a new full item
        sem_post(data->full);
        
        // Simulate some production time
        usleep(500000);  // Sleep for 0.5 seconds
    }
}

// Consumer function
void consumer(SharedData *data) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        // Wait for a full item to consume
        sem_wait(data->full);
        
        // Lock the mutex for exclusive access to the buffer
        sem_wait(data->mutex);
        
        // Consume an item
        int item = data->buffer[data->out];
        printf("Consumed: %d\n", item);
        data->out = (data->out + 1) % BUFFER_SIZE;
        
        // Unlock the mutex
        sem_post(data->mutex);
        
        // Signal that there is an empty slot
        sem_post(data->empty);
        
        // Simulate some consumption time
        usleep(1000000);  // Sleep for 1 second
    }
}

int main() {
    // Shared memory creation
    int shm_id = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach the shared memory
    SharedData *data = (SharedData *)shmat(shm_id, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Dynamically allocate memory for the buffer
    data->buffer = (int *)malloc(sizeof(int) * BUFFER_SIZE);
    if (data->buffer == NULL) {
        perror("malloc failed");
        exit(1);
    }

    // Initialize semaphores
    data->mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    data->empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    data->full = sem_open("/full", O_CREAT, 0666, 0);
    if (data->mutex == SEM_FAILED || data->empty == SEM_FAILED || data->full == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    // Initialize buffer indices
    data->in = 0;
    data->out = 0;

    // Fork a producer process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process (Producer)
        producer(data);
        exit(0);
    } else {  // Parent process (Consumer)
        consumer(data);
        wait(NULL);  // Wait for the producer to finish
    }

    // Cleanup
    free(data->buffer);
    sem_close(data->mutex);
    sem_close(data->empty);
    sem_close(data->full);
    shmctl(shm_id, IPC_RMID, NULL);  // Remove shared memory
    sem_unlink("/mutex");
    sem_unlink("/empty");
    sem_unlink("/full");

    return 0;
}