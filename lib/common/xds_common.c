/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/text_common.h"
#include "common/xds_common.h"

#include <assert.h>
#include <string.h>

#define PKSAV_XDS_TERMINATOR 0xFFFF

static enum pksav_error _pksav_xds_import_widetext(
    const uint16_t* p_input_buffer,
    wchar_t* p_output_widetext,
    size_t num_chars
)
{
    assert(p_input_buffer != NULL);
    assert(p_output_widetext != NULL);

    memset(p_output_widetext, 0, sizeof(wchar_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_buffer[char_index] == PKSAV_XDS_TERMINATOR)
        {
            break;
        }
        else
        {
            p_output_widetext[char_index] = (wchar_t)p_input_buffer[char_index];
        }
    }

    return PKSAV_ERROR_NONE;
}

static enum pksav_error _pksav_xds_export_widetext(
    const wchar_t* p_input_widetext,
    uint16_t* p_output_buffer,
    size_t num_chars
)
{
    assert(p_input_widetext != NULL);
    assert(p_output_buffer != NULL);

    memset(p_output_buffer, 0xFF, sizeof(uint16_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_widetext[char_index] == 0)
        {
            break;
        }
        else
        {
            p_output_buffer[char_index] = (uint16_t)p_input_widetext[char_index];
        }
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_xds_import_text(
    const uint16_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
)
{
    if(!p_input_buffer || !p_output_text)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    _pksav_xds_import_widetext(
        p_input_buffer, p_widetext, num_chars
    );

    memset(p_output_text, 0, num_chars);
    pksav_wcstombs(p_output_text, p_widetext, num_chars);
    free(p_widetext);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_xds_export_text(
    const char* p_input_text,
    uint16_t* p_output_buffer,
    size_t num_chars
)
{
    if(!p_input_text || !p_output_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    pksav_mbstowcs(p_widetext, p_input_text, num_chars);

    _pksav_xds_export_widetext(
        p_widetext, p_output_buffer, num_chars
    );

    free(p_widetext);

    return PKSAV_ERROR_NONE;
}
