#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "src/common/common.h"
#include "src/id_manager/id_manager.h"
#include "src/VisitLog/visitlog.h"

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
            printf("Search Patient by:\n1. ID\n2. Name\n3. CNIC\n4.<<<<<Go Back\n");
            search_choice = inputInt("Enter choice: ");
            while (search_choice != 1 && search_choice != 2 && search_choice != 3 && search_choice != 4)
            {

                printf("Please choose correct option!");
                printf("1. ID\n2. Name\n3. CNIC\n4.<<<<<Go Back\n");
                search_choice = inputInt("Enter choice: ");
            }

            if (search_choice == 1)
                searchPatientById();
            else if (search_choice == 2)
                searchPatientByName();
            else if (search_choice == 3)
                searchPatientByCnic();
            else if (search_choice == 4)
                continue;
            else
                printf("Invalid search choice!\n");
        }
        break;
        case 4:
        {

            int choice;

            printf("\nUpdate Patient by:\n1. ID\n2. Name\n3. CNIC\n4.<<<<<Go Back\n");
            choice = inputInt("Enter choice: ");
            while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
            {

                printf("Please choose correct option!");
                printf("1. ID\n2. Name\n3. CNIC\n4.<<<<<Go Back\n");
                choice = inputInt("Enter choice: ");
            }

            if (choice == 1)
                updatePatientById();
            else if (choice == 2)
                updatePatientByName();
            else if (choice == 3)
                updatePatientByCnic();
            else if (choice == 4)
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
            // Generate unique patient_id (e.g., "P00001")

            printf("Have you visit before?\n");

            char visit_choice[10];
            do
            {
                printf("1. Yes\n2. No\nEnter your choice: ");
                inputString(visit_choice, sizeof(visit_choice));

                if (strcasecmp(visit_choice, "1") == 0 || strcasecmp(visit_choice, "yes") == 0)
                {
                    // Existing patient
                    printf("Welcome back!\n");
                    printf("Enter Your CNIC: ");
                    char input_cnic[20]; // Temporary variable to store CNIC for lookup
                    inputValidatedCNIC(input_cnic, sizeof(input_cnic));

                    int found = 0;
                    for (int i = 0; i < patient_counter; i++)
                    {
                        if ((strcmp(patients[i].p_cnic, input_cnic) == 0 ||
                             (patients[i].is_minor && strcmp(patients[i].guardian_cnic, input_cnic) == 0)))
                        {
                            if (patients[i].status == PATIENT_ACTIVE)
                            {
                                printf("CNIC %s already exists for patient %s (PID: %s)\n",
                                       input_cnic, patients[i].p_name, patients[i].patient_id);
                                printf("Welcome back, %s!\n", patients[i].p_name);
                                printf("You can add your new visit details.\n");
                                addVisitLog(patients[i].patient_id);
                                found = 1;
                                break;
                            }
                            else if (patients[i].status == PATIENT_DEACTIVE)
                            {
                                printf("Found deactivated patient record for %s (PID: %s)\n",
                                       patients[i].p_name, patients[i].patient_id);
                                printf("Do you want to reactivate this record? (Y/N): ");
                                char reactivate[10];
                                inputString(reactivate, sizeof(reactivate));
                                if (strcasecmp(reactivate, "y") == 0 || strcasecmp(reactivate, "yes") == 0)
                                {
                                    patients[i].status = PATIENT_ACTIVE;
                                    savePatientsToFile();
                                    printf("Patient record reactivated successfully!\n");
                                    printf("Welcome back, %s!\n", patients[i].p_name);
                                    printf("You can add your new visit details.\n");
                                    addVisitLog(patients[i].patient_id);
                                    found = 1;
                                    break;
                                }
                                else
                                {
                                    printf("Record reactivation cancelled.\n");
                                    found = 1;
                                    break;
                                }
                            }
                        }
                    }

                    if (!found)
                    {
                        printf("No patient found with CNIC %s. Please register as a new patient.\n", input_cnic);
                    }
                    break;
                }
                else if (strcasecmp(visit_choice, "2") == 0 || strcasecmp(visit_choice, "no") == 0)
                {
                    snprintf(new_patient.patient_id, sizeof(new_patient.patient_id), "P%05d", patient_counter + 1);
                    // printf("\nEnter name: ");
                    inputValidatedName(new_patient.p_name, sizeof(new_patient.p_name));

                    new_patient.p_age = inputValidatedAge();

                    inputValidatedGender(new_patient.p_gender, sizeof(new_patient.p_gender));

                    // printf("Enter disease: ");
                    inputValidatedDisease(new_patient.p_disease, sizeof(new_patient.p_disease));

                    // Guardian CNIC logic for minors
                    if (new_patient.p_age < 18)
                    {
                        new_patient.is_minor = 1;

                        // Initialize both CNIC fields to empty strings first
                        new_patient.p_cnic[0] = '\0';        // Clear minor's CNIC
                        new_patient.guardian_cnic[0] = '\0'; // Clear guardian's CNIC

                        // Ask if minor has CNIC
                        char has_cnic[10];
                        do
                        {
                            printf("Does the minor have a CNIC? (Y/N): ");
                            inputString(has_cnic, sizeof(has_cnic));
                        } while (!(strcasecmp(has_cnic, "y") == 0 || strcasecmp(has_cnic, "yes") == 0 ||
                                   strcasecmp(has_cnic, "n") == 0 || strcasecmp(has_cnic, "no") == 0));

                        if (strcasecmp(has_cnic, "y") == 0 || strcasecmp(has_cnic, "yes") == 0)
                        {
                            // Minor has CNIC
                            printf("Enter minor's CNIC:\n");
                            inputValidatedCNIC(new_patient.p_cnic, sizeof(new_patient.p_cnic));
                            // Keep guardian CNIC empty
                        }
                        else
                        {
                            // Minor doesn't have CNIC
                            // Keep minor's CNIC empty
                            printf("Enter guardian CNIC:\n");
                            inputValidatedCNIC(new_patient.guardian_cnic, sizeof(new_patient.guardian_cnic));
                        }
                    }
                    else
                    {
                        new_patient.is_minor = 0;
                        new_patient.guardian_cnic[0] = '\0'; // Clear guardian CNIC
                        inputValidatedCNIC(new_patient.p_cnic, sizeof(new_patient.p_cnic));
                    }

                    // Deduplication: check for CNIC+Name+Age (±2)

                    for (int i = 0; i < patient_counter; i++)
                    {
                        if ((strcmp(patients[i].p_cnic, new_patient.p_cnic) == 0 ||
                             (new_patient.is_minor && strcmp(patients[i].guardian_cnic, new_patient.guardian_cnic) == 0)) &&
                            strcasecmp(patients[i].p_name, new_patient.p_name) == 0 &&
                            abs(patients[i].p_age - new_patient.p_age) <= 2)
                        {
                            printf("This CNIC, name, and age combination already exists for patient %s (PID: %s). Is this the same person? (Y/N): ", patients[i].p_name, patients[i].patient_id);
                            char confirm[10];
                            inputString(confirm, sizeof(confirm));
                            if (strcasecmp(confirm, "y") == 0 || strcasecmp(confirm, "yes") == 0)
                            {
                                printf("Duplicate entry not added.\n");

                                printf("patient status: %s\n", patients[i].status == PATIENT_ACTIVE ? "Active" : "Inactive");
                                printf("Do you want to update the status (Y/N)? ");
                                char update_confirm[10];
                                inputString(update_confirm, sizeof(update_confirm));
                                if (strcasecmp(update_confirm, "y") == 0 || strcasecmp(update_confirm, "yes") == 0)
                                {
                                    patients[i].status = (patients[i].status == PATIENT_ACTIVE) ? PATIENT_DEACTIVE : PATIENT_ACTIVE;
                                    printf("Patient status updated to: %s\n", patients[i].status == PATIENT_ACTIVE ? "Active" : "Inactive");
                                }

                                break;
                            }
                        }
                    }

                    printf("Enter blood group: ");
                    inputValidatedBloodGroup(new_patient.p_blood_group, sizeof(new_patient.p_blood_group));

                    inputValidatedContact(new_patient.p_contact_num, sizeof(new_patient.p_contact_num));

                    new_patient.registration_time = time(NULL);

                    new_patient.status = PATIENT_ACTIVE;

                    patients[patient_counter++] = new_patient;
                    savePatientsToFile();

                    id_manager.next_patient_id++;

                    saveIDManager();
                    printf("Patient Entered Successfully!\n");
                    printf("Welcome!\n");
                    break;
                }
                else
                {
                    printf("Invalid choice. Please try again.\n");
                }
            } while (1);

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
    char id[20];
    printf("Enter Id of patient : ");
    inputString(id, sizeof(id));

    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s |\n", "ID", "Name", "Age", "Gender", "Disease", "Contact", "Registration D&T");
    for (i = 0; i < patient_counter; i++)
    {
        if (strcmp(patients[i].patient_id, id) == 0 && patients[i].status == PATIENT_ACTIVE)
        {

            char reg_time_str[25];
            time_t currentTime = time(NULL);
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));

            printf("| %-5s | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s |\n",
                   patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, reg_time_str);

            found = 1;
            printf("-------------------------------------------------------------------------------------------------------\n");

            break;
        }
    }
    if (!found)
    {
        printf("Patient with ID %s not found.\n", id);
    }
    else
    {
        {
            int action_choice;
            do
            {
                printf("\nWhat do you want to do next for patient %s (ID: %s)?\n", patients[i].p_name, patients[i].patient_id);
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
                        addVisitLog(patients[i].patient_id);
                        printf("Visit Log Added Successfully!\n");
                        printf("Do you want to add another visit for %s (ID: %s)? (Y/N): ",
                               patients[i].p_name, patients[i].patient_id);
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
                    displayVisitHistory(patients[i].patient_id);
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
            printf(" %-5d | %-5s | %-20s | %-3d | %-10s | %-15s | %-15s \n",
                   1, patients[select_index].patient_id, patients[select_index].p_name, patients[select_index].p_age,
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
                printf(" %-5d | %-5s | %-20s | %-3d | %-10s | %-15s | %-15s \n",
                       i + 1, patients[idx].patient_id, patients[idx].p_name, patients[idx].p_age,
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
            printf("\nWhat do you want to do next for patient %s (ID: %s)?\n",
                   patients[select_index].p_name, patients[select_index].patient_id);
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
                    addVisitLog(patients[select_index].patient_id);
                    printf("Do you want to add another visit for %s (ID: %s)? (Y/N): ",
                           patients[select_index].p_name, patients[select_index].patient_id);
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
                displayVisitHistory(patients[select_index].patient_id);
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

void searchPatientByCnic()
{
    int confirm = inputInt("1. Search By CNIC\n2. Go back\nEnter your choice: ");
    while (confirm != 1 && confirm != 2)
    {
        printf("Wrong Choice\n");
        confirm = inputInt("1. Search By CNIC\n2. Go back\nEnter your choice: ");
    }

    char cnic[20];
    printf("Enter CNIC: ");
    inputString(cnic, sizeof(cnic));

    int matches[100];
    int matchCount = 0;

    // Match CNIC or guardian_cnic
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE &&
            (strncmp(patients[i].p_cnic, cnic, strlen(cnic)) == 0 ||
             (patients[i].is_minor && strncmp(patients[i].guardian_cnic, cnic, strlen(cnic)) == 0)))
        {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0)
    {
        printf("\nNo active patients found matching CNIC '%s'.\n", cnic);
        return;
    }

    int select_index = -1;
    while (1)
    {
        if (matchCount == 1)
        {
            select_index = matches[0];
            printf(" %-5s | %-8s | %-20s | %-3s | %-10s | %-15s | %-15s | %-15s | %-6s \n",
                   "No.", "PID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC", "Minor");
            printf(" %-5d | %-8s | %-20s | %-3d | %-10s | %-15s | %-15s | %-15s | %-6s \n",
                   1, patients[select_index].patient_id, patients[select_index].p_name, patients[select_index].p_age,
                   patients[select_index].p_gender, patients[select_index].p_disease, patients[select_index].p_contact_num, patients[select_index].p_cnic,
                   patients[select_index].is_minor ? "Minor" : "Adult");
            printf("---------------------------------------------------------------------------------------------\n");
        }
        else if (matchCount > 1)
        {
            printf("\nMultiple patients found with matching CNIC or Guardian CNIC:\n");
            printf("---------------------------------------------------------------------------------------------\n");
            printf(" %-5s | %-8s | %-20s | %-3s | %-10s | %-15s | %-15s | %-15s | %-6s \n",
                   "No.", "PID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC", "Minor");
            for (int i = 0; i < matchCount; i++)
            {
                int idx = matches[i];
                printf(" %-5d | %-8s | %-20s | %-3d | %-10s | %-15s | %-15s | %-15s | %-6s \n",
                       i + 1, patients[idx].patient_id, patients[idx].p_name, patients[idx].p_age,
                       patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num, patients[idx].p_cnic,
                       patients[idx].is_minor ? "Minor" : "Adult");
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
            printf("\nWhat do you want to do next for patient %s (PID: %s)?\n",
                   patients[select_index].p_name, patients[select_index].patient_id);
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
                    addVisitLog(patients[select_index].patient_id);
                    printf("Do you want to add another visit for %s (PID: %s)? (Y/N): ",
                           patients[select_index].p_name, patients[select_index].patient_id);
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
                displayVisitHistory(patients[select_index].patient_id);
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
    int choice;
    do
    {
        printf("1. Delete by ID\n2. Delete by CNIC\n3. Go back\nEnter your choice: ");
        choice = inputInt("");
        if (choice != 1 && choice != 2 && choice != 3)
        {
            printf("Choose Correct Option: \n");
        }
    } while (choice != 1 && choice != 2 && choice != 3);
    if (choice == 1)
    {
        // Existing delete by ID logic
        char id[20];
        printf("Enter ID: ");
        inputString(id, sizeof(id));
        int found = 0;
        for (int i = 0; i < patient_counter; i++)
        {
            if (strcmp(patients[i].patient_id, id) == 0 && patients[i].status == PATIENT_ACTIVE)
            {
                printf("\n1. Really want to delete \n\nID: %s \nName : %s\n\n", patients[i].patient_id, patients[i].p_name);
                int confirm2 = inputInt("2. Go back\nEnter your choice: ");
                if (confirm2 != 1)
                {
                    printf("Returning to main menu.\n");
                    return;
                }
                patients[i].status = PATIENT_DEACTIVE;
                printf("Patient with ID %s deleted successfully.\n", id);
                found = 1;
                savePatientsToFile();
            }
        }
        if (!found)
        {
            printf("Patient with ID %s not found.\n", id);
        }
    }
    else if (choice == 2)
    {
        deletePatientByCnic();
    }
    else if (choice == 3)
    {
        printf("Returning to main menu.\n");
        return;
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

    char id[20];
    printf("\nEnter ID of patient: ");
    inputString(id, sizeof(id));
    int found = 0;

    for (int i = 0; i < patient_counter; i++)
    {
        if (strcmp(patients[i].patient_id, id) == 0 && patients[i].status == PATIENT_ACTIVE)
        {
            found = 1;
            int updated = 0;

            while (1)
            {
                printf("\n--- Existing Info of patient with ID %s ---\n", id);
                printf("1. Name           : %s\n", patients[i].p_name);
                printf("2. Age            : %d\n", patients[i].p_age);
                printf("3. Gender         : %s\n", patients[i].p_gender);
                printf("4. Disease        : %s\n", patients[i].p_disease);
                printf("5. Contact Number : %s\n", patients[i].p_contact_num);
                printf("6. CNIC : %s\n", patients[i].p_cnic);
                printf("7. << Go Back (Finish Updating)\n\n");

                int choice = inputInt("Which field do you want to change (1-7): ");
                while (choice < 1 || choice > 7)
                {
                    printf("Invalid choice! Please enter a valid choice between 1 and 7.\n");
                    choice = inputInt("Enter a choice: ");
                }

                if (choice == 7)
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
                    inputValidatedName(patients[i].p_name, sizeof(patients[i].p_name));
                    break;
                case 2:
                    patients[i].p_age = inputValidatedAge();
                    break;
                case 3:
                    inputValidatedGender(patients[i].p_gender, sizeof(patients[i].p_gender));
                    break;
                case 4:
                    inputValidatedDisease(patients[i].p_disease, sizeof(patients[i].p_disease));
                    break;
                case 5:
                    inputValidatedContact(patients[i].p_contact_num, sizeof(patients[i].p_contact_num));
                    break;
                case 6:
                    inputValidatedCNIC(patients[i].p_cnic, sizeof(patients[i].p_cnic));
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
        printf("\n No active patient found with ID %s.\n", id);
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
        printf(" %-5s | %-5s |%-20s | %-3s | %-10s | %-15s | %-15s |%-15s |\n\n", "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC");

        for (int i = 0; i < matchCount; i++)
        {
            int idx = matches[i];
            printf(" %-5d | %-5s |%-20s | %-3d | %-10s | %-15s | %-15s | %-15s |\n",
                   i + 1, patients[idx].patient_id, patients[idx].p_name, patients[idx].p_age,
                   patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num, patients[i].p_cnic);
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
        printf("6. CNIC : %s\n", patients[select_index].p_cnic);
        printf("7. << Go Back (Finish Updating)\n\n");

        int choice = inputInt("Which field do you want to change (1-7): ");
        while (choice < 1 || choice > 7)
        {
            printf("Invalid choice! Please enter a valid choice between 1 and 6.\n");
            choice = inputInt("Enter a choice: ");
        }

        if (choice == 7)
            break;

        const char *fieldName[] = {"Name", "Age", "Gender", "Disease", "Contact Number", "CNIC"};
        printf("\nYou chose to update *%s*.\n", fieldName[choice - 1]);
        printf("1. Proceed to update\n");
        printf("2. Go back to menu\n");
        int sub_choice = inputInt("Enter your choice: ");

        if (sub_choice != 1)
            continue;

        switch (choice)
        {
        case 1:
            inputValidatedName(patients[select_index].p_name, sizeof(patients[select_index].p_name));
            break;
        case 2:
            patients[select_index].p_age = inputValidatedAge();
            break;
        case 3:
            inputValidatedGender(patients[select_index].p_gender, sizeof(patients[select_index].p_gender));
            break;
        case 4:
            inputValidatedDisease(patients[select_index].p_disease, sizeof(patients[select_index].p_disease));
            break;
        case 5:
            inputValidatedContact(patients[select_index].p_contact_num, sizeof(patients[select_index].p_contact_num));
            break;
        case 6:
            inputValidatedContact(patients[select_index].p_cnic, sizeof(patients[select_index].p_cnic));
            break;
        }

        updated = 1;

        printf("\nDo you want to update another field?\n");
        printf("1. Yes\n");
        printf("2. No (Continue to Save)\n");
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

void updatePatientByCnic()
{
    int confirm = inputInt("1. Really Want to Update By CNIC\n2. Go back\nEnter your choice: ");
    if (confirm != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }
    char cnic[20];
    printf("\nEnter CNIC of patient: ");
    inputString(cnic, sizeof(cnic));
    int matches[100];
    int matchCount = 0;
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE && strncmp(patients[i].p_cnic, cnic, strlen(cnic)) == 0)
        {
            matches[matchCount++] = i;
        }
    }
    if (matchCount == 0)
    {
        printf("\n No active patients found matching CNIC '%s'.\n", cnic);
        return;
    }
    int select_index = -1;
    if (matchCount == 1)
    {
        select_index = matches[0];
    }
    else
    {
        printf("\n Multiple patients found with matching CNIC:\n\n");
        printf("---------------------------------------------------------------------------------------------\n");
        printf(" %-5s | %-5s |%-20s | %-3s | %-10s | %-15s | %-15s | %-15s |\n\n", "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC");
        for (int i = 0; i < matchCount; i++)
        {
            int idx = matches[i];
            printf(" %-5d | %-5s |%-20s | %-3d | %-10s | %-15s | %-15s | %-15s |\n",
                   i + 1, patients[idx].patient_id, patients[idx].p_name, patients[idx].p_age,
                   patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num, patients[idx].p_cnic);
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
        printf("6. CNIC : %s\n", patients[select_index].p_cnic);
        printf("7. << Go Back (Finish Updating)\n\n");
        int choice = inputInt("Which field do you want to change (1-7): ");
        while (choice < 1 || choice > 7)
        {
            printf("Invalid choice! Please enter a valid choice between 1 and 7.\n");
            choice = inputInt("Enter a choice: ");
        }
        if (choice == 7)
            break;
        const char *fieldName[] = {"Name", "Age", "Gender", "Disease", "Contact Number", "CNIC"};
        printf("\nYou chose to update *%s*.\n", fieldName[choice - 1]);
        printf("1. Proceed to update\n");
        printf("2. Go back to menu\n");
        int sub_choice = inputInt("Enter your choice: ");
        if (sub_choice != 1)
            continue;
        switch (choice)
        {
        case 1:
            inputValidatedName(patients[select_index].p_name, sizeof(patients[select_index].p_name));
            break;
        case 2:
            patients[select_index].p_age = inputValidatedAge();
            break;
        case 3:
            inputValidatedGender(patients[select_index].p_gender, sizeof(patients[select_index].p_gender));
            break;
        case 4:
            inputValidatedDisease(patients[select_index].p_disease, sizeof(patients[select_index].p_disease));
            break;
        case 5:
            inputValidatedContact(patients[select_index].p_contact_num, sizeof(patients[select_index].p_contact_num));
            break;
        case 6:
            inputValidatedContact(patients[select_index].p_cnic, sizeof(patients[select_index].p_cnic));
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

void deletePatientByCnic()
{
    printf("\n=== Delete Patient By CNIC ===\n");
    int confirm;
    do
    {
        confirm = inputInt("1. Proceed to delete by CNIC\n2. Go back\nEnter your choice: ");
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
    char cnic[20];
    printf("Enter CNIC: ");
    inputString(cnic, sizeof(cnic));
    int matches[100];
    int matchCount = 0;
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE && strncmp(patients[i].p_cnic, cnic, strlen(cnic)) == 0)
        {
            matches[matchCount++] = i;
        }
    }
    if (matchCount == 0)
    {
        printf("\n No active patients found matching CNIC '%s'.\n", cnic);
        return;
    }
    int select_index = -1;
    if (matchCount == 1)
    {
        select_index = matches[0];
    }
    else
    {
        printf("\n Multiple patients found with matching CNIC:\n\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
        printf(" %-5s | %-5s |%-20s | %-3s | %-10s | %-15s | %-15s | %-15s |\n\n", "No.", "ID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC");
        for (int i = 0; i < matchCount; i++)
        {
            int idx = matches[i];
            printf(" %-5d | %-5s |%-20s | %-3d | %-10s | %-15s | %-15s | %-15s |\n",
                   i + 1, patients[idx].patient_id, patients[idx].p_name, patients[idx].p_age,
                   patients[idx].p_gender, patients[idx].p_disease, patients[idx].p_contact_num, patients[idx].p_cnic);
        }
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
        printf("\nSelect the number of the patient you want to delete (1-%d): ", matchCount);
        int choice = inputInt("");
        while (choice < 1 || choice > matchCount)
        {
            printf(" Invalid choice. Enter a number between 1 and %d: ", matchCount);
            choice = inputInt("");
        }
        select_index = matches[choice - 1];
    }
    printf("\n1. Really want to delete \n\nID: %s \nName : %s\n\n", patients[select_index].patient_id, patients[select_index].p_name);
    int confirm2 = inputInt("2. Go back\nEnter your choice: ");
    if (confirm2 != 1)
    {
        printf("Returning to main menu.\n");
        return;
    }
    patients[select_index].status = PATIENT_DEACTIVE;
    printf("Patient with CNIC %s deleted successfully.\n", patients[select_index].p_cnic);
    savePatientsToFile();
}

// SECONDARY FUNCTIONS

// --------------------------------------------Display Patients
void displayActivePatient()
{
    FILE *file = fopen("./data/patient.dat", "rb");
    fileCheck(file);
    printf("\n List of All Patients:\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n", "PID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC", "Guardian CNIC", "Minor", "Registration D&T");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_ACTIVE)
        {
            time_t currentTime = time(NULL);
            char reg_time_str[25];
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));
            printf("| %-8s | %-20s | %-3d | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n",
                   patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, patients[i].p_cnic,
                   patients[i].is_minor ? patients[i].guardian_cnic : "-",
                   patients[i].is_minor ? "Minor" : "Adult", reg_time_str);
        }
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    if (patient_counter == 0)
        printf("No patient records found.\n");
}

void displayDeactivePatient()
{
    FILE *file = fopen("./data/patient.dat", "rb");
    fileCheck(file);
    printf("\n List of Deactive Patients:\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n", "PID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC", "Guardian CNIC", "Minor", "Registration D&T");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        if (patients[i].status == PATIENT_DEACTIVE)
        {
            char reg_time_str[25];
            time_t currentTime = time(NULL);
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));
            printf("| %-8s | %-20s | %-3d | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n",
                   patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                   patients[i].p_disease, patients[i].p_contact_num, patients[i].p_cnic,
                   patients[i].is_minor ? patients[i].guardian_cnic : "-",
                   patients[i].is_minor ? "Minor" : "Adult", reg_time_str);
        }
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    if (patient_counter == 0)
        printf("No patient records found.\n");
}
void displayAllPatient()
{
    FILE *file = fopen("./data/patient.dat", "rb");
    fileCheck(file);
    printf("\n List of All Patients:\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n", "PID", "Name", "Age", "Gender", "Disease", "Contact", "CNIC", "Guardian CNIC", "Minor", "Registration D&T");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_counter; i++)
    {
        char reg_time_str[25];
        time_t current_time = time(NULL);
        formatRegistrationTime(current_time, reg_time_str, sizeof(reg_time_str));
        printf("| %-8s | %-20s | %-3d | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n",
               patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
               patients[i].p_disease, patients[i].p_contact_num, patients[i].p_cnic,
               patients[i].is_minor ? patients[i].guardian_cnic : "-",
               patients[i].is_minor ? "Minor" : "Adult", reg_time_str);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
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

int inputValidatedDisease(char *disease, int size)
{
    char input[60];
    int valid = 0;
    while (!valid)
    {
        printf("Enter disease (alphabets and spaces only): ");
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
            printf("Invalid disease. Please use alphabets and spaces only.\n");
        }
    }
    strncpy(disease, input, size);
    return 1;
}