/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../common/xds_common.h"

#include <pksav/gen6/text.h>

pksav_error_t pksav_text_from_gen6(
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

pksav_error_t pksav_text_to_gen6(
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
