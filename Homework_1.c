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
    int x = 5, y = 10;
    swap(&x, &y); // x is now 10, y is now 5
    return 0;
}