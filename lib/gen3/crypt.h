/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GEN3_CRYPT_H
#define PKSAV_GEN3_CRYPT_H

#include <pksav/gen3/items.h>
#include <pksav/gen3/pokemon.h>
#include <pksav/gen3/save.h>

#include <stdbool.h>
#include <stdint.h>

void pksav_gen3_crypt_pokemon(
    struct pksav_gen3_pc_pokemon* p_gen3_pokemon,
    bool should_encrypt
);

void pksav_gen3_save_crypt_items(
    union pksav_gen3_item_bag* p_gen3_item_bag,
    uint32_t security_key,
    enum pksav_gen3_save_type save_type
);

#endif /* PKSAV_GEN3_CRYPT_H */
