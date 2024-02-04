/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen3/crypt.h"
#include "gen3/save_internal.h"

#include <pksav/gen3/common.h>

#include <assert.h>
#include <string.h>

static const size_t gen3_BLOCK_ORDERS[24][4] =
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

void pksav_gen3_crypt_pokemon(
    struct pksav_gen3_pc_pokemon* p_gen3_pokemon,
    bool should_encrypt
)
{
    assert(p_gen3_pokemon != NULL);

    union pksav_gen3_pokemon_blocks_internal* p_gen3_pokemon_internal_blocks =
        (union pksav_gen3_pokemon_blocks_internal*)&p_gen3_pokemon->blocks;

    uint32_t security_key = p_gen3_pokemon->ot_id.id
                          ^ p_gen3_pokemon->personality;

    for(size_t index = 0;
        index < (pksav_gen3_POKEMON_ALL_BLOCKS_SIZE_BYTES/4);
        ++index)
    {
        p_gen3_pokemon_internal_blocks->blocks32[index] ^= security_key;
    }

    size_t index         = p_gen3_pokemon->personality % 24;
    size_t attacks_index = gen3_BLOCK_ORDERS[index][0];
    size_t effort_index  = gen3_BLOCK_ORDERS[index][1];
    size_t growth_index  = gen3_BLOCK_ORDERS[index][2];
    size_t misc_index    = gen3_BLOCK_ORDERS[index][3];

    union pksav_gen3_pokemon_blocks_internal new_blocks_internal;
    if(should_encrypt)
    {
        memcpy(
            &new_blocks_internal.blocks[growth_index],
            &p_gen3_pokemon->blocks.growth,
            sizeof(struct pksav_gen3_pokemon_growth_block)
        );
        memcpy(
            &new_blocks_internal.blocks[attacks_index],
            &p_gen3_pokemon->blocks.attacks,
            sizeof(struct pksav_gen3_pokemon_attacks_block)
        );
        memcpy(
            &new_blocks_internal.blocks[effort_index],
            &p_gen3_pokemon->blocks.effort,
            sizeof(struct pksav_gen3_pokemon_effort_block)
        );
        memcpy(
            &new_blocks_internal.blocks[misc_index],
            &p_gen3_pokemon->blocks.misc,
            sizeof(struct pksav_gen3_pokemon_misc_block)
        );
    }
    else
    {
        memcpy(
            &new_blocks_internal.by_name.growth,
            &p_gen3_pokemon_internal_blocks->blocks[growth_index],
            sizeof(struct pksav_gen3_pokemon_growth_block)
        );
        memcpy(
            &new_blocks_internal.by_name.attacks,
            &p_gen3_pokemon_internal_blocks->blocks[attacks_index],
            sizeof(struct pksav_gen3_pokemon_attacks_block)
        );
        memcpy(
            &new_blocks_internal.by_name.effort,
            &p_gen3_pokemon_internal_blocks->blocks[effort_index],
            sizeof(struct pksav_gen3_pokemon_effort_block)
        );
        memcpy(
            &new_blocks_internal.by_name.misc,
            &p_gen3_pokemon_internal_blocks->blocks[misc_index],
            sizeof(struct pksav_gen3_pokemon_misc_block)
        );
    }

    p_gen3_pokemon->blocks = new_blocks_internal.by_name;
}

void pksav_gen3_save_crypt_items(
    union pksav_gen3_item_bag* p_gen3_item_bag,
    uint32_t security_key,
    enum pksav_gen3_save_type save_type
)
{
    assert(p_gen3_item_bag != NULL);
    assert(save_type >= pksav_gen3_SAVE_TYPE_RS);
    assert(save_type <= pksav_gen3_SAVE_TYPE_FRLG);

    struct pksav_item* p_items = (struct pksav_item*)p_gen3_item_bag;
    size_t num_items = 0;
    switch(save_type)
    {
        case pksav_gen3_SAVE_TYPE_RS:
            num_items = sizeof(struct pksav_gen3_rs_item_bag) / sizeof(struct pksav_item);
            break;

        case pksav_gen3_SAVE_TYPE_EMERALD:
            num_items = sizeof(struct pksav_gen3_emerald_item_bag) / sizeof(struct pksav_item);
            break;

        default:
            num_items = sizeof(struct pksav_gen3_frlg_item_bag) / sizeof(struct pksav_item);
            break;
    }
    for(size_t item_index = 0; item_index < num_items; ++item_index)
    {
        p_items[item_index].count ^= (uint16_t)(security_key & 0xFFFF);
    }
}
