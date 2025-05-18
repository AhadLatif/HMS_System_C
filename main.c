#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "src/patient/patient.h"
#include "src/common/common.h"
#include "src/doctor/doctor.h"
#include "src/billing/billing.h"
#include "src/id_manager/id_manager.h"

int main()
{
    loadPatientFromFile();
    loadIDManager();

    int choice;
    do
    {
        displayMenu();
        choice = inputInt("Enter your choice: ");
            
        switch (choice)
        {
        case 1:
            addPatient();
            break;
        case 2:
            displayPatient();
            break;
        case 3:
        {
            int search_choice;
            printf("Search Patient by:\n1. ID\n2. Name\n");
            search_choice = inputInt("Enter choice: ");
            if (search_choice == 1)
                searchPatientById();
            else if (search_choice == 2)
                searchPatientByName();
            else
                printf("Invalid search choice!\n");
        }
        break;
        case 4:
            deletePatient();
            break;
        case 5:
            exitProgram();
            return 0;
        default:
            printf("Invalid choice! Please enter a number between 1 and 5.\n");
        }

        printf("\nPress Enter to continue...");
        while (getchar() != '\n')
            ;

    } while (choice != 5);

    return 0;
}
