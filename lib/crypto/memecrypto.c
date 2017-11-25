/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "memecrypto.h"

#include "aes.h"
#include "rsa-gmp.h"
#include "sha1.h"

#include <assert.h>
#include <string.h>

// Based on SciresM's reverse-engineering

#define BLOCK_SIZE 0x10

static void xor_buffers(
    uint8_t* buffer,
    const uint8_t* xor_buffer,
    size_t buffer_len
)
{
    for(size_t i = 0; i < buffer_len; ++i)
    {
        buffer[i] ^= xor_buffer[i];
    }
}

void pksav_memecrypto_encrypt(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len,
    const uint8_t* key_buffer
)
{
    uint8_t tmp_buf[BLOCK_SIZE] = {0};
    uint8_t tmp_buf2[BLOCK_SIZE] = {0};
    uint8_t subkey[BLOCK_SIZE] = {0};
    uint8_t block[BLOCK_SIZE] = {0};

    for(size_t i = 0; i < (buffer_len/BLOCK_SIZE); ++i)
    {
        memcpy(
            block,
            input_buffer + (i * BLOCK_SIZE),
            BLOCK_SIZE
        );
        xor_buffers(block, tmp_buf, BLOCK_SIZE);
        AES128_ECB_encrypt(block, key_buffer, tmp_buf);
        memcpy(
            output_buffer + (i * BLOCK_SIZE),
            tmp_buf,
            BLOCK_SIZE
        );
    }

    xor_buffers(tmp_buf, output_buffer, BLOCK_SIZE);
    for(size_t i = 0; i < BLOCK_SIZE; i += 2)
    {
        uint8_t b1 = tmp_buf[i];
        uint8_t b2 = tmp_buf[i + 1];
        subkey[i] = (b1 << 1) | (b2 >> 7);
        subkey[i + 1] = (b2 << 1);

        if(i < (BLOCK_SIZE - 2))
        {
            subkey[i + 1] |= (tmp_buf[i + 2] >> 7);
        }
    }

    if(tmp_buf[0] & 0x80)
    {
        subkey[0x0F] ^= 0x87;
    }

    memset(tmp_buf, 0, sizeof(tmp_buf));
    for(size_t i = 0; i < (buffer_len/BLOCK_SIZE); ++i)
    {
        memcpy(
            block,
            (output_buffer + ((buffer_len/BLOCK_SIZE) - i - 1) * BLOCK_SIZE),
            BLOCK_SIZE
        );
        xor_buffers(block, subkey, BLOCK_SIZE);
        AES128_ECB_encrypt(block, key_buffer, tmp_buf2);
        xor_buffers(tmp_buf2, tmp_buf, BLOCK_SIZE);
        memcpy(
            (output_buffer + ((buffer_len/BLOCK_SIZE) - i - 1) * BLOCK_SIZE),
            tmp_buf2,
            BLOCK_SIZE
        );
    }
}

void pksav_memecrypto_decrypt(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len,
    const uint8_t* key_buffer
)
{
    uint8_t tmp_buf[BLOCK_SIZE] = {0};
    uint8_t subkey[BLOCK_SIZE] = {0};
    uint8_t block[BLOCK_SIZE] = {0};

    for(size_t i = 0; i < (PKSAV_MEMECRYPTO_LEN/BLOCK_SIZE); ++i)
    {
        memcpy(
            block,
            (input_buffer + ((buffer_len/BLOCK_SIZE) - i - 1) * BLOCK_SIZE),
            BLOCK_SIZE
        );
        xor_buffers(block, tmp_buf, BLOCK_SIZE);
        AES128_ECB_decrypt(block, key_buffer, tmp_buf);
    }
    memcpy(
        tmp_buf,
        (output_buffer + ((buffer_len/BLOCK_SIZE) - 1) * BLOCK_SIZE),
        BLOCK_SIZE
    );

    xor_buffers(tmp_buf, output_buffer, BLOCK_SIZE);
    for(size_t i = 0; i < BLOCK_SIZE; i += 2)
    {
        uint8_t b1 = tmp_buf[i];
        uint8_t b2 = tmp_buf[i + 1];
        subkey[i] = (b1 << 1) | (b2 >> 7);
        subkey[i + 1] = (b2 << 1);

        if(i < (BLOCK_SIZE - 2))
        {
            subkey[i + 1] |= (tmp_buf[i + 2] >> 7);
        }
    }

    if(tmp_buf[0] & 0x80)
    {
        subkey[0xF] ^= 0x87;
    }

    for(size_t i = 0; i < (buffer_len/BLOCK_SIZE); ++i)
    {
        memcpy(
            block,
            output_buffer + (i * BLOCK_SIZE),
            BLOCK_SIZE
        );
        AES128_ECB_decrypt(
            block,
            key_buffer,
            (output_buffer + (i * BLOCK_SIZE))
        );
        xor_buffers(
            (output_buffer + (i * BLOCK_SIZE)),
            tmp_buf,
            BLOCK_SIZE
        );
        memcpy(tmp_buf, block, BLOCK_SIZE);
    }
}

// TODO
void pksav_memecrypto_sign(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len
)
{
    assert(buffer_len >= PKSAV_MEMECRYPTO_LEN);

    // Copy because we need to mark it up.
    uint8_t* input_buffer_copy = calloc(buffer_len, sizeof(uint8_t));
    memcpy(input_buffer_copy, input_buffer, buffer_len);

    uint8_t tmp_buf[PKSAV_MEMECRYPTO_LEN] = {0};

    uint8_t hash[0x14] = {0};
    sha1_ctx sha1[1] = {0};

    memcpy(
        output_buffer,
        input_buffer_copy,
        (buffer_len - PKSAV_MEMECRYPTO_LEN)
    );

    sha1_begin(sha1);
    sha1_hash(input_buffer_copy, (buffer_len - 8), sha1);
    sha1_end(hash, sha1);
    memcpy(
        (input_buffer_copy + buffer_len - 8),
        hash,
        8
    );

    (void)tmp_buf;

    free(input_buffer_copy);
}

// TODO
void pksav_memecrypto_verify(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len
)
{
    (void)input_buffer;
    (void)output_buffer;
    (void)buffer_len;
}
