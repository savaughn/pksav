/*!
 * @file    pksav/math/bcd.h
 * @ingroup PKSav
 * @brief   Conversions between Base-10 and binary-coded decimal (BCD) numbers.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_MATH_BCD_H
#define PKSAV_MATH_BCD_H

#include <pksav/config.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a number stored in binary-coded decimal (BCD) to Base-10.
/*!
 * \param buffer where BCD number is stored
 * \param num_bytes number of bytes to convert
 * \returns converted Base-10 number
 */
PKSAV_API size_t pksav_from_bcd(
    const uint8_t* buffer,
    size_t num_bytes
);

//! Convert a Base-10 number to binary-coded decimal (BCD).
/*!
 * \param num Base-10 number to convert
 * \param buffer_out where to place converted BCD number
 */
PKSAV_API void pksav_to_bcd(
    size_t num,
    uint8_t* buffer_out
);

#ifdef __cplusplus
}
#endif

#endif
