/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_TEXT_COMMON_H
#define PKSAV_COMMON_TEXT_COMMON_H

#include <pksav/config.h>

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

void pksav_mbstowcs(
    wchar_t* output,
    const char* input,
    size_t num_chars
);

void pksav_wcstombs(
    char* output,
    const wchar_t* input,
    size_t num_chars
);

ssize_t wchar_map_index(
    const wchar_t* char_map,
    size_t char_map_size,
    wchar_t to_find
);

#endif /* PKSAV_COMMON_TEXT_COMMON_H */
