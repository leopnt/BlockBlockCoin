#ifndef BLOCK_H
#define BLOCK_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "lib/sha256.h"
#include "utils.h"
#include "transaction.h"

/* give length of the data to make a block less than 1024 bytes long */
#define NUM_TRANSACTIONS 7
#define DATA_SIZE NUM_TRANSACTIONS * sizeof(Transaction)

/* buffer to print the struct as str */
static char _block_str_buf_out[2048];

typedef struct
{
    uint16_t id;           /* uint16_t: maximum unique id: 65 534 */
    uint64_t nonce;        /* this is 8 bytes long */
    char timestamp[21];    /* ISO-8601 "2021-12-24T05:57:06Z" is 20 char long */
    uint8_t prev_hash[32]; /* sha-256 is 256 bit long so 32 bytes */
    uint8_t hash[32];
    Transaction transactions[NUM_TRANSACTIONS];
} Block;

Block *block_new(
    int id,
    uint8_t prev_hash[SHA256_BLOCK_SIZE],
    Transaction transactions[NUM_TRANSACTIONS]);
char *block_str(const Block *b);
void block_hash_calc(uint8_t hash[SHA256_BLOCK_SIZE], const Block *b);
bool is_signed(const Block *b, const uint8_t *difficulty);
void block_mine(Block *b, uint8_t difficulty);

#endif