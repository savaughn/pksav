/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "crypt.h"

#include <string.h>

static const uint8_t gba_block_orders[24][4] =
{
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

void pksav_gba_crypt_pokemon(
    struct pksav_gba_pc_pokemon* gba_pokemon,
    bool encrypt
) {
    uint32_t security_key = gba_pokemon->ot_id.id ^ gba_pokemon->personality;
    for(uint8_t i = 0; i < 12; ++i) {
        gba_pokemon->blocks.blocks32[i] ^= security_key;
    }

    int index = gba_pokemon->personality % 24;
    int attacks_index = gba_block_orders[index][0];
    int effort_index  = gba_block_orders[index][1];
    int growth_index  = gba_block_orders[index][2];
    int misc_index    = gba_block_orders[index][3];

    union pksav_gba_pokemon_blocks blocks;
    if(encrypt) {
        memcpy(&blocks.blocks[growth_index],  &gba_pokemon->blocks.growth,  sizeof(struct pksav_gba_pokemon_growth_block));
        memcpy(&blocks.blocks[attacks_index], &gba_pokemon->blocks.attacks, sizeof(struct pksav_gba_pokemon_attacks_block));
        memcpy(&blocks.blocks[effort_index],  &gba_pokemon->blocks.effort,  sizeof(struct pksav_gba_pokemon_effort_block));
        memcpy(&blocks.blocks[misc_index],    &gba_pokemon->blocks.misc,    sizeof(struct pksav_gba_pokemon_misc_block));
    } else {
        memcpy(&blocks.growth,  &gba_pokemon->blocks.blocks[growth_index],  sizeof(struct pksav_gba_pokemon_growth_block));
        memcpy(&blocks.attacks, &gba_pokemon->blocks.blocks[attacks_index], sizeof(struct pksav_gba_pokemon_attacks_block));
        memcpy(&blocks.effort,  &gba_pokemon->blocks.blocks[effort_index],  sizeof(struct pksav_gba_pokemon_effort_block));
        memcpy(&blocks.misc,    &gba_pokemon->blocks.blocks[misc_index],    sizeof(struct pksav_gba_pokemon_misc_block));
    }

    gba_pokemon->blocks = blocks;
}

void pksav_gba_save_crypt_items(
    union pksav_gba_item_bag* item_storage,
    uint32_t security_key,
    pksav_gba_game_t gba_game
) {
    struct pksav_item* items = (struct pksav_item*)item_storage;
    uint8_t num_items = 0;
    switch(gba_game) {
        case PKSAV_GBA_RS:
            num_items = sizeof(struct pksav_gba_rs_item_bag) / sizeof(struct pksav_item);
            break;

        case PKSAV_GBA_EMERALD:
            num_items = sizeof(struct pksav_gba_emerald_item_bag) / sizeof(struct pksav_item);
            break;

        default:
            num_items = sizeof(struct pksav_gba_frlg_item_bag) / sizeof(struct pksav_item);
            break;
    }
    for(uint8_t i = 0; i < num_items; ++i) {
        items[i].count ^= (uint16_t)(security_key & 0xFFFF);
    }
}
