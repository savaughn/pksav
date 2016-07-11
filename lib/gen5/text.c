/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen5/text.h>

#include <string.h>

#define PKSAV_GEN5_TERMINATOR 0xFFFF

void pksav_text_from_gen5(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    memset(output_text, 0, num_chars);

    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    pksav_widetext_from_gen5(
        input_buffer, widetext, num_chars
    );

    wcstombs(output_text, widetext, num_chars);
    free(widetext);
}

void pksav_widetext_from_gen5(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    memset(output_text, 0, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_buffer[i] == PKSAV_GEN5_TERMINATOR) {
            break;
        } else {
            output_text[i] = input_buffer[i];
        }
    }
}

void pksav_text_to_gen5(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    wchar_t* widetext = malloc(sizeof(wchar_t)*num_chars);
    mbstowcs(widetext, input_text, num_chars);

    pksav_widetext_to_gen5(
        widetext, output_buffer, num_chars
    );

    free(widetext);
}

void pksav_widetext_to_gen5(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    memset(output_buffer, PKSAV_GEN5_TERMINATOR, sizeof(wchar_t)*num_chars);

    for(size_t i = 0; i < num_chars; ++i) {
        if(input_text[i] == 0) {
            break;
        } else {
            output_buffer[i] = (uint16_t)input_text[i];
        }
    }
}
