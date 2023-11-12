/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_SHUFFLE_H
#define PKSAV_GBA_SHUFFLE_H

#include "gba/save_internal.h"

#include <pksav/config.h>

#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

void pksav_gba_save_unshuffle_sections(
    const union pksav_gba_save_slot* save_slot_in,
    union pksav_gba_save_slot* save_slot_out,
    uint8_t* section_nums_out
);

void pksav_gba_save_shuffle_sections(
    const union pksav_gba_save_slot* save_slot_in,
    union pksav_gba_save_slot* save_slot_out,
    const uint8_t* p_section_nums
);

void pksav_gba_save_load_pokemon_pc(
    const union pksav_gba_save_slot* gba_save_slot,
    struct pksav_gba_pokemon_pc* pokemon_pc_out
);

void pksav_gba_save_save_pokemon_pc(
    struct pksav_gba_pokemon_pc* p_pokemon_pc,
    union pksav_gba_save_slot* gba_save_slot_out
);

#endif /* PKSAV_GBA_SHUFFLE_H */
