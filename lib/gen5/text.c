/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/xds_common.h"

#include <pksav/gen5/text.h>

enum pksav_error pksav_text_from_gen5(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
) {
    return _pksav_text_from_xds(
                input_buffer,
                output_text,
                num_chars
            );
}

enum pksav_error pksav_widetext_from_gen5(
    const uint16_t* input_buffer,
    wchar_t* output_text,
    size_t num_chars
) {
    return _pksav_widetext_from_xds(
                input_buffer,
                output_text,
                num_chars
            );
}

enum pksav_error pksav_text_to_gen5(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    return _pksav_text_to_xds(
                input_text,
                output_buffer,
                num_chars
            );
}

enum pksav_error pksav_widetext_to_gen5(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
) {
    return _pksav_widetext_to_xds(
                input_text,
                output_buffer,
                num_chars
            );
}
