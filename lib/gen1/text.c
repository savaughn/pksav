/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen1/text.h>

#include <string.h>

#define PKSAV_GEN1_TERMINATOR 0x50
#define PKSAV_GEN1_SPACE      0x7F

/*
 * Character map for Generation I
 *
 * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#Text_data
 *
 * There are no Unicode values for the "PK" and "MN" in-game characters, so we will use the
 * "<" and ">" characters, as they are not used in-game. Any application displaying this text
 * will need to graphically substitute in these characters.
 */
static const wchar_t pksav_gen1_char_map[] = {
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',0x20,
    0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
    0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x28,0x29,0x3A,0x3B,0x5B,0x5D,
    0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
    0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,'\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
    '\0',0x3C,0x3E,0x2D,'\0','\0',0x3F,0x21,0x2E,'\0','\0','\0','\0','\0','\0',0x2642,
    '\0',0xD7,'\0',0x2F,0x2C,0x2640,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
};

// C equivalent of std::distance
static ssize_t wchar_map_index(
    const wchar_t* char_map,
    size_t char_map_size,
    wchar_t to_find
) {
    for(ssize_t i = 0; i < (ssize_t)char_map_size; ++i) {
        if(char_map[i] == to_find) {
            return i;
        }
    }

    return -1;
}

void pksav_text_from_gen1(
    const uint8_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    memset(output_text, 0, num_chars);

    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    pksav_widetext_from_gen1(
        input_buffer, widetext, num_chars
    );

    wcstombs(output_text, widetext, num_chars);
    free(widetext);
}

void pksav_widetext_from_gen1(
    const uint8_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    memset(output_text, 0, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_buffer[i] == PKSAV_GEN1_TERMINATOR) {
            break;
        } else {
            output_text[i] = pksav_gen1_char_map[input_buffer[i]];
        }
    }
}

void pksav_text_to_gen1(
    const char* input_text,
    uint8_t* output_buffer,
    size_t num_chars
) {
    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    mbstowcs(widetext, input_text, num_chars);

    pksav_widetext_to_gen1(
        widetext, output_buffer, num_chars
    );

    free(widetext);
}

void pksav_widetext_to_gen1(
    const wchar_t* input_text,
    uint8_t* output_buffer,
    size_t num_chars
) {
    memset(output_buffer, PKSAV_GEN1_TERMINATOR, num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_text[i] == 0x20) {
            output_buffer[i] = PKSAV_GEN1_SPACE;
        } else {
            ssize_t index = wchar_map_index(pksav_gen1_char_map, 255, input_text[i]);
            if(index == -1) {
                break;
            } else {
                output_buffer[i] = (uint8_t)index;
            }
        }
    }
}
