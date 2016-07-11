/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_TEXT_H
#define PKSAV_TEXT_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API pksav_error_t pksav_text_from_game(
    size_t generation,
    const void* input_buffer,
    char* output_text,
    size_t num_chars
);

PKSAV_API pksav_error_t pksav_widetext_from_game(
    size_t generation,
    const void* input_buffer,
    wchar_t* output_widetext,
    size_t num_chars
);

PKSAV_API pksav_error_t pksav_text_to_game(
    size_t generation,
    const char* input_text,
    void* output_buffer,
    size_t num_chars
);

PKSAV_API pksav_error_t pksav_widetext_to_game(
    size_t generation,
    const wchar_t* input_text,
    void* output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_TEXT_H */
