// id_manager.h
#ifndef ID_MANAGER_H
#define ID_MANAGER_H

typedef struct
{
    int next_patient_id;
    int next_doctor_id;
    int next_bill_id;
    int next_visit_id;
} IDManager;

extern IDManager id_manager;
// Function declarations
void initializeMetaFile();
void loadIDManager();
void saveIDManager();
int getNextVisitID();

#endif
