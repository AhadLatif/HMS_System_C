#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "src/doctor/doctor.h"
#include "src/common/common.h"
#include "src/id_manager/id_manager.h"
#include "doctor.h"

// CORE
void doctorModule(){

    loadDoctorsFromFile();
    displayDoctorMenu();

    int choice;
    choice = inputInt("Enter your choice : ");


}
void addDoctor()
{

    
}

void searchDoctorById()
{
}

void searchDoctorByName()
{
}

void searchDoctorBySpecialization()
{
}

void deleteDoctor()
{
}

// File handling
void loadDoctorsFromFile()
{
}

void saveDoctorsToFile()
{
}
// Others

void displayDoctorMenu()
{
    printf("\nDoctor Selection Menu:\n");
    printf("---------------------------------------------------------------\n");
    printf("| %-20s | %-6s | %-20s | %-15s |\n", "Name", "Gender", "Specialization", "Contact");
    printf("---------------------------------------------------------------\n");
    // Additional code to loop over and display doctor records can be added here.
}

void displayDoctors()
{
}