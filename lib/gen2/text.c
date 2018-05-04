/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/text_common.h"

#include <pksav/gen2/text.h>

#include <assert.h>
#include <string.h>

#define PKSAV_GEN2_TEXT_SPACE (0x7F)
#define ASCII_SPACE           (0x20)

/*
 * Character map for Generation II
 *
 * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_II#Text_data
 *
 * There are no Unicode values for the "PK" and "MN" in-game characters, so we will use the
 * "<" and ">" characters, as they are not used in-game. Any application displaying this text
 * will need to graphically substitute in these characters.
 */
static const wchar_t PKSAV_GEN2_CHAR_MAP[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,
    0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
    0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x28,0x29,0x3A,0x3B,0x5B,0x5D,
    0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
    0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x27,0x3C,0x3E,0x2D,0x00,0x00,0x3F,0x21,0x2E,0x00,0xE9,0x00,0x00,0x00,0x00,0x2642,
    0x00,0xD7,0x00,0x2F,0x2C,0x2640,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
};
static const size_t PKSAV_GEN2_CHAR_MAP_SIZE =
    sizeof(PKSAV_GEN2_CHAR_MAP)/sizeof(PKSAV_GEN2_CHAR_MAP[0]);

static enum pksav_error _pksav_gen2_import_widetext(
    const uint8_t* p_input_buffer,
    wchar_t* p_output_widetext,
    size_t num_chars
)
{
    assert(p_input_buffer != NULL);
    assert(p_output_widetext != NULL);

    memset(p_output_widetext, 0, sizeof(wchar_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_buffer[char_index] == PKSAV_GEN2_TEXT_TERMINATOR)
        {
            break;
        }
        else
        {
            p_output_widetext[char_index] = PKSAV_GEN2_CHAR_MAP[p_input_buffer[char_index]];
        }
    }

    return PKSAV_ERROR_NONE;
}

static enum pksav_error _pksav_gen2_export_widetext(
    const wchar_t* p_input_widetext,
    uint8_t* p_output_buffer,
    size_t num_chars
)
{
    assert(p_input_widetext != NULL);
    assert(p_output_buffer != NULL);

    memset(p_output_buffer, PKSAV_GEN2_TEXT_TERMINATOR, num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_widetext[char_index] == ASCII_SPACE)
        {
            p_output_buffer[char_index] = PKSAV_GEN2_TEXT_SPACE;
        }
        else
        {
            ssize_t map_index = wchar_map_index(
                                    PKSAV_GEN2_CHAR_MAP,
                                    PKSAV_GEN2_CHAR_MAP_SIZE,
                                    p_input_widetext[char_index]
                                );
            if(map_index != -1)
            {
                p_output_buffer[char_index] = (uint8_t)map_index;
            }
            else
            {
                break;
            }
        }
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen2_import_text(
    const uint8_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
)
{
    if(!p_input_buffer || !p_output_text)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    _pksav_gen2_import_widetext(
        p_input_buffer, p_widetext, num_chars
    );

    memset(p_output_text, 0, num_chars);
    pksav_wcstombs(p_output_text, p_widetext, num_chars);
    free(p_widetext);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen2_export_text(
    const char* p_input_text,
    uint8_t* p_output_buffer,
    size_t num_chars
)
{
    if(!p_input_text || !p_output_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    pksav_mbstowcs(p_widetext, p_input_text, num_chars);

    _pksav_gen2_export_widetext(
        p_widetext, p_output_buffer, num_chars
    );

    free(p_widetext);

    return PKSAV_ERROR_NONE;
}
