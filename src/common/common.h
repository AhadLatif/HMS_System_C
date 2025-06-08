#ifndef COMMON_H
#define COMMON_H

void mainFunction();

void inputString(char *target, int size);

void exitProgram();
int fileCheck(FILE *file);
void welcomScreen(void);
void login();
int inputInt(const char *prompt);
void formatRegistrationTime(time_t registration_time, char *buffer, size_t bufferSize);

#endif