/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN5_TEXT_H
#define PKSAV_GEN5_TEXT_H

#include <pksav/config.h>

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API void pksav_text_from_gen5(
    const uint16_t* input_buffer,
    char* output_text,
    size_t num_chars
);

PKSAV_API void pksav_widetext_from_gen5(
    const uint16_t* input_buffer,
    wchar_t* output_widetext,
    size_t num_chars
);

PKSAV_API void pksav_text_to_gen5(
    const char* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

PKSAV_API void pksav_widetext_to_gen5(
    const wchar_t* input_text,
    uint16_t* output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN5_TEXT_H */
