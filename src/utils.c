#include "utils.h"

char *sha256_to_str(const uint8_t hash[SHA256_BLOCK_SIZE])
{
    memset(_sha256_str_buf_out, 0, sizeof(_sha256_str_buf_out));

    strcat(_sha256_str_buf_out, "0x");

    char hex_char[3];
    for (uint8_t i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        sprintf(hex_char, "%x", hash[i]);      /* convert to hex */
        strcat(_sha256_str_buf_out, hex_char); /* add to buffer */
    }

    return _sha256_str_buf_out;
}

void sha256(uint8_t dest[], const uint8_t phrase[], const size_t phrase_len)
{
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, phrase, phrase_len);
    sha256_final(&ctx, dest);
}

char *byte_to_str(uint8_t byte)
{
    memset(_byte_str_buf_out, 0, sizeof(_byte_str_buf_out));
    for (uint8_t bit_idx = 0; bit_idx < 8; bit_idx++)
    {
        if (byte & 0b10000000) /* check if first bit of the byte is 1 */
            _byte_str_buf_out[bit_idx] = '1';
        else
            _byte_str_buf_out[bit_idx] = '0';

        byte <<= 1; /* left shift one bit to the left */
    }

    return _byte_str_buf_out;
}