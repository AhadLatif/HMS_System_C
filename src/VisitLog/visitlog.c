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

void displayVisitLog(int patient_index)
{
    printf("\n=== Visit History ===\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-20s | %-50s | %-20s |\n",
           "Visit ID", "Doctor Name", "Reason", "Visit Time");
    printf("--------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < visit_log_counter; i++)
    {
        if (strcmp(visit_logs[i].patient_id, patients[patient_index].patient_id) == 0)
        {
            char visit_time_str[25];
            formatRegistrationTime(visit_logs[i].visit_time, visit_time_str, sizeof(visit_time_str));

            printf("| %-8d | %-20s | %-50s | %-20s |\n",
                   visit_logs[i].visit_id,
                   visit_logs[i].assigned_doctor,
                   visit_logs[i].reason,
                   visit_time_str);
            found = 1;
        }
    }
    printf("--------------------------------------------------------------------------------------------------------\n");
    if (!found)
    {
        printf("No visit records found for this patient.\n");
    }
}

void addVisitLog(char *patient_id)
{
    if (visit_log_counter >= MAX_VISIT_LOGS)
    {
        printf("\nVisit log is full. Cannot add more logs.\n");
        return;
    }
    VisitLog new_log;
    strncpy(new_log.patient_id, patient_id, sizeof(new_log.patient_id) - 1);
    new_log.patient_id[sizeof(new_log.patient_id) - 1] = '\0'; // Ensure null termination
    new_log.visit_id = getNextVisitID();
    // Unique ID for each visit

    printf("\nEnter reason for visit: ");
    inputString(new_log.reason, sizeof(new_log.reason));

    printf("\nEnter assigned doctor: ");
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
        printf("\n[ERROR] Could not open visitlog.dat for reading. Please ensure the file exists and is accessible.\n");
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
        printf("\nVisit log is full. Cannot add more logs.\n");
        return;
    }
    FILE *file = fopen("./data/visitlog.dat", "ab");
    if (!file)
    {
        printf("\n[ERROR] Could not open visitlog.dat for writing. Please ensure the file is accessible.\n");
        return;
    }
    fwrite(&log, sizeof(VisitLog), 1, file);
    fclose(file);
    visit_logs[visit_log_counter++] = log; // Store in memory
    printf("\nVisit log added successfully.\n");
}

void visitLogMenu(int patient_index)
{
    printf("\n=== Patient Information ===\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-20s | %-3s | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-20s |\n",
           "ID", "Name", "Age", "Gender", "Blood Group", "Disease", "Contact", "CNIC", "Minor", "Registration Time");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    char reg_time_str[25];
    formatRegistrationTime(patients[patient_index].registration_time, reg_time_str, sizeof(reg_time_str));

    printf("| %-8s | %-20s | %-3d | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-20s |\n",
           patients[patient_index].patient_id, patients[patient_index].p_name, patients[patient_index].p_age,
           patients[patient_index].p_gender, patients[patient_index].p_blood_group, patients[patient_index].p_disease,
           patients[patient_index].p_contact_num, patients[patient_index].p_cnic,
           patients[patient_index].is_minor ? "Yes" : "No", reg_time_str);
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (1)
    {
        printf("\n=== Visit Log Actions ===\n");
        printf("1. Add New Visit\n");
        printf("2. Display Visit History\n");
        printf("3. Go back to Search Menu\n");
        printf("\nEnter your choice: ");

        int choice = inputInt("");
        switch (choice)
        {
        case 1:
            addVisitLog(patients[patient_index].patient_id);
            break;
        case 2:
            displayVisitLog(patient_index);
            break;
        case 3:
            printf("\nReturning to search menu...\n");
            return;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    }
}
