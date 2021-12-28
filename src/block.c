#include "block.h"

Block *block_new(
    int id, uint8_t prev_hash[SHA256_BLOCK_SIZE],
    Transaction transactions[NUM_TRANSACTIONS])
{
    Block *b = NULL;
    b = malloc(sizeof(Block));

    b->id = id;
    b->nonce = 0;

    /* get current time and set the timestamp of the block */
    time_t now;
    time(&now);
    char *format = "%Y-%m-%dT%H:%M:%SZ";
    strftime(b->timestamp, sizeof(b->timestamp), format, gmtime(&now));

    /* set the previous hash */
    memcpy(b->prev_hash, prev_hash, SHA256_BLOCK_SIZE);

    /* set the transactions */
    for (uint8_t i = 0; i < NUM_TRANSACTIONS; i++)
        b->transactions[i] = transactions[i];

    /* calculate the hash of the block b according to the block b */
    block_hash_calc(b->hash, b);

    return b;
}

char *block_str(const Block *b)
{
    /* flush the previous block_str_buf_out buffer */
    memset(_block_str_buf_out, 0, sizeof(_block_str_buf_out));

    strcat(_block_str_buf_out, "Block {\n");
    char id_str[6];
    sprintf(id_str, "%hu", b->id); /* convert int id to str */
    strcat(_block_str_buf_out, "\tid: ");
    strcat(_block_str_buf_out, id_str);
    strcat(_block_str_buf_out, "\tnonce: ");
    char nonce_str[11];
    sprintf(nonce_str, "%llu", b->nonce); /* convert int nonce to str */
    strcat(_block_str_buf_out, nonce_str);
    strcat(_block_str_buf_out, "\ttstamp: ");
    strcat(_block_str_buf_out, b->timestamp);
    strcat(_block_str_buf_out, "\n\t");
    strcat(_block_str_buf_out, sha256_to_str(b->prev_hash));
    strcat(_block_str_buf_out, "\n\t");
    strcat(_block_str_buf_out, sha256_to_str(b->hash));
    strcat(_block_str_buf_out, "\n\tdata: \"");

    for (uint8_t i = 0; i < NUM_TRANSACTIONS; i++)
        strcat(_block_str_buf_out, transaction_to_str(&b->transactions[i]));

    strcat(_block_str_buf_out, "\"\n}");

    return _block_str_buf_out;
}

void block_hash_calc(uint8_t hash[SHA256_BLOCK_SIZE], const Block *b)
{
    /* get sizes in bytes */
    size_t len_id = sizeof(b->id);
    size_t len_nonce = sizeof(b->nonce);
    uint8_t len_tstamp = strlen(b->timestamp);
    /* define the length of the representation as string of the transactions */
    uint16_t len_data = NUM_TRANSACTIONS * sizeof(_transaction_str_buf_out);

    /* define a phrase that will represent block's data */
    uint8_t *phrase = NULL;
    size_t phrase_size =
        len_id + len_nonce + len_tstamp + SHA256_BLOCK_SIZE + len_data;
    phrase = malloc(phrase_size);

    uint16_t cursor = 0; /* global phrase iterator */
    uint16_t i = 0;      /* simple iterator */

    /* add the 2 bytes of the id */
    memcpy(phrase + cursor, &b->id, len_id);
    cursor += len_id;

    /* add the 4 bytes of the nonce */
    memcpy(phrase + cursor, &b->nonce, len_nonce);
    cursor += len_nonce;

    /* add the timestamp to the phrase */
    for (i = 0; i < len_tstamp; i++)
        phrase[cursor++] = (uint8_t)b->timestamp[i];

    /* add the previous hash to the phrase */
    for (i = 0; i < SHA256_BLOCK_SIZE; i++)
        phrase[cursor++] = b->prev_hash[i];

    /* add the transactions str to the phrase */
    for (uint8_t j = 0; j < NUM_TRANSACTIONS; j++)
    {
        for (i = 0; i < sizeof(_transaction_str_buf_out); i++)
        {
            char *transaction_str = transaction_to_str(&b->transactions[j]);
            phrase[cursor++] = (uint8_t)transaction_str[i];
        }
    }

    /* calculate sha256 hash of the phrase */
    sha256(hash, phrase, phrase_size);

    /* DEBUG PRINT */
    /*
    printf("PHRASE: ");
    for (cursor = 0; cursor < phrase_size; cursor++)
    {
        printf("%x ", phrase[cursor]);
    }
    printf("\n");
    printf("HASH: %s\n", sha256_to_str(hash));
    */

    free(phrase);
    phrase = NULL;
}

/* check if the block's hash starts with "difficulty" number of zero-bits */
bool is_signed(const Block *b, const uint8_t *difficulty)
{
    uint8_t byte;
    uint8_t bit_counter = 0;
    for (uint8_t i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        byte = b->hash[i];

        /* read 8 bits of current byte */
        for (uint8_t bit_idx = 0; bit_idx < 8; bit_idx++)
        {
            if (byte & 0b10000000) /* check if first bit of the byte is 1 */
                return false;

            bit_counter++;
            if (bit_counter == *difficulty)
                return true;

            byte <<= 1; /* left shift one bit to the left */
        }
    }

    return true;
}

void block_mine(Block *b, uint8_t difficulty)
{
    printf("\nmining block %d...\n", b->id);
    while (!is_signed(b, &difficulty))
    {
        b->nonce++;
        block_hash_calc(b->hash, b);
    }

    printf("found a hash starting with %d 0-bits !\n", difficulty);

    /* print the hash in binary format */
    for (uint8_t i = 0; i < SHA256_BLOCK_SIZE; i++)
        printf("%s ", byte_to_str(b->hash[i]));
    printf("\n\n");
}
