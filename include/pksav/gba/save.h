/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_H
#define PKSAV_GBA_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gba/items.h>
#include <pksav/gba/save_structs.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    PKSAV_GBA_RS = 0,
    PKSAV_GBA_EMERALD,
    PKSAV_GBA_FRLG
} pksav_gba_game_t;

// How many bytes in each section are read for the checksum
static const uint16_t pksav_gba_section_sizes[14] = {
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};

/*
 * Save file structure to be used by user
 */
typedef struct {
    pksav_gba_trainer_info_t* trainer_info;
    uint8_t* rival_name;

    pksav_gba_pokemon_party_t* pokemon_party;
    pksav_gba_pokemon_pc_t* pokemon_pc;

    pksav_gba_item_storage_t* item_storage;

    uint32_t* money;

    pksav_gba_game_t gba_game;

    // Do not edit these
    uint32_t security_key;
    uint8_t shuffled_section_nums[14];
    pksav_gba_save_sections_t* unshuffled;
    uint8_t* raw;
} pksav_gba_save_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API bool pksav_file_is_gba_save(
    const char* filepath,
    pksav_gba_game_t gba_game
);

PKSAV_API pksav_error_t pksav_gba_save_load(
    const char* filepath,
    pksav_gba_save_t* gba_save
);

PKSAV_API pksav_error_t pksav_gba_save_save(
    const char* filepath,
    pksav_gba_save_t* gba_save
);

static PKSAV_INLINE void pksav_gba_save_free(
    pksav_gba_save_t* gba_save
) {
    free(gba_save->pokemon_pc);
    free(gba_save->unshuffled);
    free(gba_save->raw);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
