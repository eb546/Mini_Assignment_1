#include <stdio.h> //This is to include the standard library header file. It is to set input and output functions and perform their operations.

void Circle_Circumference(); // This is to declare the circumference function from one of the choices selected by the user. The void function helps to perform specifc tasks and seperate different choices.
void Leap_Year(); // This is to declare the leap year function from one of the choices selected by the user. The void function helps to perform specifc tasks and seperate different choices.
void Sum_of_Natural_Numbers(); // This is to declare the natural numbers function from one of the choices selected by the user. The void function helps to perform specifc tasks and seperate different choices.
void Vowel_Checker(); // This is to declare the vowel checker function from one of the choices selected by the user. The void function helps to perform specifc tasks and seperate different choices.

int main() // Starting point and execution of codes begins here.
{
    int choice; // This variable serves to store an integer value from the user's selection from a menu options. Since integer has more bytes, the choice function allows to execute different blocks of code.

    do
    { // The do-while loop allows to repeat this block of code unless the user chooses to exit the menu program entirely.
        printf("\nMain Menu:\n"); // Prints out the main menu message to the terminal.
        printf("1. Circle Circumference\n"); // Prints out the "circle circumference" menu option to the terminal.
        printf("2. Leap Year\n"); // Prints out the "leap year" menu option to the terminal.
        printf("3. Sum of Natural Numbers\n"); // Prints out the "sum of natural numbers" menu option to the terminal.
        printf("4. Vowel Checker\n"); // Prints out the "vowel checker" menu option to the terminal.
        printf("5. Exiting the program.\n"); // Prints out the "exiting the program" menu option to the terminal.

        printf("\nEnter 1-5: "); // Prints out the "number choices" menu option to the terminal for the user to enter the assigned number.
        scanf("%d", &choice); // This function allows the user to enter a number from 1 to 5 by the integer format specifier and the choice(s) action.

        switch (choice) // This function allows to control multiple conditions from the choices being assigned.The switch statement provides to organize different branches and clearly define them.
        {
            case 1: // The first case to assign the first option.
                printf("\nYou selected 1. Circle Cirumference\n"); // Prints out a message at the terminal to show what the user has chosen from one of the menu options.
                Circle_Circumference(); // Execute this specific program being stored.
                break; // Moves on to the next branch codes.
            case 2:
                printf("\nYou selected 2. Leap Year\n"); // Prints out a message at the terminal to show what the user has chosen from one of the menu options.
                Leap_Year(); // Execute this specific program being stored.
                break; // Moves on to the next branch codes.
            case 3:
                printf("\nYou selected 3. Sum of Natural Numbers\n"); // Prints out a message at the terminal to show what the user has chosen from one of the menu options.
                Sum_of_Natural_Numbers(); // Execute this specific program being stored.
                break; // Moves on to the next branch codes.
            case 4:
                printf("\nYou selected 4. Vowel Checker\n"); // Prints out a message at the terminal to show what the user has chosen from one of the menu options.
                Vowel_Checker(); // Execute this specific program being stored.
                break; // Moves on to the next branch codes.
            case 5:
                printf("Exiting the program.\n"); // Prints out a message at the terminal to show what the user has chosen from one of the menu options.
                break; // Moves on to the next branch codes.
        }

        printf("\n"); //Enters a new line
    }while(choice != 5); //While the user chooses option from 1 tom 4, the program will still be in a loop endlessly unless the user enters 5 to exit the program.
}

void Circle_Circumference()
{
    do //Use the do-while loop to have the program on repeat unless the user chooses to exit. 
    {
        const float PI = 3.14159265; //Have the value for Pi as a float decimal number and prevent the coorect value from ever changing. 
        float radius; //Have the radius value as a float decimal number. 
        float circumference; //Have the circumference result value as a float decimal number. 

        printf("\nEnter a radius or 0 to exit: "); //Prints out a message at the terminal for the user to input any positive number as a radius or chooses to exit.
        scanf("%f", &radius); //Assign the input function to be recognize as a float and know it is a radius value.

        if (radius == 0) break; //If the user enters a number 0, it will exit the program and returns to the main menu.

        circumference = 2 * PI * radius; //Have the circumference result a value from the proper equation. 

        printf("The circumference of the circle: %f \n", circumference); //Prints out the result of the circle of the circumference at the terminal. 
        printf(" "); //Avoid any new line errors.

    } while(1); //While the program isn't returning a zero, the program will stay inside the loop function. 
}

void Leap_Year()
{
    int year; //Assign a variable to have "year" as an integer. 
    do //Use the do-while loop to have the program on repeat unless the user chooses to exit. 
    {
        printf("\nEnter a year or 0 to exit: "); //Prints out a message at the terminal for the user to input any positive number as years or chooses to exit.
        scanf("%d", &year); //Assign the input function to be recognize as an integer by the format specifier and know the value as a year being declared.

        if (year == 0) break; //If the user enters a number 0, it will exit the program and returns to the main menu.

        if (year % 400 == 0) //If the value is divisible by 400
            {
                printf("%d is a leap year.\n", year); //Prints out a message at the terminal saying that value is a leap year.
                printf(" "); //Avoid any new line errors.
            }

        else if (year % 100 == 0) //If the value is divisible by 100 but not 400
            {
                printf("%d is not a leap year.\n", year); //Prints out a message at the terminal saying that value is not a leap year.
                printf(" "); //Avoid any new line errors.
            }

        else if (year % 4 == 0) //If the value is divisible by 4 but not 100
            {
                printf("%d is a leap year.\n", year); //Prints out a message at the terminal saying that value is a leap year.
                printf(" "); //Avoid any new line errors.
            }

        else //Everything else besides that is not considered as a leap year.
            {
                printf("%d is not a leap year.\n", year);
                printf(" ");
            }

    } while (1); //While the program isn't returning a zero, the program will stay inside the loop function.
}


void Sum_of_Natural_Numbers()
{
    int num; //Assign the variable for the num as an integer.
    int sum = 0; //Assign the variable for the sum as an integer. 

    printf("Enter a positive number: "); //Prints out a message at the terminal for the user to input any positive integers.
    scanf("%d", &num); //Assign the input function to be recognize as an integer by a format specifier and know the variable "num" being declared.

    for(int i = 1; i <= num; ++i) //Use the for loop to intialize; condition; increment/decrement.
    {
        sum += i; //Have the variable "sum" up to the user's input number. 
    }

    printf("The sum of natural numbers is: %d \n", sum);// This prints out a message displaying the result of the natural numbers

}

void Vowel_Checker()
{
    char a; //Set the char variable for the user to input an alphabet.
    char s[] ="vowel!"; //Set an array of chars for printing out at the terminal. 
    int lowercase_vowel, uppercase_vowel; //Set both the lowercase and uppercasen vowels as a variable to store as integers.

    do
    {

        printf("\nEnter an alphabet (z to exit): "); //Prints out a message for the user to input an alphabet unless enter a letter "z" tp exit.
        scanf("%c", &a); //Assign the input function to be recognize as a character by the format specifier and know the variable "a" being declared.

        if (a == 'z') break; //If the user inputs a letter "z", it exit the program then returns to the main menu.

        lowercase_vowel = (a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u'); // Set to list out all the vowels as lowercase by variable "a".

        uppercase_vowel = (a == 'A' || a == 'E' || a == 'I' || a == 'O' || a == 'U'); //Set to list out all the vowels as uppercase by variable "a".

        if (lowercase_vowel || uppercase_vowel) //If the char a match to the variable upper and lowercase vowels.
            {
                printf("%c is a %c%c%c%c%c%c \n", a, s[0], s[1], s[2], s[3], s[4], s[5]); //Prints out a message to clarify that the letter is a vowel along with the uses of array of chars for the word "vowel" by using a format specifier.
            }

        else{ //If it is not, then it will ignore that letter and continue asking for a vowel.
        }

    } while(1); //While the program isn't returning a zero, the program will stay inside the loop function. 
}
