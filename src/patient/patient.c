
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "src/common/common.h"
#include "src/id_manager/id_manager.h"
#include "src/VisitLog/visitlog.h"

#include "patient.h"
#include <time.h>

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
    } while (choice != 6);
}

// CORE FUNCTIONS

// ------------  Add Patient ----------------
void addPatient()
{
    char choice[10];
    int sub_choice;
    do
    {
        sub_choice = inputInt("Want to : \n1.Add patient \n2.<<<<Go Back\n\nConfirm by Choice:");
        if (sub_choice != 1 && sub_choice != 2)
        {
            printf("Choose Correct Option: ");
        }
    } while (sub_choice != 1 && sub_choice != 2);

    if (sub_choice == 1)
    {
        do
        {
            if (patient_counter >= MAX_PATIENTS)
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

            new_patient.registration_time = time(NULL);

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

    int confirm = inputInt("1. Search By ID\n2. <<<<<Go back\nEnter your choice: ");
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }
    int found = 0;
    int i;
    int id;
    id = inputInt("Enter Id of patient : ");

    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact", "Registration D&T");
    for (i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {

            char reg_time_str[25];
            struct tm *tm_info = localtime(&patients[i].registration_time);
            strftime(reg_time_str, sizeof(reg_time_str), "%H:%M:%S %d/%m/%Y", tm_info);

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s |\n",
                   patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, reg_time_str);

            found = 1;
            printf("-------------------------------------------------------------------------------------------------------\n");

            break;
        }
    }
    if (!found)
    {
        printf("Patient with ID %d not found.\n", id);
    }
    else
    {
        {
            int action_choice;
            do
            {
                printf("\nWhat do you want to do next for patient %s (ID: %d)?\n", patients[i].p_name, patients[i].p_id);
                printf("1. Add new visit\n");
                printf("2. Display visit history\n");
                printf("3. Go back to search menu\n");
                action_choice = inputInt("Enter your choice: ");
                switch (action_choice)
                {
                case 1:
                {
                    char option[10];
                    do
                    {
                        addVisitLog(patients[i].p_id);
                        printf("Visit Log Added Successfully!\n");
                        printf("Do you want to add another visit for %s (ID: %d)? (Y/N): ",
                               patients[i].p_name, patients[i].p_id);
                        inputString(option, sizeof(option));
                        while (!(strcasecmp(option, "y") == 0 || strcasecmp(option, "yes") == 0 ||
                                 strcasecmp(option, "n") == 0 || strcasecmp(option, "no") == 0))
                        {
                            printf("Please choose correct option (Y/N): ");
                            inputString(option, sizeof(option));
                        }
                    } while (strcasecmp(option, "y") == 0 || strcasecmp(option, "yes") == 0);
                    break;
                }
                case 2:
                    displayVisitHistory(patients[i].p_id);
                    break;
                case 3:
                    printf("Returning to search menu.\n");
                    break;
                default:
                    printf("Invalid choice! Please enter a valid option.\n");
                    continue;
                }
                break;
            } while (1);
        }
    }
}

void searchPatientByName()
{
    int confirm = inputInt("1. Search By Name\n2. Go back\nEnter your choice: ");
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }

    char name[50];
    printf("Enter Name: ");
    inputString(name, sizeof(name));

    int matches[100];
    int matchCount = 0;

    // Case-insensitive prefix match
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE &&
            strncasecmp(patients[i].p_name, name, strlen(name)) == 0)
        {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0)
    {
        printf("\nNo active patients found matching '%s'.\n", name);
        return;
    }

    int select_index = -1;

    while (1)
    {
        if (matchCount == 1)
        {
            select_index = matches[0];
            printf(" %-5s | %-5s | %-20s | %-3s | %-10s | %-15s | %-15s \n",
                   "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact");
            printf(" %-5d | %-5d | %-20s | %-3d | %-10s | %-15s | %-15s \n",
                   1, patients[select_index].p_id, patients[select_index].p_name, patients[select_index].p_age,
                   patients[select_index].p_gender, patients[select_index].p_disease, patients[select_index].p_contact_num);
            printf("---------------------------------------------------------------------------------------------\n");
        }
        else if (matchCount > 1)

        {
            printf("\nMultiple patients found with matching name:\n");
            printf("---------------------------------------------------------------------------------------------\n");
            printf(" %-5s | %-5s | %-20s | %-3s | %-10s | %-15s | %-15s \n",
                   "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact");

            for (int i = 0; i < matchCount; i++)
            {
                int idx = matches[i];
                printf(" %-5d | %-5d | %-20s | %-3d | %-10s | %-15s | %-15s \n",
                       i + 1, patients[idx].p_id, patients[idx].p_name, patients[idx].p_age,
                       patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num);
            }
            printf("---------------------------------------------------------------------------------------------\n");

            int choice;
            do
            {
                printf("Select the number of the patient you want to (1-%d): \n", matchCount);
                choice = inputInt("");
            } while (choice < 1 || choice > matchCount);

            select_index = matches[choice - 1];
        }

        // Action Menu Loop
        
        int action_choice;
        do
        {
            printf("\nWhat do you want to do next for patient %s (ID: %d)?\n",
                   patients[select_index].p_name, patients[select_index].p_id);
            printf("1. Add new visit\n");
            printf("2. Display visit history\n");
            printf("3. Go back to patient selection\n");
            printf("4. Go back to search menu\n");

            action_choice = inputInt("Enter your choice: ");

            switch (action_choice)
            {
            case 1:
            {
                char option[10];
                do
                {
                    addVisitLog(patients[select_index].p_id);
                    printf("Do you want to add another visit for %s (ID: %d)? (Y/N): ",
                           patients[select_index].p_name, patients[select_index].p_id);
                    inputString(option, sizeof(option));
                    while (!(strcasecmp(option, "y") == 0 || strcasecmp(option, "yes") == 0 ||
                             strcasecmp(option, "n") == 0 || strcasecmp(option, "no") == 0))
                    {
                        printf("Please choose correct option (Y/N): ");
                        inputString(option, sizeof(option));
                    }
                } while (strcasecmp(option, "y") == 0 || strcasecmp(option, "yes") == 0);
                break;
            }
            case 2:
                displayVisitHistory(patients[select_index].p_id);
                break;
            case 3:
                // Go back to patient selection
                break;
            case 4:
                // Go back to search menu
                return;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
            }
        } while (action_choice != 3 && action_choice != 4);

        if (action_choice == 4)
        {
            // Go back to search menu
            return;
        }
        // If action_choice == 3, loop back to patient selection
        if (matchCount == 1)
        {
            // If only one match after action
            // return to search menu
            return;
        }
    }
}

//-----------------------------------Delete Patient--------------------

void deletePatient()
{
    printf("\n=== Delete Patient ===\n");
    int confirm;
    do
    {
        confirm = inputInt("1. Proceed to delete\n2. Go back\nEnter your choice: ");
        if (confirm != 1 && confirm != 2)
        {
            printf("Choose Correct Option: \n");
        }
    } while (confirm != 1 && confirm != 2);
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }

    int id, found = 0;
    id = inputInt("Enter ID: ");

    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {

            printf("\n1. Really want to delete \n\nID: %d \nName : %s\n\n", patients[i].p_id, patients[i].p_name);
            int confirm2 = inputInt("2. Go back\nEnter your choice: ");
            if (confirm2 != 1)
            {
                printf("Returning to main menu.\n");
                return;
            }
            patients[i].status = PATIENT_DEACTIVE;
            printf("Patient with ID %d deleted successfully.\n", id);
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

    int confirm = inputInt("1.Really Want to Update By ID\n2. Go back\nEnter your choice: ");
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }

    int id = inputInt("\nEnter ID of patient: ");
    int found = 0;

    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].p_id == id && patients[i].status == PATIENT_ACTIVE)
        {
            found = 1;
            int updated = 0;

            while (1)
            {
                printf("\n--- Existing Info of patient with ID %d ---\n", id);
                printf("1. Name           : %s\n", patients[i].p_name);
                printf("2. Age            : %d\n", patients[i].p_age);
                printf("3. Gender         : %s\n", patients[i].p_gender);
                printf("4. Disease        : %s\n", patients[i].p_disease);
                printf("5. Contact Number : %s\n", patients[i].p_contact_num);
                printf("6. << Go Back (Finish Updating)\n\n");

                int choice = inputInt("Which field do you want to change (1-6): ");
                while (choice < 1 || choice > 6)
                {
                    printf("Invalid choice! Please enter a valid choice between 1 and 6.\n");
                    choice = inputInt("Enter a choice: ");
                }

                if (choice == 6)
                {
                    break; // Exit the loop to final save/cancel decision
                }

                const char *fieldName[] = {"Name", "Age", "Gender", "Disease", "Contact Number"};
                printf("\nYou chose to update *%s*.\n", fieldName[choice - 1]);
                printf("1. Proceed to update\n");
                printf("2. Go back to menu\n");
                int sub_choice = inputInt("Enter your choice: ");

                if (sub_choice != 1)
                {
                    continue; // Go back to field menu
                }

                // Update selected field
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
                }

                updated = 1;

                printf("\nDo you want to update another field?\n");
                printf("1. Yes\n");
                printf("2. No (Continue to Save)\n");
                int more = inputInt("Enter your choice: ");
                if (more != 1)
                {
                    break;
                }
            }

            if (updated)
            {
                printf("\nDo you want to save the changes?\n");
                printf("1. Save and Exit\n");
                printf("2. Cancel without Saving\n\n");
                int sub_choice2 = inputInt("Enter your choice: ");

                while (sub_choice2 != 1 && sub_choice2 != 2)
                {
                    printf("Invalid option. Please try again.\n");
                    sub_choice2 = inputInt("Enter your choice: ");
                }

                if (sub_choice2 == 1)
                {
                    savePatientsToFile();
                    printf("\n Patient record updated and saved successfully.\n");
                }
                else
                {
                    printf("\n Update cancelled. No changes were saved.\n");
                }
            }
            else
            {
                printf("\n No changes were made to the patient record.\n");
            }

            break; // Update only the first matching record
        }
    }

    if (!found)
    {
        printf("\n No active patient found with ID %d.\n", id);
    }
}

void updatePatientByName()
{

    int confirm = inputInt("1. Really Want to Update By Name\n2. Go back\nEnter your choice: ");
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }

    char name[50];
    printf("\nEnter name of patient: ");
    inputString(name, sizeof(name));

    int matches[100]; // To store indexes of matching patients
    int matchCount = 0;

    // Collect all matching patients (case-insensitive substring match)
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE && strncasecmp(patients[i].p_name, name, strlen(name)) == 0)
        {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0)
    {
        printf("\n No active patients found matching '%s'.\n", name);
        return;
    }

    int select_index = -1;

    if (matchCount == 1)
    {
        select_index = matches[0]; // Only one match
    }
    else
    {
        printf("\n Multiple patients found with matching name:\n\n");
        printf("---------------------------------------------------------------------------------------------\n");
        printf(" %-5s | %-5s |%-20s | %-3s | %-10s | %-15s | %-15s |\n\n", "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact");

        for (int i = 0; i < matchCount; i++)
        {
            int idx = matches[i];
            printf(" %-5d | %-5d |%-20s | %-3d | %-10s | %-15s | %-15s |\n",
                   i + 1, patients[idx].p_id, patients[idx].p_name, patients[idx].p_age,
                   patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num);
        }
        printf("---------------------------------------------------------------------------------------------\n");

        printf("\nSelect the number of the patient you want to update (1-%d): ", matchCount);
        int choice = inputInt("");

        while (choice < 1 || choice > matchCount)
        {
            printf(" Invalid choice. Enter a number between 1 and %d: ", matchCount);
            choice = inputInt("");
        }

        select_index = matches[choice - 1];
    }

    int updated = 0;
    while (1)
    {
        printf("\n--- Existing Info of patient ---\n");
        printf("1. Name           : %s\n", patients[select_index].p_name);
        printf("2. Age            : %d\n", patients[select_index].p_age);
        printf("3. Gender         : %s\n", patients[select_index].p_gender);
        printf("4. Disease        : %s\n", patients[select_index].p_disease);
        printf("5. Contact Number : %s\n", patients[select_index].p_contact_num);
        printf("6. << Go Back (Finish Updating)\n\n");

        int choice = inputInt("Which field do you want to change (1-6): ");
        while (choice < 1 || choice > 6)
        {
            printf("Invalid choice! Please enter a valid choice between 1 and 6.\n");
            choice = inputInt("Enter a choice: ");
        }

        if (choice == 6)
            break;

        const char *fieldName[] = {"Name", "Age", "Gender", "Disease", "Contact Number"};
        printf("\nYou chose to update *%s*.\n", fieldName[choice - 1]);
        printf("1. Proceed to update\n");
        printf("2. Go back to menu\n");
        int sub_choice = inputInt("Enter your choice: ");

        if (sub_choice != 1)
            continue;

        switch (choice)
        {
        case 1:
            printf("Enter new name: ");
            inputString(patients[select_index].p_name, sizeof(patients[select_index].p_name));
            break;
        case 2:
            patients[select_index].p_age = inputInt("Enter new age: ");
            break;
        case 3:
            printf("Enter new gender: ");
            inputString(patients[select_index].p_gender, sizeof(patients[select_index].p_gender));
            break;
        case 4:
            printf("Enter new disease: ");
            inputString(patients[select_index].p_disease, sizeof(patients[select_index].p_disease));
            break;
        case 5:
            printf("Enter new contact number: ");
            inputString(patients[select_index].p_contact_num, sizeof(patients[select_index].p_contact_num));
            break;
        }

        updated = 1;

        printf("\nDo you want to update another field?\n");
        printf("1. Yes\n");
        printf("2. No (Proceed to save)\n");
        int more = inputInt("Enter your choice: ");
        if (more != 1)
            break;
    }

    if (updated)
    {
        printf("\nDo you want to save the changes?\n");
        printf("1. Save and Exit\n");
        printf("2. Cancel without Saving\n\n");
        int sub_choice2 = inputInt("Enter your choice: ");

        while (sub_choice2 != 1 && sub_choice2 != 2)
        {
            printf("Invalid option. Please try again.\n");
            sub_choice2 = inputInt("Enter your choice: ");
        }

        if (sub_choice2 == 1)
        {
            savePatientsToFile();
            printf("\n Patient record updated and saved successfully.\n");
        }
        else
        {
            printf("\n Update cancelled. No changes were saved.\n");
        }
    }
    else
    {
        printf("\n No changes were made to the patient record.\n");
    }
}

// SECONDARY FUNCTIONS

// --------------------------------------------Display Patients
void displayActivePatient()
{

    FILE *file = fopen("./data/patient.dat", "rb");

    fileCheck(file);

    printf("\n List of All Patients:\n");
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |%-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact", "Registration D&T");
    printf("-------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE)
        {
            time_t currentTime = time(NULL);
            char reg_time_str[25];
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));
            
        printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s |\n",
                   patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, reg_time_str);
        }
    }

    printf("-------------------------------------------------------------------------------------------------------\n");
    if (patient_counter == 0)
        printf("No patient records found.\n");
}


void displayDeactivePatient(){

    FILE*file = fopen("./data/patient.dat", "rb");
    fileCheck(file);

    printf("\n List of Deactive Patients:\n");
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |%-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact", "Registration D&T");
    printf("-------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_DEACTIVE)
        {
            char reg_time_str[25];
            time_t currentTime = time(NULL);
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));

            printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s |\n",
                   patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, reg_time_str);
        }
    }

    printf("-------------------------------------------------------------------------------------------------------\n");
    if (patient_counter == 0)
        printf("No patient records found.\n");
}


void displayAllPatient(){
    FILE *file = fopen("./data/patient.dat", "rb");
    fileCheck(file);

    printf("\n List of All Patients:\n");
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s |%-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact", "Registration D&T");
    printf("-------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        char reg_time_str[25];
        time_t current_time = time(NULL);
        formatRegistrationTime(current_time, reg_time_str, sizeof(reg_time_str));

        printf("| %-5d | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s |\n",
               patients[i].p_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
               patients[i].p_disease, patients[i].p_contact_num, reg_time_str);
    }

    printf("-------------------------------------------------------------------------------------------------------\n");
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
void displayPatient()
{
    int choice;
    do
    {
        printf("\n=== Display Patients ===\n");
        printf("1. Display Active Patients\n");
        printf("2. Display Deactive Patients\n");
        printf("3. Display All Patients\n");
        printf("4. Go Back\n");
        choice = inputInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            displayActivePatient();
            break;
        case 2:
            displayDeactivePatient();
            break;
        case 3:
            displayAllPatient();
            break;
        case 4:
            return; // Go back to the main menu
        default:
            printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
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
// >>>>>>> Patient-Module-18-05-2025
