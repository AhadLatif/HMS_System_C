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

void displayVisitHistory(int patient_id)
{
    loadVisitLogs(); // Load visit logs from file

    printf("Visit History for Patient ID: %d\n", patient_id);
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("| Visit ID | Patient ID | Assigned Doctor | Reason for Visit | Visit Time |\n");
    printf("-------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < visit_log_counter; i++)
    {
        if (visit_logs[i].patient_id == patient_id)
        {
            struct tm *tm_info = localtime(&visit_logs[i].visit_time);
            char visit_time_str[25];
            strftime(visit_time_str, sizeof(visit_time_str), "%H:%M:%S %d/%m/%Y", tm_info);

            printf("| %-8d | %-11d | %-15s | %-17s | %-19s |\n",
                   visit_logs[i].visit_id, visit_logs[i].patient_id,
                   visit_logs[i].assigned_doctor, visit_logs[i].reason, visit_time_str);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No visit logs found for Patient ID: %d\n", patient_id);
    }

    printf("-------------------------------------------------------------------------------------------------------\n");
}

void addVisitLog(int patient_id)
{
    if (visit_log_counter >= MAX_VISIT_LOGS)
    {
        printf("Visit log is full. Cannot add more logs.\n");
        return;
    }
    VisitLog new_log;
    new_log.visit_id = getNextVisitID();
    // Unique ID for each visit
    new_log.patient_id = patient_id;

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

    fileCheck(fp);

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
    fileCheck(file);
    fwrite(&log, sizeof(VisitLog), 1, file);
    fclose(file);
    visit_logs[visit_log_counter++] = log; // Store in memory
    printf("Visit log added successfully.\n");
}
