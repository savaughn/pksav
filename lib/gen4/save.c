/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../common/nds_checksum.h"

#include <pksav/gen4/save.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PKSAV_GEN4_SMALL_SAVE_SIZE 0x40000
#define PKSAV_GEN4_LARGE_SAVE_SIZE 0x80000

typedef enum {
    PKSAV_GEN4_SAVE_A = 0x00000,
    PKSAV_GEN4_SAVE_B = 0x40000
} pksav_gen4_save_offset_t;

typedef enum {
    PKSAV_GEN4_GENERAL_BLOCK_START = 0,
    PKSAV_GEN4_GENERAL_BLOCK_LENGTH,
    PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
    PKSAV_GEN4_STORAGE_BLOCK_START,
    PKSAV_GEN4_STORAGE_BLOCK_LENGTH,
    PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START
} pksav_gen4_block_info_field_t;

static const uint32_t pksav_gen4_block_info[][3] = {
    {0x00000,0x00000,0x00000}, // General block start
    {0x0C100,0x0CF2C,0x0F628}, // General block length
    {0x0CE0C,0x0CF18,0x0F618}, // General block footer offset
    {0x0C100,0x0CF2C,0x0F700}, // Storage block start
    {0x121E0,0x121E4,0x12310}, // Storage block length
    {0x1E2CC,0x1F0Fc,0x21A00}  // Storage block footer offset
};

typedef enum {
    PKSAV_GEN4_ADVENTURE_STARTED_TIME = 0,
    PKSAV_GEN4_POKEMON_LEAGUE_CHAMP_TIME,
    PKSAV_GEN4_TRAINER_NAME,
    PKSAV_GEN4_TRAINER_ID,
    PKSAV_GEN4_TRAINER_MONEY,
    PKSAV_GEN4_TRAINER_GENDER,
    PKSAV_GEN4_LANGUAGE,
    PKSAV_GEN4_BADGES,
    PKSAV_GEN4_HGSS_KANTO_BADGES,
    PKSAV_GEN4_TOTAL_PLAYTIME,
    PKSAV_GEN4_AVATAR,
    PKSAV_GEN4_PARTY,
    PKSAV_GEN4_ITEM_BAG,
    PKSAV_GEN4_RIVAL_NAME,
    PKSAV_GEN4_TRAINER_SIGNATURE,
} pksav_gen4_field_t;

/*
 * TODO: confirm Platinum offsets, probably in similar offsets,
 *       relative to others
 */
static const uint16_t pksav_gen4_offsets[][3] = {
    {0x0034,0x0034,0x0034}, // Adventure Started Time
    {0x003C,0x003C,0x003C}, // Pokémon League Start Time
    {0x0064,0x0068,0x0064}, // Trainer Name
    {0x0074,0x0078,0x0074}, // Trainer ID
    {0x0078,0x007C,0x0078}, // Money
    {0x007C,0x0080,0x007C}, // Trainer Gender
    {0x007D,0x0081,0x007D}, // Language
    {0x007E,0x0082,0x007E}, // Badges
    {0x007E,0x0082,0x0083}, // Kanto Badges (HGSS only)
    {0x0086,0x0086,0x0086}, // Total playtime
    {0x007F,0x0083,0x007F}, // Multiplayer Avatar
    {0x0098,0x00A0,0x0098}, // Party
    {0x0624,0x0630,0x0644}, // Item Bag
    {0x25A8,0x27E8,0x22D4}, // Rival Name
    {0x59A4,0x5BA8,0x4538}, // Trainer Signature
};

#define GEN4_BLOCK_INFO(field,game) pksav_gen4_block_info[field][game]
#define GEN4_OFFSET(field,game)     pksav_gen4_offsets[field][game]

#define GEN4_BLOCK_INFO_DATA(field,game,raw) raw[GEN4_BLOCK_INFO(field,game)]
#define GEN4_OFFSET_DATA(field,game,raw) raw[GEN4_OFFSET(field,game)]

static bool _pksav_file_is_gen4_save(
    const uint8_t* data,
    pksav_gen4_game_t gen4_game
) {
    static const uint16_t gen4_offsets[] = {0xC0F4,0xCF20,0xF61C};

    return (*(const uint32_t*)&data[gen4_offsets[gen4_game]] == GEN4_BLOCK_INFO(PKSAV_GEN4_GENERAL_BLOCK_LENGTH,gen4_game));
}

bool pksav_file_is_gen4_save(
    const char* filepath,
    pksav_gen4_game_t gen4_game
) {
    FILE* gen4_save = fopen(filepath, "r");
    if(!gen4_save) {
        return false;
    }

    fseek(gen4_save, SEEK_END, 0);
    size_t save_size = ftell(gen4_save);

    if(save_size < PKSAV_GEN4_SMALL_SAVE_SIZE) {
        fclose(gen4_save);
        return false;
    }

    uint8_t* gen4_save_data = malloc(save_size);
    fseek(gen4_save, SEEK_SET, 0);
    size_t num_read = fread((void*)gen4_save_data, 1, save_size, gen4_save);
    fclose(gen4_save);

    bool ret = false;
    if(num_read == save_size) {
        ret = _pksav_file_is_gen4_save(gen4_save_data, gen4_game);
    }

    free(gen4_save_data);
    return ret;
}

static void _pksav_gen4_save_set_block_pointers(
    pksav_gen4_save_t* gen4_save
) {
    /*
     * By default, set the pointers to the first half of a large file, which
     * works for both kinds. For large saves, check the second half and advance
     * it if necessary.
     */

    gen4_save->general_block  = &GEN4_BLOCK_INFO_DATA(
                                    PKSAV_GEN4_GENERAL_BLOCK_START,
                                    gen4_save->gen4_game,
                                    gen4_save->raw
                                );
    gen4_save->general_footer = (pksav_gen4_footer_t*)(
                                    &GEN4_BLOCK_INFO_DATA(
                                        PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
                                        gen4_save->gen4_game,
                                        gen4_save->raw
                                    )
                                );

    gen4_save->storage_block  = &GEN4_BLOCK_INFO_DATA(
                                    PKSAV_GEN4_STORAGE_BLOCK_START,
                                    gen4_save->gen4_game,
                                    gen4_save->raw
                                );
    gen4_save->storage_footer = (pksav_gen4_footer_t*)(
                                    &GEN4_BLOCK_INFO_DATA(
                                        PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START,
                                        gen4_save->gen4_game,
                                        gen4_save->raw
                                    )
                                );

    if(!gen4_save->small_save) {
        pksav_gen4_footer_t* general_footer1 = (pksav_gen4_footer_t*)(
                                                   &GEN4_BLOCK_INFO_DATA(
                                                       PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
                                                       gen4_save->gen4_game,
                                                       (gen4_save->raw + PKSAV_GEN4_SMALL_SAVE_SIZE)
                                                   )
                                               );
        if(gen4_save->gen4_game == PKSAV_GEN4_HGSS) {
            if(gen4_save->general_footer->hgss.save_index < general_footer1->hgss.save_index) {
                gen4_save->general_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->general_footer = general_footer1;
            }
        } else {
            if(gen4_save->general_footer->dppt.general_id < general_footer1->dppt.general_id) {
                gen4_save->general_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->general_footer = general_footer1;
            }
        }

        pksav_gen4_footer_t* storage_footer1 = (pksav_gen4_footer_t*)(
                                                   &GEN4_BLOCK_INFO_DATA(
                                                       PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START,
                                                       gen4_save->gen4_game,
                                                       (gen4_save->raw + PKSAV_GEN4_SMALL_SAVE_SIZE)
                                                   )
                                               );
        if(gen4_save->gen4_game == PKSAV_GEN4_HGSS) {
            if(gen4_save->storage_footer->hgss.save_index < storage_footer1->hgss.save_index) {
                gen4_save->storage_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->storage_footer = storage_footer1;
            }
        } else {
            if(gen4_save->storage_footer->dppt.storage_id < storage_footer1->dppt.storage_id) {
                gen4_save->storage_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->storage_footer = storage_footer1;
            }
        }
    }
}

static void _pksav_gen4_save_set_public_pointers(
    pksav_gen4_save_t* gen4_save
) {
    gen4_save->pokemon_party = (pksav_gen4_pokemon_party_t*)(
                                   &GEN4_OFFSET_DATA(
                                       PKSAV_GEN4_PARTY,
                                       gen4_save->gen4_game,
                                       gen4_save->general_block
                                   )
                               );

    // TODO: PC

    gen4_save->item_bag = (pksav_gen4_item_bag_t*)(
                              &GEN4_OFFSET_DATA(
                                  PKSAV_GEN4_ITEM_BAG,
                                  gen4_save->gen4_game,
                                  gen4_save->general_block
                              )
                          );

    gen4_save->trainer_name = (uint16_t*)(
                                  &GEN4_OFFSET_DATA(
                                      PKSAV_GEN4_TRAINER_NAME,
                                      gen4_save->gen4_game,
                                      gen4_save->general_block
                                  )
                              );

    gen4_save->trainer_id = (pksav_trainer_id_t*)(
                                &GEN4_OFFSET_DATA(
                                    PKSAV_GEN4_TRAINER_ID,
                                    gen4_save->gen4_game,
                                    gen4_save->general_block
                                )
                            );

    gen4_save->trainer_gender = &GEN4_OFFSET_DATA(
                                    PKSAV_GEN4_TRAINER_GENDER,
                                    gen4_save->gen4_game,
                                    gen4_save->general_block
                                );

    gen4_save->money = (uint32_t*)(
                           &GEN4_OFFSET_DATA(
                               PKSAV_GEN4_TRAINER_MONEY,
                               gen4_save->gen4_game,
                               gen4_save->general_block
                           )
                       );

    gen4_save->rival_name = (uint16_t*)(
                                &GEN4_OFFSET_DATA(
                                    PKSAV_GEN4_RIVAL_NAME,
                                    gen4_save->gen4_game,
                                    gen4_save->general_block
                                )
                            );

    gen4_save->adventure_started_time = (uint32_t*)(
                                            &GEN4_OFFSET_DATA(
                                                PKSAV_GEN4_ADVENTURE_STARTED_TIME,
                                                gen4_save->gen4_game,
                                                gen4_save->general_block
                                            )
                                        );

    gen4_save->pokemon_league_champ_time = (uint32_t*)(
                                               &GEN4_OFFSET_DATA(
                                                   PKSAV_GEN4_POKEMON_LEAGUE_CHAMP_TIME,
                                                   gen4_save->gen4_game,
                                                   gen4_save->general_block
                                               )
                                           );

    gen4_save->total_playtime = (pksav_date_t*)(
                                    &GEN4_OFFSET_DATA(
                                        PKSAV_GEN4_ADVENTURE_STARTED_TIME,
                                        gen4_save->gen4_game,
                                        gen4_save->general_block
                                    )
                                );

    gen4_save->sinnoh_johto_badges = &GEN4_OFFSET_DATA(
                                         PKSAV_GEN4_BADGES,
                                         gen4_save->gen4_game,
                                         gen4_save->general_block
                                     );

    if(gen4_save->gen4_game == PKSAV_GEN4_HGSS) {
        gen4_save->hgss_kanto_badges = &GEN4_OFFSET_DATA(
                                           PKSAV_GEN4_HGSS_KANTO_BADGES,
                                           gen4_save->gen4_game,
                                           gen4_save->general_block
                                       );
    } else {
        gen4_save->hgss_kanto_badges = NULL;
    }
}

pksav_error_t pksav_gen4_save_load(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
) {
    // Read the file and make sure it's valid
    FILE* gen4_save_file = fopen(filepath, "r");
    if(!gen4_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen4_save_file, SEEK_END, 0);
    size_t save_size = ftell(gen4_save_file);

    if(save_size >= PKSAV_GEN4_LARGE_SAVE_SIZE) {
        gen4_save->small_save = false;
    } else if(save_size >= PKSAV_GEN4_SMALL_SAVE_SIZE) {
        gen4_save->small_save = true;
    } else {
        fclose(gen4_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen4_save->raw = malloc(save_size);
    fseek(gen4_save_file, SEEK_SET, 0);
    size_t num_read = fread((void*)gen4_save->raw, 1, save_size, gen4_save_file);
    fclose(gen4_save_file);
    if(num_read != save_size) {
        free(gen4_save->raw);
        return PKSAV_ERROR_FILE_IO;
    }

    if(_pksav_file_is_gen4_save(gen4_save->raw, PKSAV_GEN4_DP)) {
        gen4_save->gen4_game = PKSAV_GEN4_DP;
    } else if(_pksav_file_is_gen4_save(gen4_save->raw, PKSAV_GEN4_PLATINUM)) {
        gen4_save->gen4_game = PKSAV_GEN4_PLATINUM;
    } else if(_pksav_file_is_gen4_save(gen4_save->raw, PKSAV_GEN4_HGSS)) {
        gen4_save->gen4_game = PKSAV_GEN4_HGSS;
    } else {
        free(gen4_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    _pksav_gen4_save_set_block_pointers(
        gen4_save
    );
    _pksav_gen4_save_set_public_pointers(
        gen4_save
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen4_save_save(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
) {
    // Make sure we can write to this file
    FILE* gen4_save_file = fopen(filepath, "w");
    if(!gen4_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    // Write to file
    size_t save_size = gen4_save->small_save ? PKSAV_GEN4_SMALL_SAVE_SIZE
                                             : PKSAV_GEN4_LARGE_SAVE_SIZE;
    fwrite((void*)gen4_save->raw, 1, save_size, gen4_save_file);
    fclose(gen4_save_file);

    return PKSAV_ERROR_NONE;
}
