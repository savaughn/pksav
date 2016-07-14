/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "text_common.h"

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

