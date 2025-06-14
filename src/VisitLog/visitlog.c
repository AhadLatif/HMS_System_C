#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "src/common/common.h"
#include "src/id_manager/id_manager.h"

#include "patient.h"
#include <time.h>
#include "src/VisitLog/visitlog.h"

VisitLog visit_logs[MAX_VISIT_LOGS]; // Array to hold visit logs
int visit_log_counter = 0;

void displayVisitHistory(char *patient_id)
{
    loadVisitLogs(); // Load visit logs from file

    printf("Visit History for Patient ID: %s\n", patient_id);
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| Visit ID | Patient ID | Assigned Doctor | Reason for Visit | Visit Time |\n");
    printf("-------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < visit_log_counter; i++)
    {
        if (strcmp(visit_logs[i].patient_id, patient_id) == 0)
        {

            char reg_time_str[25];
            formatRegistrationTime(visit_logs[i].visit_time, reg_time_str, sizeof(reg_time_str));

            printf("| %-8d | %-11s | %-15s | %-17s | %-19s |\n",
                   visit_logs[i].visit_id, visit_logs[i].patient_id,
                   visit_logs[i].assigned_doctor, visit_logs[i].reason, reg_time_str);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No visit logs found for Patient ID: %s\n", patient_id);
    }

    printf("-------------------------------------------------------------------------------------------------------\n");
    return;
}

void addVisitLog(char *patient_id)
{
    if (visit_log_counter >= MAX_VISIT_LOGS)
    {
        printf("Visit log is full. Cannot add more logs.\n");
        return;
    }
    VisitLog new_log;
    strncpy(new_log.patient_id, patient_id, sizeof(new_log.patient_id) - 1);
    new_log.patient_id[sizeof(new_log.patient_id) - 1] = '\0'; // Ensure null termination
    new_log.visit_id = getNextVisitID();
    // Unique ID for each visit

    printf("Enter reason for visit: ");
    inputString(new_log.reason, sizeof(new_log.reason));

    printf("Enter assigned doctor: ");
    inputString(new_log.assigned_doctor, sizeof(new_log.assigned_doctor));

    new_log.visit_time = time(NULL); // Current time

    saveVisitLog(new_log);
    return;
}

void loadVisitLogs()
{
    visit_log_counter = 0;
    FILE *fp = fopen("./data/visitlog.dat", "rb");
    if (!fp)
    {
        printf("[ERROR] Could not open visitlog.dat for reading. Please ensure the file exists and is accessible.\n");
        return;
    }
    while (fread(&visit_logs[visit_log_counter], sizeof(VisitLog), 1, fp) != 0)
    {
        visit_log_counter++;
    }
    fclose(fp);
}

void saveVisitLog(VisitLog log)
{
    if (visit_log_counter >= MAX_VISIT_LOGS)
    {
        printf("Visit log is full. Cannot add more logs.\n");
        return;
    }
    FILE *file = fopen("./data/visitlog.dat", "ab");
    if (!file)
    {
        printf("[ERROR] Could not open visitlog.dat for writing. Please ensure the file is accessible.\n");
        return;
    }
    fwrite(&log, sizeof(VisitLog), 1, file);
    fclose(file);
    visit_logs[visit_log_counter++] = log; // Store in memory
    printf("Visit log added successfully.\n");
}

void visitLogMenu(int patient_index)
{

    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s | %-15s |\n", "ID", "Name", "Age", "Blood Group", "Gender", "Disease", "Contact", "Registration D&T");
    for (int i = 0; i < patient_counter; i++)
    {
        if (strcmp(patients[i].patient_id, patients[patient_index].patient_id) == 0 && patients[i].status == PATIENT_ACTIVE)
        {

            char reg_time_str[25];
            time_t currentTime = time(NULL);
            formatRegistrationTime(currentTime, reg_time_str, sizeof(reg_time_str));

            printf("| %-5s | %-20s | %-3d | %-6s | %-15s | %-15s | %-19s | %-15s |\n",
                   patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_blood_group,
                   patients[i].p_gender, patients[i].p_disease, patients[i].p_contact_num, reg_time_str);

            printf("-------------------------------------------------------------------------------------------------------\n");

            break;
        }
    }
    do
    {
        int action_choice;
        printf("\nWhat do you want to do next for patient %s (ID: %s)?\n", patients[patient_index].p_name, patients[patient_index].patient_id);
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
                addVisitLog(patients[patient_index].patient_id);
                printf("Visit Log Added Successfully!\n");
                printf("Do you want to add another visit for %s (ID: %s)? (Y/N): ",
                       patients[patient_index].p_name, patients[patient_index].patient_id);
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
            displayVisitHistory(patients[patient_index].patient_id);
            break;
        case 3:
            printf("Returning to search menu.\n");
            break;
            return;
        default:
            printf("Invalid choice! Please enter a valid option.\n");
            continue;
        }
        break;
    } while (1);
}
