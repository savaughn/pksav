/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/xds_common.h"

#include <pksav/gen5/text.h>

enum pksav_error pksav_gen5_import_text(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
)
{
    return pksav_xds_import_text(
               input_buffer,
               output_text,
               num_chars
           );
}

enum pksav_error pksav_gen5_export_text(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
)
{
    return pksav_xds_export_text(
               input_text,
               output_buffer,
               num_chars
           );
}
