/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_CHECKSUM_H
#define PKSAV_GBA_CHECKSUM_H

#include <pksav/config.h>

#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <stdint.h>

uint16_t pksav_get_gba_pokemon_checksum(
    const pksav_gba_pc_pokemon_t* gba_pokemon
);

static PKSAV_INLINE void pksav_set_gba_pokemon_checksum(
    pksav_gba_pc_pokemon_t* gba_pokemon
) {
    gba_pokemon->checksum = pksav_get_gba_pokemon_checksum(gba_pokemon);
}

uint16_t pksav_get_gba_section_checksum(
    const pksav_gba_save_section_t* section,
    uint8_t section_num
);

static PKSAV_INLINE void pksav_set_gba_section_checksums(
    pksav_gba_save_sections_t* sections
) {
    for(uint8_t i = 0; i < 14; ++i) {
        sections->sections_arr[i].footer.checksum = pksav_get_gba_section_checksum(
                                                        &sections->sections_arr[i], i
                                                    );
    }
}

#endif /* PKSAV_GBA_CHECKSUM_H */
