/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "xds_common.h"

#include <string.h>

#define PKSAV_XDS_TERMINATOR 0xFFFF

pksav_error_t _pksav_text_from_xds(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    if(!input_buffer || !output_text) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    _pksav_widetext_from_xds(
        input_buffer, widetext, num_chars
    );

    memset(output_text, 0, num_chars);
    wcstombs(output_text, widetext, num_chars);
    free(widetext);

    return PKSAV_ERROR_NONE;
}

pksav_error_t _pksav_widetext_from_xds(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    if(!input_buffer || !output_text) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    memset(output_text, 0, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_buffer[i] == PKSAV_XDS_TERMINATOR) {
            break;
        } else {
            output_text[i] = input_buffer[i];
        }
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t _pksav_text_to_xds(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    if(!input_text || !output_buffer) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    mbstowcs(widetext, input_text, num_chars);

    _pksav_widetext_to_xds(
        widetext, output_buffer, num_chars
    );

    free(widetext);

    return PKSAV_ERROR_NONE;
}

pksav_error_t _pksav_widetext_to_xds(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    if(!input_text || !output_buffer) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    memset(output_buffer, PKSAV_XDS_TERMINATOR, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_text[i] == 0) {
            break;
        } else {
            output_buffer[i] = (uint16_t)input_text[i];
        }
    }

    return PKSAV_ERROR_NONE;
}
