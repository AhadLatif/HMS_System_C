// <<<<<<< HEAD
// =======
#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 100

typedef enum
{
    PATIENT_ACTIVE,
    PATIENT_DEACTIVE,
    PATIENT_DISCHARGED,
    PATIENT_UNDER_DYGNOSIS
} patientStatus;

typedef struct
{
    char p_name[50];
    int p_age;
    int p_id;
    char p_contact_num[15];
    char p_gender[10];
    char p_disease[50];
    int assigned_d_id;
    patientStatus status;
    // int p_admission_d[20];
    // char p_status[20];
} Patient;

extern Patient patients[MAX_PATIENTS];
extern int patient_counter;

// Functions

// Core Functions
void patientModule();
void addPatient();
void searchPatientById();
void searchPatientByName();
void deletePatient();

// Screen Functions

void displayPatient();
void displayPatientMenu(void);

// FileHandling Functions

void savePatientsToFile();
void loadPatientFromFile();

#endif
// >>>>>>> Patient-Module-18-05-2025
