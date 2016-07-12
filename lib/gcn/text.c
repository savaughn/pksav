/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gcn/text.h>

#include <pksav/math/endian.h>

#include <stdint.h>
#include <string.h>

/*
 * Gamecube games store strings in UCS-2 big endian format.
 */

static PKSAV_INLINE void ucs2be_mbtowc(
    const uint8_t* mb,
    uint32_t* wc
) {
    *wc = (mb[0] << 8) + mb[1];
}

static PKSAV_INLINE void ucs2be_wctomb(
    const uint32_t* wc,
    uint8_t* mb
) {
    mb[0] = (uint8_t)((*wc) >> 8);
    mb[1] = (uint8_t)(*wc);
}

void pksav_text_from_gcn(
    const uint32_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    memset(output_text, 0, num_chars);

    for(size_t i = 0; i < (num_chars/2); ++i) {
        if(input_buffer[i] >= 10000 ||
           (input_buffer[i] >= 0xD800 && input_buffer[i] < 0xE000) ||
           input_buffer[i] == 0xFFFF)
        {
            break;
        } else {
            ucs2be_wctomb(&input_buffer[i], &output_text[2*i]);
        }
    }
}

void pksav_widetext_from_gcn(
    const uint32_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    memset(output_text, 0, sizeof(wchar_t)*num_chars);

    char* text = malloc(num_chars);
    pksav_text_from_gcn(
        input_buffer, text, num_chars
    );

    mbstowcs(output_text, text, num_chars);
    free(text);
}

void pksav_text_to_gcn(
    const char* input_text,
    uint32_t* output_buffer,
    size_t num_chars
) {
    memset(output_buffer, 0xFFFF, 4*num_chars);

    for(size_t i = 0; i < num_chars; i += 2) {
        ucs2be_mbtowc(&input_text[i], &output_buffer[2*i]);
    }
}

void pksav_widetext_to_gcn(
    const wchar_t* input_text,
    uint32_t* output_buffer,
    size_t num_chars
) {
    char* text = malloc(num_chars);
    wcstombs(text, input_text, num_chars);

    pksav_text_to_gcn(
        text, output_buffer, num_chars
    );

    free(text);
}
