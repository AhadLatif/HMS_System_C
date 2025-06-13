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
#include <ctype.h>

void mainFunction()
{

    int selection;
    do
    {

        printf("Select an option:\n");
        printf("1. Doctor Module\n");
        printf("2. Patient Module\n");
        printf("3. Exit Program \n");
        printf("Enter your choice: ");
        selection = inputInt("Choose ");

    } while (selection != 1 && selection != 2 && selection != 3);

    switch (selection)
    {

    case 1:
        doctorModule();
        break;
    case 2:
        patientModule();
        break;
    case 3:
        exitProgram();
        break;
    default:
        printf("Invalid option.\n");
        break;
    }
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
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
}

int inputValidatedGender(char *gender, int size)
{
    char input[20];
    int valid = 0;
    while (!valid)
    {
        printf("Choose Gender:\n1. Male\n2. Female\n3. Other\nEnter your choice: ");
        inputString(input, sizeof(input));
        if (strcasecmp(input, "1") == 0 || strcasecmp(input, "male") == 0 || strcasecmp(input, "m") == 0)
        {
            strncpy(gender, "male", size);
            valid = 1;
        }
        else if (strcasecmp(input, "2") == 0 || strcasecmp(input, "female") == 0 || strcasecmp(input, "f") == 0)
        {
            strncpy(gender, "female", size);
            valid = 1;
        }
        else if (strcasecmp(input, "3") == 0 || strcasecmp(input, "other") == 0 || strcasecmp(input, "o") == 0)
        {
            strncpy(gender, "other", size);
            valid = 1;
        }
        else
        {
            printf("Invalid gender. Please choose 'male', 'female', or 'other'.\n");
        }
    }
    return 1;
}

int inputValidatedAge()
{
    int age = -1;
    while (1)
    {
        age = inputInt("Enter age: ");
        if (age >= 0 && age <= 120)
        {
            return age;
        }
        else
        {
            printf("Invalid age. Please enter a valid age (0-120).\n");
        }
    }
}

int inputValidatedCNIC(char *cnic, int size)
{
    char input[20];
    int valid = 0;
    while (!valid)
    {
        printf("Enter CNIC (13 digits, numbers only): ");
        inputString(input, sizeof(input));
        int len = strlen(input);
        valid = (len == 13);
        for (int i = 0; i < len && valid; i++)
        {
            if (!isdigit((unsigned char)input[i]))
                valid = 0;
        }
        if (!valid)
        {
            printf("Invalid CNIC. Please enter exactly 13 digits.\n");
        }
    }
    strncpy(cnic, input, size);
    return 1;
}

int inputValidatedContact(char *contact, int size)
{
    char input[20];
    int valid = 0;
    while (!valid)
    {
        printf("Enter contact number (numbers only): ");
        inputString(input, sizeof(input));
        int len = strlen(input);
        valid = (len >= 7 && len <= 15);
        for (int i = 0; i < len && valid; i++)
        {
            if (!isdigit((unsigned char)input[i]))
                valid = 0;
        }
        if (!valid)
        {
            printf("Invalid contact number. Please enter 7-15 digits.\n");
        }
    }
    strncpy(contact, input, size);
    return 1;
}

int inputValidatedName(char *name, int size)
{
    char input[60];
    int valid = 0;
    while (!valid)
    {
        printf("Enter name (alphabets and spaces only): ");
        inputString(input, sizeof(input));
        valid = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (!isalpha((unsigned char)input[i]) && input[i] != ' ')
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("Invalid name. Please use alphabets and spaces only.\n");
        }
    }
    strncpy(name, input, size);
    return 1;
}

int inputValidatedBloodGroup(char *blood_group, int size)
{
    char input[10];
    int valid = 0;
    while (!valid)
    {
        printf("Enter blood group (e.g., A+, O-, AB+): ");
        inputString(input, sizeof(input));
        valid = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (!isalpha((unsigned char)input[i]) && input[i] != '+' && input[i] != '-')
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("Invalid blood group. Please use only alphabets, +, or -.\n");
        }
    }
    strncpy(blood_group, input, size);
    return 1;
}
