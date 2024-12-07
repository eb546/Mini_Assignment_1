#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;

    *a = *b;

    *b = temp;
}

int main()
{
    //int arr[5] = {1, 2, 3, 4, 5};

    //int *p = arr;

    //for(int i = 0; i < 5; i++)

    //{
        //printf("%d ", *(p + i));
    //}


    int x = 5;
    
    int y = 10;

    swap(&x, &y); // x is now 10, y is now 5

    return 0;

}

