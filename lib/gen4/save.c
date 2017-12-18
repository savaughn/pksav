/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../common/nds_common.h"

#include <pksav/gen4/save.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PKSAV_GEN4_SMALL_SAVE_SIZE 0x40000
#define PKSAV_GEN4_LARGE_SAVE_SIZE 0x80000

typedef enum
{
    PKSAV_GEN4_SAVE_A = 0x00000,
    PKSAV_GEN4_SAVE_B = 0x40000
} pksav_gen4_save_offset_t;

typedef enum
{
    PKSAV_GEN4_GENERAL_BLOCK_START = 0,
    PKSAV_GEN4_GENERAL_BLOCK_LENGTH,
    PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
    PKSAV_GEN4_STORAGE_BLOCK_START,
    PKSAV_GEN4_STORAGE_BLOCK_LENGTH,
    PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START
} pksav_gen4_block_info_field_t;

static const uint32_t pksav_gen4_block_info[][3] =
{
    {0x00000,0x00000,0x00000}, // General block start
    {0x0C100,0x0CF2C,0x0F628}, // General block length
    {0x0C0EC,0x0CF18,0x0F618}, // General block footer offset
    {0x0C100,0x0CF2C,0x0F700}, // Storage block start
    {0x121E0,0x121E4,0x12310}, // Storage block length
    {0x1E2CC,0x1F0FC,0x21A00}  // Storage block footer offset
};

typedef enum
{
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
    PKSAV_GEN4_PLAYER_COORDINATES,
    PKSAV_GEN4_HGSS_FOLLOWER_COORDINATES,
    PKSAV_GEN4_TRAINER_SIGNATURE,
} pksav_gen4_field_t;

/*
 * TODO: confirm Platinum offsets, probably in similar offsets,
 *       relative to others
 */
static const uint32_t pksav_gen4_offsets[][3] =
{
    {0x0034,0x0034,0x00034}, // Adventure Started Time
    {0x003C,0x003C,0x0003C}, // Pokémon League Start Time
    {0x0064,0x0068,0x00064}, // Trainer Name
    {0x0074,0x0078,0x00074}, // Trainer ID
    {0x0078,0x007C,0x00078}, // Money
    {0x007C,0x0080,0x0007C}, // Trainer Gender
    {0x007D,0x0081,0x0007D}, // Language
    {0x007E,0x0082,0x0007E}, // Badges
    {0x007E,0x0082,0x00083}, // Kanto Badges (HGSS only)
    {0x0086,0x0086,0x00086}, // Total playtime
    {0x007F,0x0083,0x0007F}, // Multiplayer Avatar
    {0x0094,0x009C,0x00094}, // Party
    {0x0624,0x0630,0x00644}, // Item Bag
    {0x25A8,0x27E8,0x022D4}, // Rival Name
    {0x25FA,0x0000,0x0236E}, // Player Coordinates (TODO: figure out Platinum's)
    {0x0000,0x0000,0x023BE}, // Follower Coordinates (HGSS only)
    {0x5904,0x5BA8,0x1C100}, // Trainer Signature
};

#define GEN4_BLOCK_INFO(field,game) pksav_gen4_block_info[field][game]
#define GEN4_OFFSET(field,game)     pksav_gen4_offsets[field][game]

#define GEN4_BLOCK_INFO_DATA(field,game,raw) raw[GEN4_BLOCK_INFO(field,game)]
#define GEN4_OFFSET_DATA(field,game,raw)     raw[GEN4_OFFSET(field,game)]

pksav_error_t pksav_buffer_is_gen4_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gen4_game_t gen4_game,
    bool* result_out
)
{
    if(!buffer || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(buffer_len < PKSAV_GEN4_SMALL_SAVE_SIZE)
    {
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    static const uint16_t gen4_offsets[] = {0xC0F4,0xCF20,0xF61C};
    *result_out = (*(const uint32_t*)&buffer[gen4_offsets[gen4_game]] == GEN4_BLOCK_INFO(PKSAV_GEN4_GENERAL_BLOCK_LENGTH,gen4_game));
    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_file_is_gen4_save(
    const char* filepath,
    pksav_gen4_game_t gen4_game,
    bool* result_out
)
{
    if(!filepath || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    FILE* gen4_save = fopen(filepath, "rb");
    if(!gen4_save)
    {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen4_save, 0, SEEK_END);
    size_t save_size = ftell(gen4_save);

    if(save_size < PKSAV_GEN4_SMALL_SAVE_SIZE)
    {
        fclose(gen4_save);
        *result_out = false;

        return PKSAV_ERROR_NONE;
    }

    uint8_t* gen4_save_data = malloc(save_size);
    fseek(gen4_save, 0, SEEK_SET);
    size_t num_read = fread((void*)gen4_save_data, 1, save_size, gen4_save);
    fclose(gen4_save);

    bool ret = false;
    if(num_read == save_size)
    {
        pksav_buffer_is_gen4_save(
            gen4_save_data,
            save_size,
            gen4_game,
            &ret
        );
    }

    free(gen4_save_data);
    *result_out = ret;
    return PKSAV_ERROR_NONE;
}

static void _pksav_gen4_save_set_block_pointers(
    pksav_gen4_save_t* gen4_save
)
{
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

    if(!gen4_save->small_save)
    {
        pksav_gen4_footer_t* general_footer2 = (pksav_gen4_footer_t*)(
                                                   &GEN4_BLOCK_INFO_DATA(
                                                       PKSAV_GEN4_GENERAL_BLOCK_FOOTER_START,
                                                       gen4_save->gen4_game,
                                                       (gen4_save->raw + PKSAV_GEN4_SMALL_SAVE_SIZE)
                                                   )
                                               );
        if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
        {
            if(gen4_save->general_footer->hgss.save_index < general_footer2->hgss.save_index)
            {
                gen4_save->general_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->general_footer = general_footer2;
            }
        }
        else
        {
            if(gen4_save->general_footer->dppt.general_id < general_footer2->dppt.general_id)
            {
                gen4_save->general_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->general_footer = general_footer2;
            }
        }

        pksav_gen4_footer_t* storage_footer2 = (pksav_gen4_footer_t*)(
                                                   &GEN4_BLOCK_INFO_DATA(
                                                       PKSAV_GEN4_STORAGE_BLOCK_FOOTER_START,
                                                       gen4_save->gen4_game,
                                                       (gen4_save->raw + PKSAV_GEN4_SMALL_SAVE_SIZE)
                                                   )
                                               );
        if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
        {
            if(gen4_save->storage_footer->hgss.save_index < storage_footer2->hgss.save_index)
            {
                gen4_save->storage_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->storage_footer = storage_footer2;
            }
        }
        else
        {
            if(gen4_save->storage_footer->dppt.storage_id < storage_footer2->dppt.storage_id)
            {
                gen4_save->storage_block += PKSAV_GEN4_SMALL_SAVE_SIZE;
                gen4_save->storage_footer = storage_footer2;
            }
        }
    }
}

static void _pksav_gen4_save_set_public_pointers(
    pksav_gen4_save_t* gen4_save
)
{
    gen4_save->pokemon_party = (pksav_gen4_pokemon_party_t*)(
                                   &GEN4_OFFSET_DATA(
                                       PKSAV_GEN4_PARTY,
                                       gen4_save->gen4_game,
                                       gen4_save->general_block
                                   )
                               );

    gen4_save->pokemon_pc = (pksav_gen4_pokemon_pc_t*)(
                                gen4_save->storage_block
                            );

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

    gen4_save->player_coordinates = (pksav_coordinates_t*)(
                                        &GEN4_OFFSET_DATA(
                                            PKSAV_GEN4_PLAYER_COORDINATES,
                                            gen4_save->gen4_game,
                                            gen4_save->general_block
                                        )
                                    );

    if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
    {
        gen4_save->hgss_follower_coordinates = (pksav_coordinates_t*)(
                                                   &GEN4_OFFSET_DATA(
                                                       PKSAV_GEN4_HGSS_FOLLOWER_COORDINATES,
                                                       gen4_save->gen4_game,
                                                       gen4_save->general_block
                                                   )
                                               );
    }
    else
    {
        gen4_save->hgss_follower_coordinates = NULL;
    }

    gen4_save->sinnoh_johto_badges = &GEN4_OFFSET_DATA(
                                         PKSAV_GEN4_BADGES,
                                         gen4_save->gen4_game,
                                         gen4_save->general_block
                                     );

    if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
    {
        gen4_save->hgss_kanto_badges = &GEN4_OFFSET_DATA(
                                           PKSAV_GEN4_HGSS_KANTO_BADGES,
                                           gen4_save->gen4_game,
                                           gen4_save->general_block
                                       );
    }
    else
    {
        gen4_save->hgss_kanto_badges = NULL;
    }

    gen4_save->trainer_card_signature = (pksav_trainer_card_signature_t*)(
                                            &GEN4_OFFSET_DATA(
                                                PKSAV_GEN4_TRAINER_SIGNATURE,
                                                gen4_save->gen4_game,
                                                gen4_save->general_block
                                            )
                                        );
}

static void _pksav_gen4_save_set_block_checksums(
    pksav_gen4_save_t* gen4_save
)
{
    if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
    {
        gen4_save->general_footer->hgss.checksum = pksav_nds_block_get_checksum(
                                                       gen4_save->general_block,
                                                       GEN4_BLOCK_INFO(
                                                           PKSAV_GEN4_GENERAL_BLOCK_LENGTH,
                                                           gen4_save->gen4_game
                                                       )
                                                   );
        gen4_save->storage_footer->hgss.checksum = pksav_nds_block_get_checksum(
                                                       gen4_save->storage_block,
                                                       GEN4_BLOCK_INFO(
                                                           PKSAV_GEN4_STORAGE_BLOCK_LENGTH,
                                                           gen4_save->gen4_game
                                                       )
                                                   );
    }
    else
    {
        gen4_save->general_footer->dppt.checksum = pksav_nds_block_get_checksum(
                                                       gen4_save->general_block,
                                                       GEN4_BLOCK_INFO(
                                                           PKSAV_GEN4_GENERAL_BLOCK_LENGTH,
                                                           gen4_save->gen4_game
                                                       )
                                                   );
        gen4_save->storage_footer->dppt.checksum = pksav_nds_block_get_checksum(
                                                       gen4_save->storage_block,
                                                       GEN4_BLOCK_INFO(
                                                           PKSAV_GEN4_STORAGE_BLOCK_LENGTH,
                                                           gen4_save->gen4_game
                                                       )
                                                   );
    }
}

static void _pksav_gen4_crypt_all_pokemon(
    pksav_gen4_save_t* gen4_save,
    bool encrypt
)
{
    // Party
    for(uint8_t i = 0; i < 6; ++i)
    {
        if(encrypt)
        {
            pksav_nds_pokemon_set_checksum(
                &gen4_save->pokemon_party->party[i].pc
            );
        }
        pksav_nds_crypt_pokemon(
            &gen4_save->pokemon_party->party[i].pc,
            encrypt
        );
    }

    // PC
    for(uint8_t i = 0; i < 18; ++i)
    {
        for(uint8_t j = 0; j < 30; ++j)
        {
            if(gen4_save->gen4_game == PKSAV_GEN4_HGSS)
            {
                if(encrypt)
                {
                    pksav_nds_pokemon_set_checksum(
                        &gen4_save->pokemon_pc->hgss.boxes[i].entries[j]
                    );
                }
                pksav_nds_crypt_pokemon(
                    &gen4_save->pokemon_pc->hgss.boxes[i].entries[j],
                    encrypt
                );
            }
            else
            {
                if(encrypt)
                {
                    pksav_nds_pokemon_set_checksum(
                        &gen4_save->pokemon_pc->dppt.boxes[i].entries[j]
                    );
                }
                pksav_nds_crypt_pokemon(
                    &gen4_save->pokemon_pc->dppt.boxes[i].entries[j],
                    encrypt
                );
            }
        }
    }
}

pksav_error_t pksav_gen4_save_load(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
)
{
    if(!filepath || !gen4_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Read the file and make sure it's valid
    FILE* gen4_save_file = fopen(filepath, "rb");
    if(!gen4_save_file)
    {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen4_save_file, 0, SEEK_END);
    size_t save_size = ftell(gen4_save_file);

    if(save_size >= PKSAV_GEN4_LARGE_SAVE_SIZE)
    {
        gen4_save->small_save = false;
    }
    else if(save_size >= PKSAV_GEN4_SMALL_SAVE_SIZE)
    {
        gen4_save->small_save = true;
    }
    else
    {
        fclose(gen4_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen4_save->raw = malloc(save_size);
    fseek(gen4_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gen4_save->raw, 1, save_size, gen4_save_file);
    fclose(gen4_save_file);
    if(num_read != save_size)
    {
        free(gen4_save->raw);
        return PKSAV_ERROR_FILE_IO;
    }

    // Detect what type of save this is.
    bool found = false;
    for(pksav_gen4_game_t i = PKSAV_GEN4_DP; i <= PKSAV_GEN4_HGSS; ++i)
    {
        pksav_buffer_is_gen4_save(
            gen4_save->raw,
            save_size,
            i,
            &found
        );
        if(found)
        {
            gen4_save->gen4_game = i;
            break;
        }
    }

    if(!found)
    {
        free(gen4_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    _pksav_gen4_save_set_block_pointers(
        gen4_save
    );
    _pksav_gen4_save_set_public_pointers(
        gen4_save
    );
    _pksav_gen4_crypt_all_pokemon(
        gen4_save,
        false
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen4_save_save(
    const char* filepath,
    pksav_gen4_save_t* gen4_save
)
{
    if(!filepath || !gen4_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Make sure we can write to this file
    FILE* gen4_save_file = fopen(filepath, "wb");
    if(!gen4_save_file)
    {
        return PKSAV_ERROR_FILE_IO;
    }

    // Encrypt all Pokémon
    _pksav_gen4_crypt_all_pokemon(
        gen4_save,
        true
    );

    // Set save checksums
    _pksav_gen4_save_set_block_checksums(gen4_save);

    // Write to file
    size_t save_size = gen4_save->small_save ? PKSAV_GEN4_SMALL_SAVE_SIZE
                                             : PKSAV_GEN4_LARGE_SAVE_SIZE;
    fwrite((void*)gen4_save->raw, 1, save_size, gen4_save_file);
    fclose(gen4_save_file);

    // Decrypt all Pokémon again
    _pksav_gen4_crypt_all_pokemon(
        gen4_save,
        false
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen4_save_free(
    pksav_gen4_save_t* gen4_save
)
{
    if(!gen4_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    free(gen4_save->raw);
    gen4_save->pokemon_party = NULL;
    gen4_save->pokemon_pc = NULL;
    gen4_save->item_bag = NULL;
    gen4_save->trainer_name = NULL;
    gen4_save->trainer_id = NULL;
    gen4_save->trainer_gender = NULL;
    gen4_save->money = NULL;
    gen4_save->rival_name = NULL;
    gen4_save->adventure_started_time = NULL;
    gen4_save->pokemon_league_champ_time = NULL;
    gen4_save->total_playtime = NULL;
    gen4_save->player_coordinates = NULL;
    gen4_save->hgss_follower_coordinates = NULL;
    gen4_save->trainer_card_signature = NULL;
    gen4_save->sinnoh_johto_badges = NULL;
    gen4_save->hgss_kanto_badges = NULL;
    gen4_save->general_block = NULL;
    gen4_save->general_footer = NULL;
    gen4_save->storage_block = NULL;
    gen4_save->storage_footer = NULL;
    gen4_save->raw = NULL;

    return PKSAV_ERROR_NONE;
}
