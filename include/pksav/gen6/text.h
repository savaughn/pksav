/*!
 * @file    pksav/gen6/text.h
 * @ingroup PKSav
 * @brief   Conversions between C strings and the string format used in Generation VI games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN6_TEXT_H
#define PKSAV_GEN6_TEXT_H

#include <pksav/config.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a string from Generation VI format to a multi-byte C string
/*!
 * In Generation VI games, strings are stored as Unicode.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_buffer Generation VI string
 * \param output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 */
PKSAV_API void pksav_text_from_gen6(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
);

//! Convert a string from Generation VI format to a wide-character C string
/*!
 * In Generation VI games, strings are stored as Unicode.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_buffer Generation VI string
 * \param output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 */
PKSAV_API void pksav_widetext_from_gen6(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
);

//! Convert a multi-byte C string to Generation VI format
/*!
 * In Generation VI games, strings are stored as Unicode.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_text C string to convert
 * \param output_buffer output buffer in which to place Generation VI string
 * \param num_chars the number of characters to convert
 */
PKSAV_API void pksav_text_to_gen6(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

//! Convert a wide-character C string to Generation VI format
/*!
 * In Generation VI games, strings are stored as Unicode.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_text C string to convert
 * \param output_buffer output buffer in which to place Generation VI string
 * \param num_chars the number of characters to convert
 */
PKSAV_API void pksav_widetext_to_gen6(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN6_TEXT_H */
