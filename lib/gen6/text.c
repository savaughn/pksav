/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../common/xds_common.h"

#include <pksav/gen6/text.h>

void pksav_text_from_gen6(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    _pksav_text_from_xds(
        input_buffer,
        output_text,
        num_chars
    );
}

void pksav_widetext_from_gen6(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    _pksav_widetext_from_xds(
        input_buffer,
        output_text,
        num_chars
    );
}

void pksav_text_to_gen6(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    _pksav_text_to_xds(
        input_text,
        output_buffer,
        num_chars
    );
}

void pksav_widetext_to_gen6(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    _pksav_widetext_to_xds(
        input_text,
        output_buffer,
        num_chars
    );
}
