#ifndef COMMON_H
#define COMMON_H

void mainFunction();

void inputString(char *target, int size);

void exitProgram();
int fileCheck(FILE *file);
void welcomScreen(void);
int inputInt(const char *prompt);
void formatRegistrationTime(time_t registration_time, char *buffer, size_t bufferSize);

// Input validation and helper functions
int inputValidatedGender(char *gender, int size);
int inputValidatedAge();
int inputValidatedCNIC(char *cnic, int size);
int inputValidatedContact(char *contact, int size);
int inputValidatedName(char *name, int size);
int inputValidatedBloodGroup(char *blood_group, int size);

#endif