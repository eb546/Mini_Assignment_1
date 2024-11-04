#include <stdio.h>

void Circle_Circumference();
void Leap_Year();
void Sum_of_Natural_Numbers();
void Vowel_Checker();

int main()
{
    int choice;

    do{
        printf("\nMain Menu:\n");
        printf("1. Circle Circumference\n");
        printf("2. Leap Year\n");
        printf("3. Sum of Natural Numbers\n");
        printf("4. Vowel Checker\n");
        printf("5. Exiting the program.\n");

        printf("Enter 1-5: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("\nYou selected 1. Circle Cirumference\n");
                Circle_Circumference();
                break;
            case 2:
                printf("\nYou selected 2. Leap Year\n");
                Leap_Year();
                break;
            case 3:
                printf("\nYou selected 3. Sum of Natural Numbers\n");
                Sum_of_Natural_Numbers();
                break;
            case 4:
                printf("\nYou selected 4. Vowel Checker\n");
                Vowel_Checker();
                break;
            case 5:
                printf("Exiting the program.\n");
                break;


        }
        printf("\n");
    }while(choice != 5);

}



void Circle_Circumference()
{
    do{
        const float PI = 3.14159265;
        float radius;
        float circumference;

        printf("\nEnter a radius or 0 to exit: ");
        scanf("%f", &radius);

        if(radius == 0) break;

        circumference = 2 * PI * radius;

        printf("The circumference of the circle is %f", circumference);
    }while(1);

}

void Leap_Year()
{
    int year;
    do
    {
        printf("\nEnter a year or 0 to exit: ");
        scanf("%d", &year);

        if(year == 0 ) break;

        if(year % 400 == 0)
        {
            printf("%d is a leap year.", year);
        }

        else if(year % 100 == 0)
        {
            printf("%d is not a leap year.", year);
        }

        else if(year % 4 == 0)
        {
            printf("%d is a leap year", year);
        }

        else{
            printf("%d is not a leap year", year);
        }
    } while(1);
}


void Sum_of_Natural_Numbers()
{
    
}
    

void Vowel_Checker()
{

}

