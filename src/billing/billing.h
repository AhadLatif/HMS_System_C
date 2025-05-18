#ifndef BILLING_H
#define BILLING_H

// Billing structure definition
typedef struct {
    int bill_id;
    int patient_id;
    float amount;
    char date[20];
    char status[20];
} Bill;

// Function declarations
// TODO: Add billing-related function declarations here

#endif // BILLING_H
