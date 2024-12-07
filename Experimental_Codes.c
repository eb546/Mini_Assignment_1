#include <stdio.h>
#include <stdlib.h>



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

int main() 
{
    // Task a: Declare an integer variable num and initialize it with the value 42.
    int num = 42;

    // Task b: Declare a pointer ptr that points to an integer.
    int *ptr;

    // Task c: Assign the address of num to ptr.
    ptr = &num;

    // Task d: Print the value of num, the address of num, and the value stored in ptr.
    printf("Value of num: %d\n", num);
    printf("Address of num: %p\n", (void*)&num);
    printf("Value stored in ptr (Address of num): %p\n", (void*)ptr);

    // Task e: Use the pointer to modify the value of num. Set it to 100.
    *ptr = 100;

    // Task f: Print the new value of num to verify the change.
    printf("New value of num: %d\n", num);

    // Task g: Declare an integer array arr with 5 elements: {10, 20, 30, 40, 50}.
    int arr[5] = {10, 20, 30, 40, 50};

    // Task h: Create a pointer arrPtr that points to the first element of arr.
    int *arrPtr = arr;

    // Task i: Use pointer arithmetic to print each element of the array.
    printf("Elements of the array using pointer arithmetic:\n");
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d\n", i, *(arrPtr + i));
    }

    // Task j: Use pointer arithmetic to double the value of each element in the array.
    for (int i = 0; i < 5; i++) {
        *(arrPtr + i) *= 2;
    }

    // Task k: Print the modified array to verify the changes.
    printf("Modified array after doubling the values:\n");
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d\n", i, *(arrPtr + i));
    }

    printf("\n");
    return 0;
}