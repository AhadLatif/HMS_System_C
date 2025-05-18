#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../include/common.h"
#include "../include/id_manager.h"

IDManager id_manager;
void initializeMetaFile()
{
}

void loadIDManager()
{

    FILE *file = fopen("./data/meta.dat", "rb");

    if (file != NULL)
    {
        fread(&id_manager, sizeof(IDManager), 1, file);
        fclose(file);
    }
    else
    {
        id_manager.next_bill_id = 1;
        id_manager.next_doctor_id = 1;
        id_manager.next_patient_id = 1;
    }
};

void saveIDManager()
{

    FILE *file = fopen("./data/meta.dat", "wb");

    if (file != NULL)
    {
        fwrite(&id_manager, sizeof(IDManager), 1, file);
        fclose(file);
    }
}