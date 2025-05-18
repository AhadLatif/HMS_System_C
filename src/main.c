#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "../include/patient.h"
#include "../include/common.h"
#include "../include/doctor.h"
#include "../include/billing.h"
#include "../include/id_manager.h"

int main()
{

    loadPatientFromFile();

    loadIDManager();

    // welcomScreen();
    // login();
    int choice;
    do
    {
        displayMenu();

        choice = inputInt("Enter your choice: ");

        if (choice == 1)
            addPatient();
        else if (choice == 2)
            displayPatient();
        else if (choice == 3)
        {
            int search_choice;
            printf("Search Patient by:\n1. ID\n2. Name\nEnter choice: ");
            search_choice = inputInt("enter choice");
            if (search_choice == 1)
                searchPatientById();
            else if (search_choice == 2)
                searchPatientByName();
            else
                printf("Error\n");
        }
        else if (choice == 4)
            deletePatient();
        else
        {
            printf("You enter wrong choice ! \n");
            exitProgram();
        }
    } while (choice >= 1 && choice <= 4);

    return 0;
}
