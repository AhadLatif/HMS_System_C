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

    int choice;
    printf("1. Patient Module\n");
    printf("2. Doctor Module\n");
    choice = inputInt("Choose an option :");

    switch (choice)
    { 
    case 1:
        patientModule();
        break;
    case 2:
        doctorModule();
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }

    return 0;
}
