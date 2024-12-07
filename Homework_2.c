#include <stdio.h>
#include <stdlib.h>



//void swap(int *a, int *b)
//{
    //int temp = *a;
    //*a = *b;
    //*b = temp;
//}

void* calloc(size_t num_elements, size_t element_size);

int main() 
{
    //int x = 5, y = 10;
    //swap(&x, &y); // x is now 10, y is now 5

    //printf("%d, %d", x, y);

    //return 0;

    int *ptr = (int*)calloc(5, sizeof(int));

    printf("%d", *ptr);

}