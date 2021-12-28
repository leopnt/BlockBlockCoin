#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ADDR_SIZE 64

char _transaction_str_buf_out[256];

typedef struct
{
    char fromAddr[ADDR_SIZE];
    char toAddr[ADDR_SIZE];
    uint32_t amount;
} Transaction;

Transaction transaction_new(char fromAddr[], char toAddr[], uint32_t amount);
char *transaction_to_str(const Transaction *transaction);

#endif