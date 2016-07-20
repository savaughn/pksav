/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"
#include "crypt.h"
#include "shuffle.h"

#include <string.h>

void pksav_gba_save_load_pokemon_pc(
    const pksav_gba_save_sections_t* gba_save_sections,
    pksav_gba_pokemon_pc_t* pokemon_pc_out
) {
    // Copy data from sections into contiguous data structure
    uint8_t* dst_ptr = (uint8_t*)pokemon_pc_out;
    for(uint8_t i = 0; i < 13; ++i) {
        memcpy(dst_ptr, gba_save_sections->sections_arr[i].data8, pksav_gba_section_sizes[i]);
        dst_ptr += pksav_gba_section_sizes[i];
    }

    // Decrypt Pokémon
    for(uint8_t i = 0; i < 14; ++i) {
        for(uint8_t j = 0; j < 30; ++j) {
            pksav_gba_crypt_pokemon(
                &pokemon_pc_out->boxes[i].entries[j],
                false
            );
        }
    }
}

void pksav_gba_save_save_pokemon_pc(
    pksav_gba_pokemon_pc_t* pokemon_pc,
    pksav_gba_save_sections_t* gba_save_sections_out
) {
    // Encrypt Pokémon
    for(uint8_t i = 0; i < 14; ++i) {
        for(uint8_t j = 0; j < 30; ++j) {
            pksav_set_gba_pokemon_checksum(
                &pokemon_pc->boxes[i].entries[j]
            );
        }
    }

    // Copy contiguous data structure back into sections
    uint8_t* src_ptr = (uint8_t*)pokemon_pc;
    for(uint8_t i = 5; i <= 13; ++i) {
        memcpy(
            gba_save_sections_out->sections_arr[i].data8,
            src_ptr,
            pksav_gba_section_sizes[i]
        );
        src_ptr += pksav_gba_section_sizes[i];
    }
}
