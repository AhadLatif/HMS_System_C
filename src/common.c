#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "patient.h"
#include "common.h"
#include "doctor.h"
#include "billing.h"

void displayMenu(void)
{

    printf("\n--- Patient Management System ---\n");
    printf("1. Add a Patient\n");
    printf("2. Display All Patients\n");
    printf("3. Search for a Patient\n");
    printf("4. Delete a Patient\n");
    printf("5. Exit\n");
}

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

    savePatientsToFile(); //
    printf("\n All patient data saved successfully!\n");
    printf("Thank you for using the Patient Management System. Goodbye!\n");
    return; //<-----this
}

int inputInt(const char *prompt)
{
    char buffer[100];
    int value;

    while (1)
    {
        printf("%s", prompt);

        // Get input as a string
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            // Remove newline character if present
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
            }

            // Try to parse as integer
            char extra;
            if (sscanf(buffer, "%d %c", &value, &extra) == 1)
            {
                return value;
            }
            else
            {
                printf("Invalid input. Please enter an integer.\n");
            }
        }
        else
        {
            printf(" Error reading input.\n");
        }
    }
}
