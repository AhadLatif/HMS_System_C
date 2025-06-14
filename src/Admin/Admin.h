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
} AdminStatus;

typedef struct
{
    char a_id[20];
    char a_name[50];
    char a_cnic[15];
    char password[MAX_PASSWORD_LENGTH];
    time_t registration_time;
    AdminStatus status;
    int failed_login_attempts;
} Admin;

extern Admin admins[MAX_ADMINS];
extern int admin_counter;

// Core Functions
void adminModule();

// Add Admin
void addAdmin();

// Search Functions
void searchAdmin();
void searchAdminByCnic();

// File Operations
void saveAdminsToFile();
void loadAdminsFromFile();

#endif
