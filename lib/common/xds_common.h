/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_XDS_COMMON_H
#define PKSAV_COMMON_XDS_COMMON_H

#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

//! Convert a string from in-game Unicode to a multi-byte C string.
enum pksav_error pksav_xds_import_text(
    const uint16_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
);

//! Convert a multi-byte C string to in-game Unicode.
enum pksav_error pksav_xds_export_text(
    const char* p_input_text,
    uint16_t* p_output_buffer,
    size_t num_chars
);

#endif /* PKSAV_COMMON_XDS_COMMON_H */
