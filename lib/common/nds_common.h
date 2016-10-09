/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_COMMON_NDS_COMMON_H
#define PKSAV_COMMON_NDS_COMMON_H

#include <pksav/config.h>
#include <pksav/common/nds_pokemon.h>
#include <pksav/gen4/save_structs.h>

#include <stdbool.h>
#include <stdint.h>

uint16_t pksav_nds_block_get_checksum(
    const uint8_t* data,
    uint32_t len
);

uint16_t pksav_nds_pokemon_get_checksum(
    const pksav_nds_pc_pokemon_t* nds_pokemon
);

static PKSAV_INLINE void pksav_nds_pokemon_set_checksum(
    pksav_nds_pc_pokemon_t* nds_pokemon
) {
    nds_pokemon->checksum = pksav_nds_pokemon_get_checksum(nds_pokemon);
}

void pksav_nds_crypt_pokemon(
    pksav_nds_pc_pokemon_t* nds_pokemon,
    bool encrypt
);

#endif /* PKSAV_COMMON_NDS_COMMON_H */
