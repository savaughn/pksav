/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/text_common.h"

#include <locale.h>
#include <stdio.h>
#include <string.h>

#if defined(PKSAV_PLATFORM_MINGW) || defined(PKSAV_PLATFORM_WIN32)

#include <windows.h>

void pksav_mbstowcs(
    wchar_t* output,
    const char* input,
    size_t num_chars
) {
    MultiByteToWideChar(
        CP_UTF8,
        0,
        input,
        -1,
        output,
        (int)num_chars
    );
}

void pksav_wcstombs(
    char* output,
    const wchar_t* input,
    size_t num_chars
) {
    WideCharToMultiByte(
        CP_UTF8,
        0,
        input,
        -1,
        output,
        (int)num_chars,
        NULL,
        NULL
    );
}

#else

void pksav_mbstowcs(
    wchar_t* output,
    const char* input,
    size_t num_chars
) {
    char* old_locale = setlocale(LC_CTYPE, NULL);
    setlocale(LC_CTYPE, "en_US.utf8");
    mbstowcs(output, input, num_chars);
    if(old_locale) {
        (void)setlocale(LC_CTYPE, old_locale);
    }
}

void pksav_wcstombs(
    char* output,
    const wchar_t* input,
    size_t num_chars
) {
    char* old_locale = setlocale(LC_CTYPE, NULL);
    setlocale(LC_CTYPE, "en_US.utf8");
    wcstombs(output, input, num_chars);
    if(old_locale) {
        (void)setlocale(LC_CTYPE, old_locale);
    }
}

#endif

// C equivalent of std::distance
ssize_t wchar_map_index(
    const wchar_t* char_map,
    size_t char_map_size,
    wchar_t to_find
) {
    for(ssize_t i = 0; i < (ssize_t)char_map_size; ++i) {
        if(char_map[i] == to_find) {
            return i;
        }
    }
    return -1;
}

