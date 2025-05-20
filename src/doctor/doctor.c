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
        {
            int search_choice;
            printf("Search Doctor by:\n1. ID\n2. Name\n3. Specialization");
            search_choice = inputInt("Enter choice: ");
            if (search_choice == 1)
                searchDoctorById();
            else if (search_choice == 2)
                searchDoctorByName();
            else if (search_choice == 3)
                searchDoctorBySpecialization();
            else
                printf("Invalid search choice!\n");
        }
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

        id_manager.next_patient_id++;

        saveIDManager();
        printf("Patient Entered Successfully!\n");

        printf("Do you want to add another patient (Y/N)? ");
        inputString(choice, sizeof(choice));

        /* code */
    } while (strcasecmp(choice, "n") != 0 && strcasecmp(choice, "no") != 0);
}

void searchDoctorById()
{
}

void searchDoctorByName()
{
}

void searchDoctorBySpecialization()
{
}

void deleteDoctor()
{
}

// File handling
void loadDoctorsFromFile()
{
}

void saveDoctorsToFile()
{
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
    
}