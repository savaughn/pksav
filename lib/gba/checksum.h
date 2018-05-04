/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_CHECKSUM_H
#define PKSAV_GBA_CHECKSUM_H

#include "gba/save_internal.h"

#include <pksav/config.h>

#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t pksav_gba_get_pokemon_checksum(
    const struct pksav_gba_pc_pokemon* p_gba_pokemon
);

static inline void pksav_gba_set_pokemon_checksum(
    struct pksav_gba_pc_pokemon* p_gba_pokemon
)
{
    assert(p_gba_pokemon != NULL);

    p_gba_pokemon->checksum = pksav_gba_get_pokemon_checksum(p_gba_pokemon);
}

uint16_t pksav_gba_get_section_checksum(
    const struct pksav_gba_save_section* p_section,
    size_t section_num
);

void pksav_gba_set_section_checksums(
    union pksav_gba_save_slot* p_sections
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_CHECKSUM_H */
