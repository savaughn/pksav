/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_MATH_BASE256_H
#define PKSAV_MATH_BASE256_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a Base-256 number to Base-10.
/*!
 * \param buffer where original Base-256 number is stored
 * \param num_bytes number of bytes to convert
 * \param result_out converted Base-10 number
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API enum pksav_error pksav_import_base256(
    const uint8_t* buffer,
    size_t num_bytes,
    size_t* result_out
);

//! Convert a Base-10 number to Base-256.
/*!
 * \param num Base-10 number to convert
 * \param buffer_out where to store converted Base-256 number
 * \param buffer_size size of the buffer_out parameter
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if buffer_out is NULL
 */
PKSAV_API enum pksav_error pksav_export_base256(
    size_t num,
    uint8_t* buffer_out,
    size_t buffer_size
);

#ifdef __cplusplus
}
#endif

#endif
