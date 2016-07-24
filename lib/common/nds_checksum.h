/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_COMMON_NDS_CHECKSUM_H
#define PKSAV_COMMON_NDS_CHECKSUM_H

#include <pksav/config.h>

#include <pksav/common/nds_pokemon.h>

#include <stdint.h>

uint16_t pksav_nds_block_checksum(
    const uint8_t* data,
    uint16_t len
);

uint16_t pksav_nds_pokemon_get_checksum(
    const pksav_nds_pc_pokemon_t* nds_pokemon
);

static PKSAV_INLINE void pksav_nds_pokemon_set_checksum(
    pksav_nds_pc_pokemon_t* nds_pokemon
) {
    nds_pokemon->checksum = pksav_nds_pokemon_get_checksum(nds_pokemon);
}

#endif /* PKSAV_COMMON_NDS_CHECKSUM_H */
