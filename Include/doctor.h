#ifndef DOCTOR_H
#define DOCTOR_H

// Doctor structure definition
typedef struct
{
    int d_id;
    char d_name[50];
    char d_specialization[50];
    char d_contact[15];
} Doctor;

// Function declarations for doctor operations
void addDoctor();
void displayDoctors();
void searchDoctorById();
void searchDoctorByName();
void deleteDoctor();

#endif // DOCTOR_H
