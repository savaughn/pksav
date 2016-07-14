/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_XDS_COMMON_H
#define PKSAV_COMMON_XDS_COMMON_H

#include <stdint.h>
#include <stdlib.h>

/*
 * Functions shared between Nintendo DS and Nintendo 3DS games. These
 * functions will not be built if support for at least one of these
 * is not built.
 */

//! Convert a string from in-game Unicode to a multi-byte C string.
void _pksav_text_from_xds(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
);


//! Convert a string from in-game Unicode to a wide-character C string.
void _pksav_widetext_from_xds(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
);

//! Convert a multi-byte C string to in-game Unicode.
void _pksav_text_to_xds(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

//! Convert a wide-character C string to in-game Unicode.
void _pksav_widetext_to_xds(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

#endif /* PKSAV_COMMON_XDS_COMMON_H */
