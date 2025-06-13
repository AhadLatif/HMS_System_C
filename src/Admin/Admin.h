#ifndef ADMIN_H
#define ADMIN_H

#include <time.h>

#define MAX_ADMINS 100
#define MAX_PASSWORD_LENGTH 20
#define MAX_LOGIN_ATTEMPTS 3

typedef enum
{
    ADMIN_ACTIVE,
    ADMIN_DEACTIVE,
    ADMIN_BLOCK
} ADMINStatus;

typedef struct
{
    
    char d_id[20];

    char d_name[50];
    char d_cnic[15];
    char password[MAX_PASSWORD_LENGTH];
    time_t registration_time;
    ADMINStatus status;
    int failed_login_attempts;
} ADMIN;

extern ADMIN ADMINs[MAX_ADMINS];
extern int ADMIN_counter;

// Core Functions
void ADMINModule();

// Add ADMIN
void addADMIN();

// Search Functions
void searchADMIN();
void searchADMINByCnic();


// File Operations
void saveADMINsToFile();
void loadADMINsFromFile();

#endif
