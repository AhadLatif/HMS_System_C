#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "src/doctor/doctor.h"
#include "src/common/common.h"
#include "src/id_manager/id_manager.h"
#include "doctor.h"

// CORE
Doctor doctors[MAX_DOCTORS];
int doctor_counter = 0;

void doctorModule()
{

    loadDoctorsFromFile();
    loadIDManager();
    int choice;
    do
    {
        displayDoctorMenu();
        choice = inputInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            addDoctor();
            break;
        case 2:
            displayDoctors();
            break;
        case 3:
            searchDoctor();
            break;
        case 4:
            deleteDoctor();
            break;
        case 5:
            exitProgram();
            break;
            // return 0;
        default:
            printf("Invalid choice! Please enter a number between 1 and 5.\n");
        }

        // printf("\nPress Enter to continue...");
        // while (getchar() != '\n';

    } while (choice != 5);
}

void addDoctor()
{
    if (doctor_counter > MAX_DOCTORS)
    {
        printf("List is full\n");
        return;
    }
    char choice[10];

    do
    {

        Doctor new_doctor;

        new_doctor.d_id = id_manager.next_doctor_id;

        for (int i = 0; i < doctor_counter; i++)
        {
            if (doctors[i].d_id == new_doctor.d_id)
            {
                printf("doctor with id %d is already exists ", new_doctor.d_id);
            }
        }

        printf("Enter name: ");
        inputString(new_doctor.d_name, sizeof(new_doctor.d_name));

        new_doctor.d_age = inputInt("Enter your age: ");

        printf("Enter gender: ");
        inputString(new_doctor.d_gender, sizeof(new_doctor.d_gender));

        printf("Enter Specialization: ");
        inputString(new_doctor.d_specialization, sizeof(new_doctor.d_specialization));

        printf("Enter contact number: ");
        inputString(new_doctor.d_contact, sizeof(new_doctor.d_contact));

        new_doctor.status = ACTIVE;

        doctors[doctor_counter++] = new_doctor;

        saveDoctorsToFile();

        id_manager.next_doctor_id++;

        saveIDManager();
        printf("doctor Entered Successfully!\n");

        printf("Do you want to add another doctor (Y/N)? ");
        inputString(choice, sizeof(choice));

        /* code */
    } while (strcasecmp(choice, "n") != 0 && strcasecmp(choice, "no") != 0);
}

void searchDoctor()
{

    char choice[20];

    do
    {

        int search_choice;
        printf("Search Doctor by:\n1. ID\n2. Name\n3. Specialization\n");
        search_choice = inputInt("Enter choice: ");
        if (search_choice == 1)
            searchDoctorById();
        else if (search_choice == 2)
            searchDoctorByName();
        else if (search_choice == 3)
            searchDoctorBySpecialization();
        else
            printf("Invalid search choice!\n");

        printf("Do you want to add another doctor (Y/N)? ");
        inputString(choice, sizeof(choice));
    } while (strcasecmp(choice, "n") != 0 && strcasecmp(choice, "n") != 0);
}

void searchDoctorById()
{

    int id;
    id = inputInt("Enter an Id : ");

    for (int i = 0; i < doctor_counter; i++)
    {
        if (doctors[i].d_id == id && doctors[i].status == ACTIVE)
        {
            printf("-----------------------------------------------------------------------------------\n");
            printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Specialization", "Contact");
            printf("-----------------------------------------------------------------------------------\n");

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", doctors[i].d_id, doctors[i].d_name, doctors[i].d_age, doctors[i].d_gender, doctors[i].d_specialization, doctors[i].d_contact);
            return;
        }
    }
    printf("doctor with ID %d not found.\n", id);
}

void searchDoctorByName()
{
    char d_name[50];
    printf("Enter Name : ");

    inputString(d_name, sizeof(d_name));
    int found = 0;

    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Specialization", "Contact");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < doctor_counter; i++)
    {
        if (strncasecmp(doctors[i].d_name, d_name, strlen(d_name)) == 0 && doctors[i].status == ACTIVE)
        {
            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", doctors[i].d_id, doctors[i].d_name, doctors[i].d_age, doctors[i].d_gender, doctors[i].d_specialization, doctors[i].d_contact);
            found = 1;
        }
    }

    if (!found)
    {
        printf("doctor with name %s not found.\n", d_name);
    }
    else
    {
        printf("You are all caught up!");
    }
}

void searchDoctorBySpecialization()
{
    char specialization[50];
    int found = 0;
    printf("Enter Specialization : ");

    inputString(specialization, sizeof(specialization));
    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Specialization", "Contact");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < doctor_counter; i++)
    {
        if (strncasecmp(doctors[i].d_specialization, specialization, strlen(specialization)) == 0 && doctors[i].status==ACTIVE)
        {
            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", doctors[i].d_id, doctors[i].d_name, doctors[i].d_age, doctors[i].d_gender, doctors[i].d_specialization, doctors[i].d_contact);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Doctor With Specialization %s Not Found!.\n", specialization);
    }
    else
    {

        printf("You are all caught up!");
    }
}

void deleteDoctor()
{

    int id, is_deleted = 0;
    id = inputInt("Enter an Id: ");

    for (int i = 0; i < doctor_counter; i++)
    {
        if (doctors[i].d_id == id && doctors[i].status == ACTIVE)
        {
            doctors[i].status = DEACTIVE;
            printf("Doctor with ID %d is deleted successfully.\n", id);
            is_deleted = 1;
            saveDoctorsToFile();
        }
    }
    if (!is_deleted)
    {
        printf("Patient with ID %d not found.\n", id);
    }
}

// File handling
void loadDoctorsFromFile()
{
    FILE *file = fopen("./data/doctor.dat", "rb");

    fileCheck(file);
    while (fread(&doctors[doctor_counter], sizeof(Doctor), 1, file) != 0)
    {
        doctor_counter++;
    }
    fclose(file);
}

void saveDoctorsToFile()
{
    FILE *file = fopen("./data/doctor.dat", "wb");

    for (int i = 0; i < doctor_counter; i++)
    {
        fwrite(&doctors[i], sizeof(Doctor), 1, file);
    }

    fclose(file);
}
// Others

void displayDoctorMenu()
{

    printf("\n=== Doctor Module ===\n");
    printf("1. Add a Doctor\n");
    printf("2. Display All Doctors\n");
    printf("3. Search for a Doctor\n");
    printf("4. Delete a Doctor\n");
    printf("5. Exit\n");
    printf("==============================\n");
}

void displayDoctors()
{
    FILE *file = fopen("./data/doctor.dat", "rb");

    fileCheck(file);

    printf("\n List of All doctors:\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Specialization", "Contact");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < doctor_counter; i++)
    {
        if (doctors[i].status == ACTIVE)
        {

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", doctors[i].d_id, doctors[i].d_name, doctors[i].d_age, doctors[i].d_gender, doctors[i].d_specialization, doctors[i].d_contact);
        }
    }

    printf("-----------------------------------------------------------------------------------\n");
    if (doctor_counter == 0)
        printf("No doctor records found.\n");
}