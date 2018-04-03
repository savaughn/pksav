/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_MATH_BCD_H
#define PKSAV_MATH_BCD_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a number stored in binary-coded decimal (BCD) to Base-10.
/*!
 * \param buffer where BCD number is stored
 * \param num_bytes number of bytes to convert
 * \param result_out converted Base-10 number
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API enum pksav_error pksav_import_bcd(
    const uint8_t* buffer,
    size_t num_bytes,
    size_t* result_out
);

//! Convert a Base-10 number to binary-coded decimal (BCD).
/*!
 * \param num Base-10 number to convert
 * \param buffer_out where to place converted BCD number
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if buffer_out is NULL
 */
PKSAV_API enum pksav_error pksav_export_bcd(
    size_t num,
    uint8_t* buffer_out,
    size_t buffer_size
);

#ifdef __cplusplus
}
#endif

#endif
