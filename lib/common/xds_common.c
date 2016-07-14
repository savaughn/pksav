/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "xds_common.h"

#include <string.h>

#define PKSAV_XDS_TERMINATOR 0xFFFF

void _pksav_text_from_xds(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    memset(output_text, 0, num_chars);

    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    _pksav_widetext_from_xds(
        input_buffer, widetext, num_chars
    );

    wcstombs(output_text, widetext, num_chars);
    free(widetext);
}

void _pksav_widetext_from_xds(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    memset(output_text, 0, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_buffer[i] == PKSAV_XDS_TERMINATOR) {
            break;
        } else {
            output_text[i] = input_buffer[i];
        }
    }
}

void _pksav_text_to_xds(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    mbstowcs(widetext, input_text, num_chars);

    _pksav_widetext_to_xds(
        widetext, output_buffer, num_chars
    );

    free(widetext);
}

void _pksav_widetext_to_xds(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    memset(output_buffer, PKSAV_XDS_TERMINATOR, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_text[i] == 0) {
            break;
        } else {
            output_buffer[i] = (uint16_t)input_text[i];
        }
    }
}
