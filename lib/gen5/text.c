/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/xds_common.h"

#include <pksav/gen5/text.h>

enum pksav_error pksav_gen5_import_text(
    const uint16_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
)
{
    return pksav_xds_import_text(
               p_input_buffer,
               p_output_text,
               num_chars
           );
}

enum pksav_error pksav_gen5_export_text(
    const char* p_input_text,
    uint16_t* p_output_buffer,
    size_t num_chars
)
{
    return pksav_xds_export_text(
               p_input_text,
               p_output_buffer,
               num_chars
           );
}
