/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gba/pokemon.h>

#include <stdbool.h>
#include <string.h>

static const uint8_t gen3_block_orders[24][4] = {
    /* A  E  G  M */
    /* GAEM */ {1, 2, 0, 3},
    /* GAME */ {1, 3, 0, 2},
    /* GEAM */ {2, 1, 0, 3},
    /* GEMA */ {3, 1, 0, 2},
    /* GMAE */ {2, 3, 0, 1},
    /* GMEA */ {3, 2, 0, 1},
    /* AGEM */ {0, 2, 1, 3},
    /* AGME */ {0, 3, 1, 2},
    /* AEGM */ {0, 1, 2, 3},
    /* AEMG */ {0, 1, 3, 2},
    /* AMGE */ {0, 3, 2, 1},
    /* AMEG */ {0, 2, 3, 1},
    /* EGAM */ {2, 0, 1, 3},
    /* EGMA */ {3, 0, 1, 2},
    /* EAGM */ {1, 0, 2, 3},
    /* EAMG */ {1, 0, 3, 2},
    /* EMGA */ {3, 0, 2, 1},
    /* EMAG */ {2, 0, 3, 1},
    /* MGAE */ {2, 3, 1, 0},
    /* MGEA */ {3, 2, 1, 0},
    /* MAGE */ {1, 3, 2, 0},
    /* MAEG */ {1, 2, 3, 0},
    /* MEGA */ {3, 1, 2, 0},
    /* MEAG */ {2, 1, 3, 0}
};

static void _pksav_crypt_gba_pokemon(
    pksav_gba_pc_pokemon_t* gen3_pokemon,
    bool encrypt
) {
    uint32_t security_key = gen3_pokemon->ot_id.id ^ gen3_pokemon->personality;
    for(size_t i = 0; i < 12; i++) {
        gen3_pokemon->blocks.blocks32[i] ^= security_key;
    }

    int index = gen3_pokemon->personality % 24;
    int attacks_index = gen3_block_orders[index][0];
    int effort_index  = gen3_block_orders[index][1];
    int growth_index  = gen3_block_orders[index][2];
    int misc_index    = gen3_block_orders[index][3];

    pksav_gba_pokemon_blocks_t blocks;
    if(encrypt) {
        memcpy(&blocks.blocks[growth_index],  &gen3_pokemon->blocks.growth,  sizeof(pksav_gba_pokemon_growth_t));
        memcpy(&blocks.blocks[attacks_index], &gen3_pokemon->blocks.attacks, sizeof(pksav_gba_pokemon_attacks_t));
        memcpy(&blocks.blocks[effort_index],  &gen3_pokemon->blocks.effort,  sizeof(pksav_gba_pokemon_effort_t));
        memcpy(&blocks.blocks[misc_index],    &gen3_pokemon->blocks.misc,    sizeof(pksav_gba_pokemon_misc_t));
    } else {
        memcpy(&blocks.growth,  &gen3_pokemon->blocks.blocks[growth_index],  sizeof(pksav_gba_pokemon_growth_t));
        memcpy(&blocks.attacks, &gen3_pokemon->blocks.blocks[attacks_index], sizeof(pksav_gba_pokemon_attacks_t));
        memcpy(&blocks.effort,  &gen3_pokemon->blocks.blocks[effort_index],  sizeof(pksav_gba_pokemon_effort_t));
        memcpy(&blocks.misc,    &gen3_pokemon->blocks.blocks[misc_index],    sizeof(pksav_gba_pokemon_misc_t));
    }

    gen3_pokemon->blocks = blocks;
}
