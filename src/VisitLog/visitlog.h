#ifndef VISITLOG_H
#define VISITLOG_H
#include <time.h>

#define MAX_VISIT_LOGS 100

typedef struct
{
    int visit_id;         // Unique for each visit
    char patient_id[16];       // Refers to patient in patient.dat
    int doctor_id;        // Refers to doctor in doctor.dat
    char reason[100];     // Reason for visit
    char assigned_doctor[50]; // Name of the doctor assigned
    time_t visit_time;    // Time of visit
    
} VisitLog;


extern VisitLog visit_logs[MAX_VISIT_LOGS]; // Array to hold visit logs
extern int visit_log_counter; // Counter for the number of visit logs

// Functions    
typedef struct {
    int patient_id;
    VisitLog logs[MAX_VISIT_LOGS];
    int log_count;
} PatientVisitBucket;

// Function declarations
void loadVisitLogs();
void saveVisitLog(VisitLog log);
void displayVisitHistory(char* patient_id);
void addVisitLog(char* patient_id);
void visitLogMenu(int patient_index);

#endif