/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/pokedex.h>

#include <math.h>

static PKSAV_INLINE void _pksav_get_pokedex_bit_pos(
    uint16_t pokedex_num,
    uint8_t* index,
    uint8_t* mask
) {
    *index = (uint8_t)((pokedex_num-1)/8);
    *mask  = (uint8_t)(pow(2.0, ((pokedex_num-1)%8)));
}

pksav_error_t pksav_get_pokedex_bit(
    const uint8_t* raw,
    uint16_t pokedex_num,
    bool* result_out
) {
    if(!raw || !result_out) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint8_t index, mask;
    _pksav_get_pokedex_bit_pos(pokedex_num, &index, &mask);
    *result_out = (bool)(raw[index] & mask);

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_set_pokedex_bit(
    uint8_t* raw,
    uint16_t pokedex_num,
    bool set
) {
    if(!raw) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint8_t index, mask;
    _pksav_get_pokedex_bit_pos(pokedex_num, &index, &mask);
    if(set) {
        raw[index] |= mask;
    } else {
        raw[index] &= ~mask;
    }

    return PKSAV_ERROR_NONE;
}
