#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 100
typedef struct
{
    char p_name[50];
    int p_age;
    int p_id;
    char p_contact_num[15];
    char p_gender[10];
    char p_disease[50];
    // int p_admission_d[20];
    // char p_status[20];
} Patient;

extern Patient patients[MAX_PATIENTS];
extern int patient_counter;

// Functions

// Core Functions
void addPatient();
void searchPatientById();
void searchPatientByName();
void deletePatient();

// Screen Functions

void displayPatient();
void displayMenu(void);

// FileHandling Functions

void savePatientsToFile();
void loadPatientFromFile();

#endif
