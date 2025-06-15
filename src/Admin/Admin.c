#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "../common/common.h"
#include "../id_manager/id_manager.h"
#include <time.h>
#include "Admin.h"

Admin admins[MAX_ADMINS];
int admin_counter = 0;

void adminModule()
{
    loadAdminsFromFile();
    // loadIDManagerFromFile();
    int option;
    printf("Select an option:\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    option = inputInt("Enter your choice:");

    if (option == 1)
    {
        addAdmin();
    }
    else if (option == 2)
    {
        searchAdminByCnic();
    }
    else if (option == 3)
    {
        exitProgram();
    }

    else
    {
        printf("Invalid choice. Please try again.\n");
    }
}

// Add ADMIN
// ...existing code...
void addAdmin()
{
    Admin new_admin;
    inputValidatedName(new_admin.a_name, sizeof(new_admin.a_name));

    int unique = 0;
    while (!unique)
    {
        inputValidatedCNIC(new_admin.a_cnic, sizeof(new_admin.a_cnic));
        int found = 0;
        for (int i = 0; i < admin_counter; i++)
        {
            if (strcmp(admins[i].a_cnic, new_admin.a_cnic) == 0)
            {
                found = 1;
                printf("This CNIC is already assigned to admin: %s (ID: %s)\n", admins[i].a_name, admins[i].a_id);
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
                        if (strcmp(admins[i].password, password) == 0)
                        {
                            printf("Welcome back, %s!\n", admins[i].a_name);
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
    snprintf(new_admin.a_id, sizeof(new_admin.a_id), "ADMIN%03d", admin_counter + 1);
    admins[admin_counter] = new_admin;
    admin_counter++;
    saveAdminsToFile();
    printf("New admin added successfully with ID: %s\n", new_admin.a_id);

    // Return to login menu
    adminModule();
}


void searchAdminByCnic()
{
    char check_cnic[16];
    
    inputValidatedCNIC(check_cnic, sizeof(check_cnic));
    for (int i = 0; i < admin_counter; i++)
    {
        if (admins[i].status == ADMIN_ACTIVE && (strcmp(admins[i].a_cnic, check_cnic)) == 0)
        {
            printf("Admin found:\n");
            printf("ID: %s\nName :%s\n\n", admins[i].a_id,admins[i].a_name);
            printf("Enter your password: ");
            char password[20];
            inputString(password, sizeof(password));
            if (strcmp(admins[i].password, password) == 0)
            {
                printf("Login successful!\n\n");
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
    printf("No admin found with the provided CNIC.\n");
    adminModule();
}

// File Operations
void saveAdminsToFile()
{
    FILE *file = fopen("./data/Admin.dat", "wb");
    if (file == NULL)
    {
        printf("Failed to open file for saving admins");
        return;
    }
    for (int i = 0; i < admin_counter; i++)
    {
        if (fwrite(&admins[i], sizeof(Admin), 1, file) != 1)
        {
            printf("Error writing admin %d to file.\n", i + 1);
        }
    }
    fclose(file);

    printf("Admins saved to file successfully.\n");
}
void loadAdminsFromFile()
{
    FILE *file = fopen("./data/Admin.dat", "rb");
    if (!file)
    {
        printf("No existing admin data file found. Starting with empty database.\n");
        admin_counter = 0;
        return;
    }

    admin_counter = 0; // Reset counter before loading
    while (fread(&admins[admin_counter], sizeof(Admin), 1, file) != 0 && admin_counter < MAX_ADMINS)
    {
        admin_counter++;
    }
    fclose(file);
}
