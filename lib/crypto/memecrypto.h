/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_CRYPTO_MEMECRYPTO_H
#define PKSAV_CRYPTO_MEMECRYPTO_H

#include <stdint.h>
#include <stdlib.h>

#define PKSAV_MEMECRYPTO_LEN 0x60

#ifdef __cplusplus
extern "C"
{
#endif

void pksav_memecrypto_encrypt(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len,
    const uint8_t* key_buffer
);

void pksav_memecrypto_decrypt(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len,
    const uint8_t* key_buffer
);

void pksav_memecrypto_sign(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len
);

void pksav_memecrypto_verify(
    const uint8_t* input_buffer,
    uint8_t* output_buffer,
    size_t buffer_len
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_CRYPTO_MEMECRYPTO_H */
