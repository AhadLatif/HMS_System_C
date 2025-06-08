#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "common/common.h"
#include "id_manager.h"

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
        id_manager.next_visit_id = 1;
    }
};


int getNextVisitID()
{
    FILE *file = fopen("./data/meta.dat", "rb+");
    if (!file)
    {
        printf("Error opening ID manager file.\n");
        return -1;
    }

    IDManager id_manager;
    fread(&id_manager, sizeof(IDManager), 1, file);

    int current_id = id_manager.next_visit_id;
    id_manager.next_visit_id++; // Increment

    rewind(file);
    fwrite(&id_manager, sizeof(IDManager), 1, file);
    fclose(file);

    return current_id;
}
void saveIDManager()
{

    FILE *file = fopen("./data/meta.dat", "wb");

    if (file != NULL)
    {
        fwrite(&id_manager, sizeof(IDManager), 1, file);
        fclose(file);
    }
}