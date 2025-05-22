
<<<<<<< HEAD
#include "src/patient/patient.h"
// Doctor structure definition
#define MAX_DOCTORS 100

typedef enum
{

    ACTIVE,
    DEACTIVE
} DoctorStatus;

typedef struct
{
    int d_id;
    int d_age;
    char d_name[50];
    char d_gender[10];
    char d_specialization[50];
    char d_contact[15];
    DoctorStatus status;

} Doctor;

extern Doctor doctors[MAX_DOCTORS];
extern int doctor_counter;
// Function declarations for doctor operations
// CORE FUNCTIONS
void doctorModule();
void addDoctor();
void displayDoctors();
void searchDoctorById();
void searchDoctorByName();
void searchDoctorBySpecialization();
void searchDoctor();
void deleteDoctor();

// OTHERS
void displayDoctorMenu();
void displayDoctors();

// FILE HANDLING
void loadDoctorsFromFile();
void saveDoctorsToFile();

#endif
=======
>>>>>>> Patient-Module-18-05-2025
