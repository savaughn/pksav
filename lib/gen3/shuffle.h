/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef pksav_gen3_SHUFFLE_H
#define pksav_gen3_SHUFFLE_H

#include "gen3/save_internal.h"

#include <pksav/config.h>

#include <pksav/gen3/pokemon.h>
#include <pksav/gen3/save.h>

void pksav_gen3_save_unshuffle_sections(
    const union pksav_gen3_save_slot* save_slot_in,
    union pksav_gen3_save_slot* save_slot_out,
    uint8_t* section_nums_out
);

void pksav_gen3_save_shuffle_sections(
    const union pksav_gen3_save_slot* save_slot_in,
    union pksav_gen3_save_slot* save_slot_out,
    const uint8_t* p_section_nums
);

void pksav_gen3_save_load_pokemon_pc(
    const union pksav_gen3_save_slot* gen3_save_slot,
    struct pksav_gen3_pokemon_pc* pokemon_pc_out
);

void pksav_gen3_save_save_pokemon_pc(
    struct pksav_gen3_pokemon_pc* p_pokemon_pc,
    union pksav_gen3_save_slot* gen3_save_slot_out
);

#endif /* pksav_gen3_SHUFFLE_H */
