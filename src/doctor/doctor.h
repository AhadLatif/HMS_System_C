#ifndef DOCTOR_H
#define DOCTOR_H

// Doctor structure definition
MAX_COUNT 
typedef struct
{
    int d_id;
    char d_name[50];
    char d_gender[10];
    char d_specialization[50];
    char d_contact[15];
} Doctor;

// Function declarations for doctor operations
// CORE FUNCTIONS 

void doctorModule();
void addDoctor();
void displayDoctors();
void searchDoctorById();
void searchDoctorByName();
void searchDoctorBySpecialization();
void deleteDoctor();


// OTHERS
void displayDoctorMenu ();
void displayDoctors ();

// FILE HANDLING
void loadDoctorsFromFile();
void saveDoctorsToFile();

#endif // DOCTOR_H
