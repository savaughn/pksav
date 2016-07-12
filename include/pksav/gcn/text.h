/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GCN_TEXT_H
#define PKSAV_GCN_TEXT_H

#include <pksav/config.h>

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API void pksav_text_from_gcn(
    const uint32_t* input_buffer,
    char* output_text,
    size_t num_chars
);

PKSAV_API void pksav_widetext_from_gcn(
    const uint32_t* input_buffer,
    wchar_t* output_widetext,
    size_t num_chars
);

PKSAV_API void pksav_text_to_gcn(
    const char* input_text,
    uint32_t* output_buffer,
    size_t num_chars
);

PKSAV_API void pksav_widetext_to_gcn(
    const wchar_t* input_text,
    uint32_t* output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GCN_TEXT_H */
