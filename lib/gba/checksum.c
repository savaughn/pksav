/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"

uint16_t pksav_get_gba_pokemon_checksum(
    const pksav_gba_pc_pokemon_t* gba_pokemon
) {
    uint16_t ret = 0;

    for(uint8_t i = 0; i < (sizeof(pksav_gba_pokemon_blocks_t)/2); i++) {
        ret += gba_pokemon->blocks.blocks16[i];
    }

    return ret;
}

uint16_t pksav_get_gba_section_checksum(
    const pksav_gba_save_section_t* section,
    uint8_t section_num
) {
    uint32_t checksum = 0;
    uint16_t* checksum_ptr = (uint16_t*)&checksum;

    for(int i = 0; i < (pksav_gba_section_sizes[section_num]/4); i++) {
        checksum += section->data32[i];
    }

    return (checksum_ptr[0] + checksum_ptr[1]);
}
