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
    int selection;
    printf("Select an option:\n");
    printf("1. Doctor Module\n");
    printf("2. Patient Module\n");
    printf("Enter your choice: ");
    if (scanf("%d", &selection) != 1) {
        printf("Invalid input. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    switch(selection)
    {
        case 1:
            doctorModule();
            break;
        case 2:
            patientModule();
            break;
        default:
            printf("Invalid option.\n");
            break;
    }

    return 0;
}
