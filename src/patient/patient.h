#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 100

typedef enum
{

    PATIENT_ACTIVE,
    PATIENT_DEACTIVE,
    PATIENT_DISCHARGED,

} PatientStatus;

typedef struct
{
    char patient_id[16]; // Unique hospital ID (e.g., "P00001")
    char p_name[50];
    int p_age;
    char p_cnic[15];        // Patient's CNIC (optional, not unique)
    char guardian_cnic[15]; // Guardian's CNIC (optional, only for minors)
    int is_minor;           // 1 if minor, 0 if adult
    char p_contact_num[15];
    char p_gender[10];
    char p_disease[50];
    char p_blood_group[5];
    time_t registration_time;
    PatientStatus status;
} Patient;

extern Patient patients[MAX_PATIENTS];
extern int patient_counter;

// Functions

// Core Functions
void patientModule();
void addPatient();
void searchPatientById();
void searchPatientByName();
void searchPatientByCnic();
void deletePatient();
void updatePatient(int select_index);
void updatePatientById();
void updatePatientByName();
void updatePatientByCnic();
void deletePatientByCnic();

// Screen Functions

void displayActivePatient();
void displayDeactivePatient();
void displayAllPatient();
void displayPatient();
void displayPatientMenu(void);

// FileHandling Functions

void savePatientsToFile();
void loadPatientFromFile();

// Others
void visitLogMenu(int patient_index);
int inputValidatedDisease(char *disease, int size);

#endif
