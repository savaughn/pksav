/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_TEXT_H
#define PKSAV_GBA_TEXT_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a string from Game Boy Advance format to a UTF-8 C string
/*!
 * In Game Boy Advance games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param p_input_buffer Game Boy Advance string
 * \param p_output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if p_input_buffer or p_output_text is NULL
 */
PKSAV_API enum pksav_error pksav_gba_import_text(
    const uint8_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
);

//! Convert a UTF-8 string to Game Boy Advance format
/*!
 * In Game Boy Advance games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param p_input_text C string to convert
 * \param p_output_buffer output buffer in which to place Game Boy Advance string
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if p_input_text or p_output_buffer is NULL
 */
PKSAV_API enum pksav_error pksav_gba_export_text(
    const char* p_input_text,
    uint8_t* p_output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_TEXT_H */
