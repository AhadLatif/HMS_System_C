#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "src/common/common.h"
#include "src/id_manager/id_manager.h"

#include "patient.h"

Patient patients[MAX_PATIENTS];
int patient_counter = 0;

void patientModule()
{

    loadPatientFromFile();
    loadIDManager();

    int choice;
    do
    {
        displayPatientMenu();
        choice = inputInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            addPatient();
            break;
        case 2:
            displayPatient();
            break;
        case 3:
        {

            int search_choice;
            printf("Search Patient by:\n1. ID\n2. Name\n3.<<<<<Go Back\n");
            search_choice = inputInt("Enter choice: ");
            while (search_choice != 1 && search_choice != 2 && search_choice != 3)
            {

                printf("Please choose correct option!");
                printf("1. ID\n2. Name\n3.<<<<<Go Back\n");
                search_choice = inputInt("Enter choice: ");
            }

            if (search_choice == 1)
                searchPatientById();
            else if (search_choice == 2)
                searchPatientByName();
            else if (search_choice == 3)
                continue;
            else
                printf("Invalid search choice!\n");
        }
        break;
        case 4:
        {

            int choice;

            printf("\nUpdate Patient by:\n1. ID\n2. Name\n3.<<<<<Go Back\n");
            choice = inputInt("Enter choice: ");
            while (choice != 1 && choice != 2 && choice != 3)
            {

                printf("Please choose correct option!");
                printf("1. ID\n2. Name\n3.<<<<<Go Back\n");
                choice = inputInt("Enter choice: ");
            }

            if (choice == 1)
                updatePatientById();
            else if (choice == 2)
                updatePatientByName();
            else if (choice == 3)
                continue;
            else
                printf("Invalid search choice!\n");
            break;
        }
        case 5:
            deletePatient();
            break;

        case 6:
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

// CORE FUNCTIONS

// ------------  Add Patient ----------------
void addPatient()
{
    char choice[10];
    int sub_choice;
    sub_choice = inputInt("Want to : \n1.Add patient \n2.t<<<<Go Back\n\nConfirm by Choice:");
    while (sub_choice != 1 && sub_choice != 2)
    {
        printf("Choose Correct Option: ");
    }

    if (sub_choice == 1)
    {
        do
        {
            if (patient_counter > MAX_PATIENTS)
            {
                printf("List is full\n");
                return;
            }

            Patient new_patient;
            new_patient.p_id = id_manager.next_patient_id;

            for (int i = 0; i < patient_counter; i++)
            {
                if (patients[i].p_id == new_patient.p_id && patients[i].status == PATIENT_ACTIVE)
                {
                    printf("ID %d already exists\n", new_patient.p_id);
                    return;
                }
            }

            printf("\nEnter name: ");
            inputString(new_patient.p_name, sizeof(new_patient.p_name));

            new_patient.p_age = inputInt("Enter your age: ");

            printf("Enter gender: ");
            inputString(new_patient.p_gender, sizeof(new_patient.p_gender));

            printf("Enter disease: ");
            inputString(new_patient.p_disease, sizeof(new_patient.p_disease));

            printf("Enter contact number: ");
            inputString(new_patient.p_contact_num, sizeof(new_patient.p_contact_num));
            new_patient.status = PATIENT_ACTIVE;
            patients[patient_counter++] = new_patient;
            savePatientsToFile();
            id_manager.next_patient_id++;
            saveIDManager();
            printf("Patient Entered Successfully!\n");

            printf("Do you want to add another patient (Y/N)? ");
            inputString(choice, sizeof(choice));

        } while (strcasecmp(choice, "n") != 0 && strcasecmp(choice, "no") != 0);
    }
    else if (sub_choice == 2)
    {
        return;
    }
}

//  ---------------------------------------Search Functions
void searchPatientById()
{
    int id;
    id = inputInt("Enter Id of patient : ");

    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender, patients[i].p_disease, patients[i].p_contact_num);
            printf("-----------------------------------------------------------------------------------\n");
            return;
        }
    }
    printf("Patient with ID %d not found.\n", id);
}

void searchPatientByName()
{

    char name[50];
    int found = 0;
    printf("Enter Name : ");

    inputString(name, sizeof(name));

    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact");
    for (int i = 0; i < patient_counter; i++)
    {
        if (strncasecmp(patients[i].p_name, name, strlen(name)) == 0 && patients[i].status == PATIENT_ACTIVE)
        {

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender, patients[i].p_disease, patients[i].p_contact_num);
            found = 1;
        }
    }
    printf("-----------------------------------------------------------------------------------\n");
    if (!found)
    {
        printf("Patient with name : %s not found.", name);
        /* code */
    }
    else
    {
        printf("You are all caught up");
    }
}

//-----------------------------------Delete Patient--------------------

void deletePatient()
{

    int id, found = 0;

    id = inputInt("Enter ID:");

    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {

            patients[i].status = PATIENT_DEACTIVE;
            printf("Patient with ID %d  deleted successfully.\n", id);
            found = 1;
            savePatientsToFile();
        }
    }
    if (!found)
    {

        printf("Patient with ID %d not found.\n", id);
    }
}

// Update Function

void updatePatientById()
{
    int found = 0;
    int id = inputInt("\nEnter Id of patient: ");

    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {
            found = 1;

            while (1)
            {
                printf("\nExisting Info of patient with id %d:\n\n", id);
                printf("1. Name           : %s\n", patients[i].p_name);
                printf("2. Age            : %d\n", patients[i].p_age);
                printf("3. Gender         : %s\n", patients[i].p_gender);
                printf("4. Disease        : %s\n", patients[i].p_disease);
                printf("5. Contact Number : %s\n", patients[i].p_contact_num);
                printf("6. <<<< Go Back\n\n");

                int choice = inputInt("Which field do you want to change (1-6) : ");
                while (choice < 1 || choice > 6)
                {
                    printf("Invalid choice! Please enter a valid choice between 1 and 6.\n");
                    choice = inputInt("Enter a choice: ");
                }
                if (choice == 6)
                    return;

                // Ask for confirmation before continuing with update
                char *fieldName[] = {"NAME", "AGE", "GENDER", "DISEASE", "CONTACT NUMBER"};
                printf("\nYou choose to update *%s*. Do you really want to:\n\n", fieldName[choice - 1]);
                printf("1. Proceed to update\n");
                printf("2. Go back to menu\n\n");
                int sub_choice = inputInt("Enter your choice: ");

                if (sub_choice == 2)
                {
                    continue; // Go back to field menu
                }

                // Update field
                switch (choice)
                {
                case 1:
                    printf("Enter new name: ");
                    inputString(patients[i].p_name, sizeof(patients[i].p_name));
                    break;
                case 2:
                    patients[i].p_age = inputInt("Enter new age: ");
                    break;
                case 3:
                    printf("Enter new gender: ");
                    inputString(patients[i].p_gender, sizeof(patients[i].p_gender));
                    break;
                case 4:
                    printf("Enter new disease: ");
                    inputString(patients[i].p_disease, sizeof(patients[i].p_disease));
                    break;
                case 5:
                    printf("Enter new contact number: ");
                    inputString(patients[i].p_contact_num, sizeof(patients[i].p_contact_num));
                    break;

                case 6:

                    // printf(" Field updated! You can change more fields or save & exit.\n");
                    return;
                    break;
                }
            }

            int sub_choice2;

            printf(" 1. Save and Exit\n");
            printf(" 2. Cancel without Saving\n\n");
            sub_choice2 = inputInt("Enter a Choice");
            while (sub_choice2 != 1 && sub_choice2 != 2)
            {
                printf("Choose correct Option\n\n");
                printf(" 1. Save and Exit\n");
                printf(" 2. Cancel without Saving\n\n");
            }
            if (sub_choice2 == 1)
            {
                savePatientsToFile();
                printf(" Patient record updated and saved successfully.\n");
                break;
            }
            else
            {
                printf("Update cancelled. No changes were saved.\n");
                break;
            }
        }
    }

    if (!found)
    {
        printf("\nNo active patient found with ID %d.\n", id);
    }
}

void updatePatientByName()
{
}

// SECONDARY FUNCTIONS

// --------------------------------------------Display Patients
void displayPatient()
{

    FILE *file = fopen("./data/patient.dat", "rb");

    fileCheck(file);

    printf("\n List of All Patients:\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE)
        {

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s |\n", patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender, patients[i].p_disease, patients[i].p_contact_num);
        }
    }

    printf("-----------------------------------------------------------------------------------\n");
    if (patient_counter == 0)
        printf("No patient records found.\n");
}

//----------------------------------savePatientsToFile

void savePatientsToFile()
{

    FILE *file = fopen("./data/patient.dat", "wb");
    fileCheck(file);

    for (int i = 0; i < patient_counter; i++)
    {
        fwrite(&patients[i], sizeof(Patient), 1, file);
    }

    fclose(file);
}

// ----------------------------------------Display Menu Function
void displayPatientMenu(void)
{
    printf("\n=== Hospital Management System ===\n");
    printf("1. Add a Patient\n");
    printf("2. Display All Patients\n");
    printf("3. Search for a Patient\n");
    printf("4. Update a Patient\n");
    printf("5. Delete a Patient\n");
    printf("6. Exit\n");
    printf("==============================\n");
}

//---------------------------------------------Load Patient
void loadPatientFromFile(void)

{

    FILE *fp = fopen("./data/patient.dat", "rb");

    fileCheck(fp);

    while (fread(&patients[patient_counter], sizeof(Patient), 1, fp) != 0)
    {
        patient_counter++;
    }

    fclose(fp);
}