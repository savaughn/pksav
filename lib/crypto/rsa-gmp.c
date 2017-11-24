/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "rsa-gmp.h"

#include "mini-gmp.h"

#include <string.h>

void pksav_rsa_decrypt(
    pksav_rsa_fields_t* rsa_fields_ptr,
    const uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size
)
{
    uint8_t tmp_buf[PKSAV_RSA_NUM_BYTES];
    size_t output_len = 0;

    mpz_t modulus_mpz;
    mpz_t privkey_mpz;
    mpz_t input_mpz;
    mpz_t output_mpz;

    mpz_init(modulus_mpz);
    mpz_init(privkey_mpz);
    mpz_init(input_mpz);
    mpz_init(output_mpz);

    // Take in caller input.
    mpz_import(
        modulus_mpz,
        sizeof(rsa_fields_ptr->modulus),
        1, 1, 1, 0,
        rsa_fields_ptr->modulus
    );
    mpz_import(
        privkey_mpz,
        sizeof(rsa_fields_ptr->privkey),
        1, 1, 1, 0,
        rsa_fields_ptr->privkey
    );
    mpz_import(
        input_mpz,
        input_buffer_size,
        1, 1, 1, 0,
        input_buffer
    );

    // (input ^ privkey) % modulus
    mpz_powm(
        output_mpz,
        input_mpz,
        privkey_mpz,
        modulus_mpz
    );

    mpz_export(
        tmp_buf,
        &output_len,
        1, 1, 1, 0,
        output_mpz
    );

    output_len = (output_len < output_buffer_size) ? output_len
                                                   : output_buffer_size;

    memset(
        output_buffer,
        0,
        (output_buffer_size - output_len)
    );
    memcpy(
        output_buffer + (output_buffer_size - output_len),
        tmp_buf,
        output_len
    );
}

void pksav_rsa_encrypt(
    pksav_rsa_fields_t* rsa_fields_ptr,
    const uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size
)
{
    uint8_t tmp_buf[PKSAV_RSA_NUM_BYTES];
    size_t output_len = 0;

    mpz_t modulus_mpz;
    mpz_t input_mpz;
    mpz_t output_mpz;

    mpz_init(modulus_mpz);
    mpz_init(input_mpz);
    mpz_init(output_mpz);

    // Take in caller input.
    mpz_import(
        modulus_mpz,
        sizeof(rsa_fields_ptr->modulus),
        1, 1, 1, 0,
        rsa_fields_ptr->modulus
    );
    mpz_import(
        input_mpz,
        input_buffer_size,
        1, 1, 1, 0,
        input_buffer
    );

    // (input ^ privkey) % modulus
    mpz_powm_ui(
        output_mpz,
        input_mpz,
        0x10001,
        modulus_mpz
    );

    mpz_export(
        tmp_buf,
        &output_len,
        1, 1, 1, 0,
        output_mpz
    );

    output_len = (output_len < output_buffer_size) ? output_len
                                                   : output_buffer_size;

    memset(
        output_buffer,
        0,
        (output_buffer_size - output_len)
    );
    memcpy(
        output_buffer + (output_buffer_size - output_len),
        tmp_buf,
        output_len
    );
}
