/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"
#include "crypt.h"
#include "shuffle.h"

#include <pksav/config.h>

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <pksav/math/endian.h>

#include <stdio.h>

#define SECURITY_KEY1(sections,game) (sections)->section0.data32[pksav_gba_section0_offsets[PKSAV_GBA_SECURITY_KEY1][game]/4]
#define SECURITY_KEY2(sections,game) (sections)->section0.data32[pksav_gba_section0_offsets[PKSAV_GBA_SECURITY_KEY2][game]/4]
#define SAVE_INDEX(sections)         pksav_littleendian32((sections)->section0.footer.save_index)

#define SECTION0_DATA8(sections,game,offset) \
    (sections)->section0.data8[pksav_gba_section0_offsets[offset][game]]

#define SECTION0_DATA32(sections,game,offset) \
    (sections)->section0.data32[pksav_gba_section0_offsets[offset][game]/4]

#define SECTION1_DATA8(sections,game,offset) \
    (sections)->section1.data8[pksav_gba_section1_offsets[offset][game]]

#define SECTION1_DATA32(sections,game,offset) \
    (sections)->section1.data32[pksav_gba_section1_offsets[offset][game]/4]

#define SECTION2_DATA8(sections,game,offset) \
    (sections)->section2.data8[pksav_gba_section2_offsets[offset][game]]

#define SECTION2_DATA32(sections,game,offset) \
    (sections)->section2.data32[pksav_gba_section2_offsets[offset][game]/4]

#define SECTION4_DATA8(sections,game,offset) \
    (sections)->section4.data8[pksav_gba_section4_offsets[offset][game]]

#define SECTION4_DATA32(sections,game,offset) \
    (sections)->section4.data32[pksav_gba_section4_offsets[offset][game]/4]

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Offsets
 */

#define PKSAV_GBA_SAVE_SIZE  0x20000 // TODO: small Gen III saves
#define PKSAV_GBA_VALIDATION 0x08012025

typedef enum {
    PKSAV_GBA_SAVE_A = 0x0000,
    PKSAV_GBA_SAVE_B = 0xE000
} pksav_gba_save_offset_t;

typedef enum {
    PKSAV_GBA_NAT_POKEDEX_A = 0,
    PKSAV_GBA_POKEDEX_OWNED,
    PKSAV_GBA_POKEDEX_SEEN_A,
    PKSAV_GBA_GAME_CODE,
    PKSAV_GBA_SECURITY_KEY1,
    PKSAV_GBA_SECURITY_KEY2
} pksav_gba_section0_field_t;

static const uint16_t pksav_gba_section0_offsets[][4] = {
    {0x019,0x019,0x01B},
    {0x028,0x028,0x028},
    {0x05C,0x05C,0x05C},
    {0x0AC,0x0AC,0x1F4}, // RS has no security key
    {0x0AC,0x0AC,0x1F4}, // Emerald has no game code
    {0x0AC,0xAF8,0xF20}
};

typedef enum {
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_ITEM_STORAGE,
    PKSAV_GBA_POKEDEX_SEEN_B
} pksav_gba_section1_field_t;

static const uint16_t pksav_gba_section1_offsets[][4] = {
    {0x234,0x490,0x498},
    {0x234,0x490,0x498},
    {0x034,0x290,0x298},
    {0x938,0x988,0x5F8}
};

typedef enum {
    PKSAV_GBA_NAT_POKEDEX_B = 0,
    PKSAV_GBA_NAT_POKEDEX_C
} pksav_gba_section2_field_t;

static const uint16_t pksav_gba_section2_offsets[][4] = {
    {0x3A6,0x402,0x068},
    {0x44C,0x4A8,0x11C}
};

typedef enum {
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
} pksav_gba_section4_field_t;

static const uint16_t pksav_gba_section4_offsets[][4] = {
    {0xC0C,0xCA4,0xB98},
    {0x000,0x000,0xBCC} // FR/LG only
};

static bool _pksav_file_is_gba_save(
    const uint8_t* data,
    pksav_gba_game_t gba_game
) {
    const pksav_gba_save_slot_t* save_slot = (const pksav_gba_save_slot_t*)data;
    for(uint8_t i = 0; i < 14; ++i) {
        if(save_slot->sections_arr[i].footer.validation != PKSAV_GBA_VALIDATION) {
            return false;
        }
    }

    uint32_t security_key1 = SECURITY_KEY1(save_slot, gba_game);
    uint32_t security_key2 = SECURITY_KEY2(save_slot, gba_game);

    printf("_pksav_file_is_gba_save (%d): %u %u\n", gba_game, security_key1, security_key2);

    if(gba_game == PKSAV_GBA_RS) {
        return (security_key1 == security_key2) && (security_key1 == 0);
    } else {
        return (security_key1 == security_key2);
    }

    return true;
}

bool pksav_file_is_gba_save(
    const char* filepath,
    pksav_gba_game_t gba_game
) {
    FILE* gba_save = fopen(filepath, "r");
    if(!gba_save) {
        return false;
    }

    fseek(gba_save, 0, SEEK_END);

    if(ftell(gba_save) < PKSAV_GBA_SAVE_SIZE) {
        return false;
    }

    uint8_t* gba_save_data = malloc(PKSAV_GBA_SAVE_SIZE);
    fseek(gba_save, 0, SEEK_SET);
    size_t num_read = fread((void*)gba_save_data, 1, PKSAV_GBA_SAVE_SIZE, gba_save);
    fclose(gba_save);

    bool ret = false;
    if(num_read == PKSAV_GBA_SAVE_SIZE) {
        ret = _pksav_file_is_gba_save(gba_save_data, gba_game);
    }

    free(gba_save_data);
    return ret;
}

// Assumes all dynamically allocated memory has already been allocated
static void _pksav_gba_save_set_pointers(
    pksav_gba_save_t* gba_save
) {
    // Find the most recent save slot
    const pksav_gba_save_slot_t* sections_pair = (const pksav_gba_save_slot_t*)gba_save->raw;
    const pksav_gba_save_slot_t* most_recent;
    if(SAVE_INDEX(&sections_pair[0]) > SAVE_INDEX(&sections_pair[1])) {
        most_recent = &sections_pair[0];
        gba_save->from_first_slot = true;
    } else {
        most_recent = &sections_pair[1];
        gba_save->from_first_slot = false;
    }

    // Set pointers
    pksav_gba_save_unshuffle_sections(
        most_recent,
        gba_save->unshuffled,
        gba_save->shuffled_section_nums
    );
    gba_save->trainer_info = &gba_save->unshuffled->trainer_info;
    gba_save->rival_name = &SECTION4_DATA8(
                               gba_save->unshuffled,
                               gba_save->gba_game,
                               PKSAV_GBA_FRLG_RIVAL_NAME
                           );
    gba_save->pokemon_party = (pksav_gba_pokemon_party_t*)&SECTION1_DATA8(
                                                              gba_save->unshuffled,
                                                              gba_save->gba_game,
                                                              PKSAV_GBA_POKEMON_PARTY
                                                          );
    for(uint8_t i = 0; i < 6; ++i) {
        pksav_gba_crypt_pokemon(
            &gba_save->pokemon_party->party[i].pc,
            false
        );
    }

    pksav_gba_save_load_pokemon_pc(
        gba_save->unshuffled,
        gba_save->pokemon_pc
    );

    gba_save->item_storage = (pksav_gba_item_storage_t*)&SECTION1_DATA8(
                                                            gba_save->unshuffled,
                                                            gba_save->gba_game,
                                                            PKSAV_GBA_ITEM_STORAGE
                                                        );
    pksav_gba_save_crypt_items(
        gba_save->item_storage,
        SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game),
        gba_save->gba_game
    );

    gba_save->money = &SECTION1_DATA32(
                          gba_save->unshuffled,
                          gba_save->gba_game,
                          PKSAV_GBA_MONEY
                      );
    *gba_save->money ^= SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game);
}

pksav_error_t pksav_gba_save_load(
    const char* filepath,
    pksav_gba_save_t* gba_save
) {
    // Read the file and make sure it's valid
    FILE* gba_save_file = fopen(filepath, "r");
    if(!gba_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gba_save_file, 0, SEEK_END);
    if(ftell(gba_save_file) < PKSAV_GBA_SAVE_SIZE) {
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gba_save->raw = malloc(PKSAV_GBA_SAVE_SIZE);
    fseek(gba_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gba_save->raw, 1, PKSAV_GBA_SAVE_SIZE, gba_save_file);
    fclose(gba_save_file);
    if(num_read != PKSAV_GBA_SAVE_SIZE) {
        return PKSAV_ERROR_FILE_IO;
    }

    // Detect what kind of save this is
    if(_pksav_file_is_gba_save(gba_save->raw, PKSAV_GBA_RS)) {
        gba_save->gba_game = PKSAV_GBA_RS;
    } else if(_pksav_file_is_gba_save(gba_save->raw, PKSAV_GBA_EMERALD)) {
        gba_save->gba_game = PKSAV_GBA_EMERALD;
    } else if(_pksav_file_is_gba_save(gba_save->raw, PKSAV_GBA_FRLG)) {
        gba_save->gba_game = PKSAV_GBA_FRLG;
    } else {
        free(gba_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    // Allocate memory as needed and set pointers
    gba_save->unshuffled = malloc(sizeof(pksav_gba_save_slot_t));
    gba_save->pokemon_pc = malloc(sizeof(pksav_gba_pokemon_pc_t));
    _pksav_gba_save_set_pointers(
        gba_save
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gba_save_save(
    const char* filepath,
    pksav_gba_save_t* gba_save
) {
    // Make sure we can write to this file
    FILE* gen1_save_file = fopen(filepath, "w");
    if(!gen1_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    *gba_save->money ^= SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game);

    pksav_gba_save_crypt_items(
        gba_save->item_storage,
        SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game),
        gba_save->gba_game
    );

    pksav_gba_save_save_pokemon_pc(
        gba_save->pokemon_pc,
        gba_save->unshuffled
    );

    for(uint8_t i = 0; i < 6; ++i) {
        pksav_gba_crypt_pokemon(
            &gba_save->pokemon_party->party[i].pc,
            true
        );
    }

    // Find the least recent save slot, increment the save index, save into that
    uint32_t save_index = pksav_littleendian32(SAVE_INDEX(gba_save->unshuffled) + 1);
    for(uint8_t i = 0; i < 14; ++i) {
        gba_save->unshuffled->sections_arr[i].footer.save_index = save_index;
    }
    pksav_gba_save_slot_t* sections_pair = (pksav_gba_save_slot_t*)gba_save->raw;
    pksav_gba_save_slot_t* save_into = gba_save->from_first_slot ? &sections_pair[1]
                                                                 : &sections_pair[0];
    gba_save->from_first_slot = !gba_save->from_first_slot;

    pksav_gba_save_shuffle_sections(
        gba_save->unshuffled,
        save_into,
        gba_save->shuffled_section_nums
    );

    // With everything saved to the new slot, reload it
    _pksav_gba_save_set_pointers(
        gba_save
    );

    return PKSAV_ERROR_NONE;
}
