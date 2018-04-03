/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/text_common.h"
#include "common/xds_common.h"

#include <assert.h>
#include <string.h>

#define PKSAV_XDS_TERMINATOR 0xFFFF

static enum pksav_error _pksav_xds_import_widetext(
    const uint16_t* input_buffer,
    wchar_t* output_widetext,
    size_t num_chars
)
{
    assert(input_buffer != NULL);
    assert(output_widetext != NULL);

    memset(output_widetext, 0, sizeof(wchar_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(input_buffer[char_index] == PKSAV_XDS_TERMINATOR)
        {
            break;
        }
        else
        {
            output_widetext[char_index] = (wchar_t)input_buffer[char_index];
        }
    }

    return PKSAV_ERROR_NONE;
}

static enum pksav_error _pksav_xds_export_widetext(
    const wchar_t* input_widetext,
    uint16_t* output_buffer,
    size_t num_chars
)
{
    assert(input_widetext != NULL);
    assert(output_buffer != NULL);

    memset(output_buffer, 0xFF, sizeof(uint16_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(input_widetext[char_index] == 0)
        {
            break;
        }
        else
        {
            output_buffer[char_index] = (uint16_t)input_widetext[char_index];
        }
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_xds_import_text(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
)
{
    if(!input_buffer || !output_text)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* widetext = calloc(num_chars, sizeof(wchar_t));
    _pksav_xds_import_widetext(
        input_buffer, widetext, num_chars
    );

    memset(output_text, 0, num_chars);
    pksav_wcstombs(output_text, widetext, num_chars);
    free(widetext);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_xds_export_text(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
)
{
    if(!input_text || !output_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* widetext = calloc(num_chars, sizeof(wchar_t));
    pksav_mbstowcs(widetext, input_text, num_chars);

    _pksav_xds_export_widetext(
        widetext, output_buffer, num_chars
    );

    free(widetext);

    return PKSAV_ERROR_NONE;
}
