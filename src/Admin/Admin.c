#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "../common/common.h"
#include "../id_manager/id_manager.h"
#include <time.h>
#include "Admin.h"
ADMIN ADMINs[MAX_ADMINS];
int ADMIN_counter = 0;
void ADMINModule()
{
    loadADMINsFromFile();
    // loadIDManagerFromFile();
    int option;
    printf("Select an option:\n");
    printf("1. Register\n");
    printf("2. Login\n");
    option = inputInt("Enter your choice:");

    if (option == 1)
    {
        addADMIN();
    }
    else if (option == 2)
    {
        searchADMINByCnic();
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
    }
}

// Add ADMIN
// ...existing code...
void addADMIN()
{
    ADMIN new_admin;
    inputValidatedName(new_admin.d_name, sizeof(new_admin.d_name));

    int unique = 0;
    while (!unique)
    {
        inputValidatedCNIC(new_admin.d_cnic, sizeof(new_admin.d_cnic));
        int found = 0;
        for (int i = 0; i < ADMIN_counter; i++)
        {
            if (strcmp(ADMINs[i].d_cnic, new_admin.d_cnic) == 0)
            {
                found = 1;
                printf("This CNIC is already assigned to admin: %s (ID: %s)\n", ADMINs[i].d_name, ADMINs[i].d_id);
                printf("Is it you? (y/n): ");
                char answer[4];
                inputString(answer, sizeof(answer));
                if (answer[0] == 'y' || answer[0] == 'Y')
                {
                    int attempts = 0;
                    char password[32];
                    while (attempts < 3)
                    {
                        printf("Enter your password: ");
                        inputString(password, sizeof(password));
                        if (strcmp(ADMINs[i].password, password) == 0)
                        {
                            printf("Welcome back, %s!\n", ADMINs[i].d_name);
                            // Redirect to main admin module if needed
                            mainFunction();
                            return;
                        }
                        else
                        {
                            printf("Incorrect password. ");
                            attempts++;
                            if (attempts < 3)
                            {
                                printf("Try again.\n");
                            }
                        }
                    }
                    printf("Too many incorrect attempts. Program will terminate.\n");
                    exit(1);
                }
                else
                {
                    printf("Please enter a different CNIC.\n");
                }
                break; // break for loop, will repeat while loop if not unique
            }
        }
        if (!found)
        {
            unique = 1;
        }
    }

    printf("Enter password: ");
    inputString(new_admin.password, sizeof(new_admin.password));
    new_admin.registration_time = time(NULL);
    new_admin.status = ADMIN_ACTIVE;
    snprintf(new_admin.d_id, sizeof(new_admin.d_id), "ADMIN%03d", ADMIN_counter + 1);
    ADMINs[ADMIN_counter] = new_admin;
    ADMIN_counter++;
    saveADMINsToFile();
    printf("New ADMIN added successfully with ID: %s\n", new_admin.d_id);

    // Return to login menu
    ADMINModule();
}

// ..
// }.existing code...
// // Search Functions
// void searchADMIN()
// {
// }
void searchADMINByCnic()
{
    char check_cnic[16];
    printf("Enter your CNIC: ");
    inputValidatedCNIC(check_cnic, sizeof(check_cnic));
    for (int i = 0; i < ADMIN_counter; i++)
    {
        if (ADMINs[i].status == ADMIN_ACTIVE && (strcmp(ADMINs[i].d_cnic, check_cnic)) == 0)
        {
            printf("ADMIN found:\n");
            printf("ID: %s\n", ADMINs[i].d_id);
            printf("Enter your password: ");
            char password[20];
            inputString(password, sizeof(password));
            if (strcmp(ADMINs[i].password, password) == 0)
            {
                printf("Login successful!\n");
                // Proceed to ADMIN main module
                mainFunction();
                return;
            }
            else
            {
                printf("Incorrect password. Please try again.\n");
                return;
            }
        }
    }
    // Only print if no admin was found
    printf("No ADMIN found with the provided CNIC.\n");
}

// File Operations
void saveADMINsToFile()
{
    FILE *file = fopen("./data/Admin.dat", "wb");
    if (file == NULL)
    {
        printf("Failed to open file for saving ADMINs");
        return;
    }
    for (int i = 0; i < ADMIN_counter; i++)
    {
        if (fwrite(&ADMINs[i], sizeof(ADMIN), 1, file) != 1)
        {
            printf("Error writing ADMIN %d to file.\n", i + 1);
        }
    }
    fclose(file);

    printf("ADMINs saved to file successfully.\n");
}
void loadADMINsFromFile()
{
    FILE *file = fopen("./data/Admin.dat", "rb");
    if (!file)
    {
        printf("No existing ADMIN data file found. Starting with empty database.\n");
        ADMIN_counter = 0;
        return;
    }

    ADMIN_counter = 0; // Reset counter before loading
    while (fread(&ADMINs[ADMIN_counter], sizeof(ADMIN), 1, file) != 0 && ADMIN_counter < MAX_ADMINS)
    {
        ADMIN_counter++;
    }
    fclose(file);
}
