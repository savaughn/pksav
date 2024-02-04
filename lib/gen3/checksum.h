/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GEN3_CHECKSUM_H
#define PKSAV_GEN3_CHECKSUM_H

#include "gen3/save_internal.h"

#include <pksav/config.h>

#include <pksav/gen3/pokemon.h>
#include <pksav/gen3/save.h>

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Expose this to allow third-party libraries to do memory comparisons.
PKSAV_API uint16_t pksav_gen3_get_pokemon_checksum(
    const struct pksav_gen3_pc_pokemon* p_gen3_pokemon
);

static inline void pksav_gen3_set_pokemon_checksum(
    struct pksav_gen3_pc_pokemon* p_gen3_pokemon
)
{
    assert(p_gen3_pokemon != NULL);

    p_gen3_pokemon->checksum = pksav_gen3_get_pokemon_checksum(p_gen3_pokemon);
}

uint16_t pksav_gen3_get_section_checksum(
    const struct pksav_gen3_save_section* p_section,
    size_t section_num
);

void pksav_gen3_set_section_checksums(
    union pksav_gen3_save_slot* p_sections
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN3_CHECKSUM_H */
