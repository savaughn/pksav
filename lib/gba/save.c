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

#define SECTION0_DATA16(sections,game,offset) \
    (sections)->section0.data16[pksav_gba_section0_offsets[offset][game]/2]

#define SECTION0_DATA32(sections,game,offset) \
    (sections)->section0.data32[pksav_gba_section0_offsets[offset][game]/4]

#define SECTION1_DATA8(sections,game,offset) \
    (sections)->section1.data8[pksav_gba_section1_offsets[offset][game]]

#define SECTION1_DATA16(sections,game,offset) \
    (sections)->section1.data16[pksav_gba_section1_offsets[offset][game]/2]

#define SECTION1_DATA32(sections,game,offset) \
    (sections)->section1.data32[pksav_gba_section1_offsets[offset][game]/4]

#define SECTION2_DATA8(sections,game,offset) \
    (sections)->section2.data8[pksav_gba_section2_offsets[offset][game]]

#define SECTION2_DATA16(sections,game,offset) \
    (sections)->section2.data16[pksav_gba_section2_offsets[offset][game]/2]

#define SECTION2_DATA32(sections,game,offset) \
    (sections)->section2.data32[pksav_gba_section2_offsets[offset][game]/4]

#define SECTION4_DATA8(sections,game,offset) \
    (sections)->section4.data8[pksav_gba_section4_offsets[offset][game]]

#define SECTION4_DATA32(sections,game,offset) \
    (sections)->section4.data32[pksav_gba_section4_offsets[offset][game]/4]

/*
 * Offsets
 */

#define PKSAV_GBA_SMALL_SAVE_SIZE 0x10000
#define PKSAV_GBA_SAVE_SIZE       0x20000

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
    {0x0019,0x0019,0x001B}, // National Pokédex A
    {0x0028,0x0028,0x0028}, // Pokédex Owned
    {0x005C,0x005C,0x005C}, // Pokédex Seen A
    {0x00AC,0x00AC,0x00AC}, // Game Code
    {0x00AC,0x00AC,0x0AF8}, // Security Key 1
    {0x00AC,0x01F4,0x0F20}  // Security Key 2
};

typedef enum {
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_CASINO_COINS,
    PKSAV_GBA_ITEM_STORAGE,
    PKSAV_GBA_POKEDEX_SEEN_B
} pksav_gba_section1_field_t;

static const uint16_t pksav_gba_section1_offsets[][4] = {
    {0x0234,0x0234,0x0034}, // Pokémon Party
    {0x0490,0x0490,0x0290}, // Money
    {0x0494,0x0494,0x0294}, // Casino Coins
    {0x0498,0x0498,0x0298}, // Item Storage
    {0x0938,0x0988,0x05F8}  // Pokédex Seen B
};

typedef enum {
    PKSAV_GBA_NAT_POKEDEX_B = 0,
    PKSAV_GBA_NAT_POKEDEX_C
} pksav_gba_section2_field_t;

static const uint16_t pksav_gba_section2_offsets[][4] = {
    {0x03A6,0x0402,0x0068}, // National Pokédex B
    {0x044C,0x04A8,0x011C}  // National Pokédex C
};

typedef enum {
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
} pksav_gba_section4_field_t;

static const uint16_t pksav_gba_section4_offsets[][4] = {
    {0x0C0C,0x0CA4,0x0B98}, // Pokédex Seen C
    {0x0000,0x0000,0x0BCC}  // Rival Name (FR/LG only)
};

bool pksav_buffer_is_gba_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gba_game_t gba_game
) {
    if(buffer_len < PKSAV_GBA_SMALL_SAVE_SIZE) {
        return false;
    }

    /*
     * If the save is not a small save, we need to find the most recent save slot first.
     *
     * Once the proper save slot has been found, it needs to be unshuffled. Sadly, that
     * means more memory allocation.
     */
    const pksav_gba_save_slot_t* sections_pair = (const pksav_gba_save_slot_t*)buffer;
    const pksav_gba_save_slot_t* save_slot;
    if(buffer_len < PKSAV_GBA_SAVE_SIZE) {
        save_slot = sections_pair;
    } else if(SAVE_INDEX(&sections_pair[0]) > SAVE_INDEX(&sections_pair[1])) {
        save_slot = &sections_pair[0];
    } else {
        save_slot = &sections_pair[1];
    }

    pksav_gba_save_slot_t unshuffled;
    uint8_t section_nums[14];
    pksav_gba_save_unshuffle_sections(
        save_slot,
        &unshuffled,
        section_nums
    );

    uint32_t game_code = pksav_littleendian32(SECTION0_DATA32((&unshuffled), gba_game, PKSAV_GBA_GAME_CODE));
    uint32_t security_key1 = pksav_littleendian32(SECURITY_KEY1((&unshuffled), gba_game));
    uint32_t security_key2 = pksav_littleendian32(SECURITY_KEY2((&unshuffled), gba_game));

    if(gba_game == PKSAV_GBA_RS) {
        return (game_code == 0) && (security_key1 == security_key2);
    } else if(gba_game == PKSAV_GBA_FRLG) {
        return (game_code == 1) && (security_key1 == security_key2);
    } else {
        return (security_key1 == security_key2);
    }
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
    size_t filesize = ftell(gba_save);

    if(filesize < PKSAV_GBA_SMALL_SAVE_SIZE) {
        fclose(gba_save);
        return false;
    }

    uint8_t* gba_save_data = malloc(filesize);
    fseek(gba_save, 0, SEEK_SET);
    size_t num_read = fread((void*)gba_save_data, 1, filesize, gba_save);
    fclose(gba_save);

    bool ret = false;
    if(num_read == filesize) {
        ret = pksav_buffer_is_gba_save(
                  gba_save_data,
                  filesize,
                  gba_game
              );
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

    if(gba_save->small_save) {
        most_recent = sections_pair;
        gba_save->from_first_slot = true;
    } else {
        if(SAVE_INDEX(&sections_pair[0]) > SAVE_INDEX(&sections_pair[1])) {
            most_recent = &sections_pair[0];
            gba_save->from_first_slot = true;
        } else {
            most_recent = &sections_pair[1];
            gba_save->from_first_slot = false;
        }
    }

    // Set pointers
    pksav_gba_save_unshuffle_sections(
        most_recent,
        gba_save->unshuffled,
        gba_save->shuffled_section_nums
    );
    gba_save->trainer_info = &gba_save->unshuffled->trainer_info;
    if(gba_save->gba_game == PKSAV_GBA_FRLG) {
        gba_save->rival_name = &SECTION4_DATA8(
                                   gba_save->unshuffled,
                                   gba_save->gba_game,
                                   PKSAV_GBA_FRLG_RIVAL_NAME
                               );
    } else {
        gba_save->rival_name = NULL;
    }
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

    gba_save->casino_coins = &SECTION1_DATA16(
                                 gba_save->unshuffled,
                                 gba_save->gba_game,
                                 PKSAV_GBA_CASINO_COINS
                             );
    *gba_save->casino_coins ^= (uint16_t)(SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game) & 0xFFFF);

    gba_save->pokedex_owned = &SECTION0_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_OWNED
                              );

    gba_save->pokedex_seenA = &SECTION0_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_A
                              );

    gba_save->pokedex_seenB = &SECTION1_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_B
                              );

    gba_save->pokedex_seenC = &SECTION4_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_C
                              );

    if(gba_save->gba_game == PKSAV_GBA_FRLG) {
        gba_save->rse_nat_pokedex_unlockedA = NULL;

        gba_save->frlg_nat_pokedex_unlockedA = &SECTION0_DATA8(
                                                   gba_save->unshuffled,
                                                   gba_save->gba_game,
                                                   PKSAV_GBA_NAT_POKEDEX_A
                                               );
    } else {
        gba_save->rse_nat_pokedex_unlockedA = &SECTION0_DATA16(
                                                  gba_save->unshuffled,
                                                  gba_save->gba_game,
                                                  PKSAV_GBA_NAT_POKEDEX_A
                                              );
        gba_save->frlg_nat_pokedex_unlockedA = NULL;
    }

    gba_save->nat_pokedex_unlockedB = &SECTION2_DATA8(
                                          gba_save->unshuffled,
                                          gba_save->gba_game,
                                          PKSAV_GBA_NAT_POKEDEX_B
                                      );

    gba_save->nat_pokedex_unlockedC = &SECTION2_DATA16(
                                          gba_save->unshuffled,
                                          gba_save->gba_game,
                                          PKSAV_GBA_NAT_POKEDEX_C
                                      );
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
    size_t filesize = ftell(gba_save_file);

    if(filesize < PKSAV_GBA_SMALL_SAVE_SIZE) {
        fclose(gba_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gba_save->raw = malloc(filesize);
    fseek(gba_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gba_save->raw, 1, filesize, gba_save_file);
    fclose(gba_save_file);
    if(num_read != filesize) {
        return PKSAV_ERROR_FILE_IO;
    }

    gba_save->small_save = (filesize < PKSAV_GBA_SAVE_SIZE);

    // Detect what kind of save this is
    bool found = false;
    for(pksav_gba_game_t i = PKSAV_GBA_RS; i <= PKSAV_GBA_FRLG; ++i) {
        if(pksav_buffer_is_gba_save(
               gba_save->raw,
               filesize,
               i
           )
        ) {
            gba_save->gba_game = i;
            found = true;
            break;
        }
    }

    if(!found) {
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
    FILE* gba_save_file = fopen(filepath, "w");
    if(!gba_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    *gba_save->money ^= SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game);
    *gba_save->casino_coins ^= (uint16_t)(SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game) & 0xFFFF);

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

    pksav_set_gba_section_checksums(
        gba_save->unshuffled
    );
    pksav_gba_save_shuffle_sections(
        gba_save->unshuffled,
        save_into,
        gba_save->shuffled_section_nums
    );

    // With everything saved to the new slot, reload it
    _pksav_gba_save_set_pointers(
        gba_save
    );

    fclose(gba_save_file);

    return PKSAV_ERROR_NONE;
}
