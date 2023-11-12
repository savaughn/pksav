/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/text_common.h"

#include <locale.h>
#include <stdio.h>
#include <string.h>

#if defined(PKSAV_PLATFORM_MINGW) || defined(PKSAV_PLATFORM_WIN32)

#include <windows.h>

void pksav_mbstowcs(
    wchar_t* p_output,
    const char* p_input,
    size_t num_chars
)
{
    MultiByteToWideChar(
        CP_UTF8,
        0,
        p_input,
        -1,
        p_output,
        (int)num_chars
    );
}

void pksav_wcstombs(
    char* p_output,
    const wchar_t* p_input,
    size_t num_chars
)
{
    WideCharToMultiByte(
        CP_UTF8,
        0,
        p_input,
        -1,
        p_output,
        (int)num_chars,
        NULL,
        NULL
    );
}

#else

void pksav_mbstowcs(
    wchar_t* p_output,
    const char* p_input,
    size_t num_chars
)
{
    char* p_old_locale = setlocale(LC_CTYPE, NULL);
    setlocale(LC_CTYPE, "en_US.utf8");
    mbstowcs(p_output, p_input, num_chars);
    if(p_old_locale)
    {
        (void)setlocale(LC_CTYPE, p_old_locale);
    }
}

void pksav_wcstombs(
    char* p_output,
    const wchar_t* p_input,
    size_t num_chars
)
{
    char* p_old_locale = setlocale(LC_CTYPE, NULL);
    setlocale(LC_CTYPE, "en_US.utf8");
    wcstombs(p_output, p_input, num_chars);
    if(p_old_locale)
    {
        (void)setlocale(LC_CTYPE, p_old_locale);
    }
}

#endif

// C equivalent of std::distance
ssize_t wchar_map_index(
    const wchar_t* p_char_map,
    size_t char_map_size,
    wchar_t to_find
)
{
    for(size_t char_index = 0;
        char_index < char_map_size;
        ++char_index)
    {
        if(p_char_map[char_index] == to_find)
        {
            return char_index;
        }
    }
    return -1;
}

