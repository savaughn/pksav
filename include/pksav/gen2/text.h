/*!
 * @file    pksav/gen2/text.h
 * @ingroup PKSav
 * @brief   Conversions between C strings and the string format used in Generation II games.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_TEXT_H
#define PKSAV_GEN2_TEXT_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a string from Generation II format to a multi-byte C string
/*!
 * In Generation II games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_buffer Generation II string
 * \param output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_buffer or output_text is NULL
 */
PKSAV_API pksav_error_t pksav_text_from_gen2(
    const uint8_t* input_buffer,
    char* output_text,
    size_t num_chars
);

//! Convert a string from Generation II format to a wide-character C string
/*!
 * In Generation II games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_buffer Generation II string
 * \param output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_buffer or output_text is NULL
 */
PKSAV_API pksav_error_t pksav_widetext_from_gen2(
    const uint8_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
);

//! Convert a multi-byte C string to Generation II format
/*!
 * In Generation II games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_text C string to convert
 * \param output_buffer output buffer in which to place Generation II string
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_text or output_buffer is NULL
 */
PKSAV_API pksav_error_t pksav_text_to_gen2(
    const char* input_text,
    uint8_t* output_buffer,
    size_t num_chars
);

//! Convert a wide-character C string to Generation II format
/*!
 * In Generation II games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_text C string to convert
 * \param output_buffer output buffer in which to place Generation II string
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_text or output_buffer is NULL
 */
PKSAV_API pksav_error_t pksav_widetext_to_gen2(
    const wchar_t* input_text,
    uint8_t* output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_TEXT_H */
