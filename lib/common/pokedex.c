/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/pokedex.h>

#include <assert.h>
#include <math.h>
#include <stdlib.h>

static inline void _pksav_get_pokedex_bit_pos(
    uint16_t pokedex_num,
    uint8_t* p_index,
    uint8_t* p_mask
)
{
    assert(p_index != NULL);
    assert(p_mask != NULL);

    *p_index = (uint8_t)((pokedex_num-1)/8);
    *p_mask  = (uint8_t)(pow(2.0, ((pokedex_num-1)%8)));
}

enum pksav_error pksav_get_pokedex_bit(
    const uint8_t* p_buffer,
    uint16_t pokedex_num,
    bool* p_result_out
)
{
    if(!p_buffer || !p_result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint8_t index, mask;
    _pksav_get_pokedex_bit_pos(pokedex_num, &index, &mask);
    *p_result_out = (bool)(p_buffer[index] & mask);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_set_pokedex_bit(
    uint8_t* p_buffer,
    uint16_t pokedex_num,
    bool set
)
{
    if(!p_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    uint8_t index, mask;
    _pksav_get_pokedex_bit_pos(pokedex_num, &index, &mask);
    if(set)
    {
        p_buffer[index] |= mask;
    }
    else
    {
        p_buffer[index] &= ~mask;
    }

    return PKSAV_ERROR_NONE;
}
