#ifndef DOCTOR_H
#define DOCTOR_H


typedef struct 
{

}Doctor;

void addDoctor();
void deleteDoctor();
void searchDoctorById();
void searchDoctorByName();
void searchDoctorBySpecialization();
void deleteDoctor();

// File Handling 
void saveDoctorToFile();
void loadDoctorFromFile();

//Others

void displayDoctorMenu();
void displayDoctor();


#endif