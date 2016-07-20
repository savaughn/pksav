/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_SHUFFLE_H
#define PKSAV_GBA_SHUFFLE_H

#include <pksav/config.h>

#include <pksav/gba/pokemon.h>
#include <pksav/gba/save_structs.h>
#include <pksav/gba/save.h>

static PKSAV_INLINE void pksav_gba_save_unshuffle_sections(
    const pksav_gba_save_sections_t* save_sections_in,
    pksav_gba_save_sections_t* save_sections_out,
    uint8_t section_nums[14]
) {
    for(uint8_t i = 0; i < 14; ++i) {
        uint8_t section_id = save_sections_in->sections_arr[i].footer.section_id;
        save_sections_out->sections_arr[section_id] = save_sections_in->sections_arr[i];

        // Cache the original positions
        section_nums[i] = section_id;
    }
}

static PKSAV_INLINE void pksav_gba_save_shuffle_sections(
    const pksav_gba_save_sections_t* save_sections_in,
    pksav_gba_save_sections_t* save_sections_out,
    const uint8_t section_nums[14]
) {
    for(uint8_t i = 0; i < 14; ++i) {
        save_sections_out->sections_arr[i] = save_sections_in->sections_arr[section_nums[i]];
    }
}

void pksav_gba_save_load_pokemon_pc(
    const pksav_gba_save_sections_t* gba_save_sections,
    pksav_gba_pokemon_pc_t* pokemon_pc_out
);

void pksav_gba_save_save_pokemon_pc(
    pksav_gba_pokemon_pc_t* pokemon_pc,
    pksav_gba_save_sections_t* gba_save_sections_out
);

#endif /* PKSAV_GBA_SHUFFLE_H */
