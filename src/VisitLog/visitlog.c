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
        if (strcmp(visit_logs[i].patient_id , patient_id)==0)
        {

            char reg_time_str[25];
            time_t visit_time_str = time(NULL);
            formatRegistrationTime(visit_logs[i].visit_time,reg_time_str, sizeof(reg_time_str));

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
