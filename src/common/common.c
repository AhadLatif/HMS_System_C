#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "src/patient/patient.h"
#include "src/common/common.h"
#include "src/doctor/doctor.h"
#include "src/billing/billing.h"



void welcomScreen(void)
{
    printf("\n\n\n\n\n\n\n\t\t\t\t--------------------------------------------");
    printf("\n\n\t\t\t\t\t\t< WELCOME TO >");
    printf("\n\n\t\t\t\t\t< HOSPITAL MANAGEMENT SYSTEM >");
    printf("\n\t\t\t\t--------------------------------------------");
    printf("\n\n\n\n\n Press any key to continue......\n");
    getch();
    //  system("cls");//Use to clear screen
}

void login()
{
    printf("LOGIN\n");
}

void inputString(char *target, int size)
{
    int ch;

    if (fgets(target, size, stdin) != NULL)
    {
        size_t len = strlen(target);
        if (len > 0 && target[len - 1] == '\n')
        {
            target[len - 1] = '\0';
        }
        else
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
        }
    }
}
int fileCheck(FILE *file)
{
    if (!file)
    {
        printf("Error while opening file\n");
        exit(EXIT_FAILURE); //<-----this
    }
    return 0;
}

void exitProgram(void)
{
    savePatientsToFile();
    printf("\n All patient data saved successfully!\n");
    printf("Thank you for using the Patient Management System. Goodbye!\n");
    exit(0); // Actually exit the program
}

void formatRegistrationTime(time_t registration_time, char *buffer, size_t bufferSize)
{

                                                            // First you have to create
                                                            //  time_t currentTime = time(NULL);
                                                            // char reg_time_str[25];

                                                            // then call this function
                                                            // formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));

    if (buffer == NULL || bufferSize == 0)
    {
        printf("Invalid buffer provided in formatRegistrationTime\n");
        return; // Invalid buffer
    }
    struct tm *tm_info = localtime(&registration_time); 
    strftime(buffer, bufferSize, "%H:%M:%S %d/%m/%Y", tm_info);
}

int inputInt(const char *prompt)
{
    char buffer[100];
    int value;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error reading input.\n");
            continue;
        }

        // Trim trailing newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Check if input is a valid integer (and only integer)
        char extra;
        if (sscanf(buffer, " %d %c", &value, &extra) == 1)
        {
            return value;
        }
        else
        {
            printf("❌ Invalid input. Please enter a valid integer.\n");
        }
    }
}
