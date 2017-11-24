/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_CRYPTO_RSA_GMP_H
#define PKSAV_CRYPTO_RSA_GMP_H

#include <stdint.h>
#include <stdlib.h>

#define PKSAV_RSA_NUM_BITS 768
#define PKSAV_RSA_NUM_BYTES (PKSAV_RSA_NUM_BITS/8)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    uint8_t modulus[PKSAV_RSA_NUM_BYTES];
    uint8_t privkey[PKSAV_RSA_NUM_BYTES];
} pksav_rsa_fields_t;

void pksav_rsa_decrypt(
    pksav_rsa_fields_t* rsa_fields_ptr,
    const uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size
);

void pksav_rsa_encrypt(
    pksav_rsa_fields_t* rsa_fields_ptr,
    const uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_CRYPTO_RSA_GMP_H */
