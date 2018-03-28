/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/pokedex.h>

#include <pksav/gba/pokedex.h>

// TODO

enum pksav_error pksav_gba_pokedex_set_has_seen(
    struct pksav_gba_pokedex* gba_pokedex_ptr,
    uint16_t pokedex_num,
    bool has_seen
)
{
    if(!gba_pokedex_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    (void)pokedex_num;
    (void)has_seen;

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gba_pokedex_set_national_pokedex_unlocked(
    struct pksav_gba_pokedex* gba_pokedex_ptr,
    enum pksav_gba_save_type save_type,
    bool is_unlocked
)
{
    if(!gba_pokedex_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if((save_type < PKSAV_GBA_SAVE_TYPE_NONE) || (save_type > PKSAV_GBA_SAVE_TYPE_FRLG))
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    (void)is_unlocked;

    return PKSAV_ERROR_NONE;
}
