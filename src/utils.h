#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/sha256.h"

/* buffer to convert the sha256 hash to str */
/* "0x" + "<64 char long hash>" = 66 characters */
static char _sha256_str_buf_out[67];

/* e.g. "01010101" */
static char _byte_str_buf_out[9];

char *sha256_to_str(const uint8_t hash[SHA256_BLOCK_SIZE]);
void sha256(uint8_t dest[], const uint8_t phrase[], const size_t phrase_size);
char *byte_to_str(uint8_t byte);

#endif