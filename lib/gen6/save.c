/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen6/save.h>

#include <pksav/math/endian.h>

#include <assert.h>
#include <stdio.h>

#define PKSAV_GEN6_VALIDATION 0x42454546

/*
 * Offsets
 */

#define PKSAV_GEN6_OFFSET_PTR(gen6_save,offset_enum) \
    (&gen6_save->raw[pksav_gen6_offsets[offset_enum][gen6_save->gen6_game]])

typedef enum {
    PKSAV_GEN6_POKEPUFF_INVENTORY,
    PKSAV_GEN6_ITEM_BAG,
    PKSAV_GEN6_SELECT_BOUNDS_ITEMS,
    PKSAV_GEN6_TRAINER_COORDS1,
    PKSAV_GEN6_TRAINER_COORDS2,
    PKSAV_GEN6_TIME_PLAYED,
    PKSAV_GEN6_ADVENTURE_STARTED_TIME,
    PKSAV_GEN6_TRAINER_INFO, // TODO: more granular
    PKSAV_GEN6_POKEMON_BOX_INFO,
    PKSAV_GEN6_BATTLE_BOX,
    PKSAV_GEN6_TRAINER_CARD,
    PKSAV_GEN6_POKEMON_PARTY,
    PKSAV_GEN6_POKEDEX,
    PKSAV_GEN6_FUSED_ZEKROM_RESHIRAM_STORAGE,
    PKSAV_GEN6_USER_METADATA,
    PKSAV_GEN6_HALL_OF_FAME_DATA,
    PKSAV_GEN6_MAISON_DATA,
    PKSAV_GEN6_DAYCARE_DATA,
    PKSAV_GEN6_BERRY_FIELD_DATA,
    PKSAV_GEN6_SUPER_TRAINING_DATA,
    PKSAV_GEN6_POKEMON_PC,
    PKSAV_GEN6_PICTURE_DATA
} pksav_gen6_offset_field_t;

static const uint32_t pksav_gen6_offsets[][2] =
{
    {0x00005400,0x00005400}, // Poképuff Inventory
    {0x00005800,0x00005800}, // Item bag
    {0x00006400,0x00006400}, // Select-bound items
    {0x00006802,0x00006802}, // Trainer coordinates 1
    {0x000068F4,0x000068F4}, // Trainer coordinates 2
    {0x00006C00,0x00006C00}, // Time played
    {0x00006C04,0x00006C04}, // Adventure started time
    {0x00009600,0x00009600}, // Trainer information
    {0x00009800,0x00009800}, // Box info
    {0x00009E00,0x00009E00}, // Battle box
    {0x00019400,0x00019400}, // Trainer card
    {0x00019600,0x00019600}, // Pokémon party
    {0x0001A400,0x0001A400}, // Pokédex
    {0x0001B400,0x0001BE00}, // Fused Zekrom/Reshiram storage
    {0x0001C400,0x0001CE00}, // User metadata
    {0x0001E800,0x0001F200}, // Hall of Fame data
    {0x00020400,0x00020E00}, // Battle Maison data
    {0x00020600,0x00021000}, // Daycare data
    {0x00020C00,0x00021800}, // Berry Field data
    {0x00024600,0x00025600}, // Super Training data
    {0x00027A00,0x00038400}, // Box data
    {0x0005C600,0x0006D000}, // Picture data
};

pksav_error_t pksav_buffer_is_gen6_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gen6_game_t gen6_game,
    bool* result_out
)
{
    if(!buffer || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // TODO: enforce enum

    size_t expected_size = (gen6_game == PKSAV_GEN6_XY) ? PKSAV_GEN6_XY_SAVE_SIZE
                                                        : PKSAV_GEN6_ORAS_SAVE_SIZE;
    if(buffer_len != expected_size)
    {
        *result_out = false;
    }
    else if(*((uint32_t*)&buffer[buffer_len - 0x1F0]) != PKSAV_GEN6_VALIDATION)
    {
        *result_out = false;
    }
    else
    {
        *result_out = true;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_file_is_gen6_save(
    const char* filepath,
    pksav_gen6_game_t gen6_game,
    bool* result_out
)
{
    if(!filepath || !result_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    FILE* gen6_save_file = fopen(filepath, "rb");
    if(!gen6_save_file)
    {
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    fseek(gen6_save_file, 0, SEEK_END);
    size_t filesize = ftell(gen6_save_file);

    size_t expected_size = (gen6_game == PKSAV_GEN6_XY) ? PKSAV_GEN6_XY_SAVE_SIZE
                                                        : PKSAV_GEN6_ORAS_SAVE_SIZE;
    if(filesize != expected_size)
    {
        fclose(gen6_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    uint8_t* gen6_save_data = malloc(filesize);
    fseek(gen6_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gen6_save_data, 1, filesize, gen6_save_file);
    fclose(gen6_save_file);

    bool ret = false;
    if(num_read == filesize) {
        pksav_buffer_is_gen6_save(
            gen6_save_data,
            filesize,
            gen6_game,
            &ret
        );
    }

    free(gen6_save_data);
    *result_out = ret;
    return PKSAV_ERROR_NONE;
}

/*
    PKSAV_GEN6_POKEPUFF_INVENTORY,
    PKSAV_GEN6_ITEM_BAG,
    PKSAV_GEN6_SELECT_BOUNDS_ITEMS,
    PKSAV_GEN6_TRAINER_COORDS1,
    PKSAV_GEN6_TRAINER_COORDS2,
    PKSAV_GEN6_TIME_PLAYED,
    PKSAV_GEN6_ADVENTURE_STARTED_TIME,
    PKSAV_GEN6_TRAINER_INFO, // TODO: more granular
    PKSAV_GEN6_POKEMON_BOX_INFO,
    PKSAV_GEN6_BATTLE_BOX,
    PKSAV_GEN6_TRAINER_CARD,
    PKSAV_GEN6_POKEMON_PARTY,
    PKSAV_GEN6_POKEDEX,
    PKSAV_GEN6_FUSED_ZEKROM_RESHIRAM_STORAGE,
    PKSAV_GEN6_USER_METADATA,
    PKSAV_GEN6_HALL_OF_FAME_DATA,
    PKSAV_GEN6_MAISON_DATA,
    PKSAV_GEN6_DAYCARE_DATA,
    PKSAV_GEN6_BERRY_FIELD_DATA,
    PKSAV_GEN6_SUPER_TRAINING_DATA,
    PKSAV_GEN6_POKEMON_PC,
    PKSAV_GEN6_PICTURE_DATA
 */

static void _pksav_gen6_set_pointers(
    pksav_gen6_save_t* gen6_save_out
)
{
    assert(gen6_save_out);

    gen6_save_out->pokemon_box_info = (pksav_gen6_pokemon_box_info_t*)(
                                          PKSAV_GEN6_OFFSET_PTR(
                                              gen6_save_out,
                                              PKSAV_GEN6_POKEMON_BOX_INFO
                                          )
                                      );
    gen6_save_out->trainer_card = (pksav_gen6_trainer_card_t*)(
                                      PKSAV_GEN6_OFFSET_PTR(
                                          gen6_save_out,
                                          PKSAV_GEN6_TRAINER_CARD
                                      )
                                  );
    gen6_save_out->pokemon_party = (pksav_gen6_pokemon_party_t*)(
                                       PKSAV_GEN6_OFFSET_PTR(
                                           gen6_save_out,
                                           PKSAV_GEN6_POKEMON_PARTY
                                       )
                                   );
    gen6_save_out->pokemon_pc = (pksav_gen6_pokemon_pc_t*)(
                                    PKSAV_GEN6_OFFSET_PTR(
                                        gen6_save_out,
                                        PKSAV_GEN6_POKEMON_PC
                                    )
                                );
    gen6_save_out->item_bag = (pksav_gen6_bag_t*)(
                                  PKSAV_GEN6_OFFSET_PTR(
                                      gen6_save_out,
                                      PKSAV_GEN6_ITEM_BAG
                                  )
                              );
}

pksav_error_t pksav_gen6_save_load(
    const char* filepath,
    pksav_gen6_save_t* gen6_save_out
)
{
    if(!filepath || !gen6_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Read the file and make sure it's valid
    FILE* gen6_save_file = fopen(filepath, "rb");
    if(!gen6_save_file)
    {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen6_save_file, 0, SEEK_END);
    size_t filesize = ftell(gen6_save_file);

    if(filesize == PKSAV_GEN6_XY_SAVE_SIZE)
    {
        gen6_save_out->gen6_game = PKSAV_GEN6_XY;
    }
    else if(filesize == PKSAV_GEN6_ORAS_SAVE_SIZE)
    {
        gen6_save_out->gen6_game = PKSAV_GEN6_ORAS;
    }
    else
    {
        fclose(gen6_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen6_save_out->raw = malloc(filesize);
    fseek(gen6_save_file, 0, SEEK_SET);
    size_t num_read = fread(
                          (void*)gen6_save_out->raw,
                          1,
                          filesize,
                          gen6_save_file
                      );
    fclose(gen6_save_file);
    if(num_read != filesize)
    {
        free(gen6_save_out->raw);
        return PKSAV_ERROR_FILE_IO;
    }

    _pksav_gen6_set_pointers(gen6_save_out);

    (void)pksav_gen6_offsets;

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen6_save_save(
    const char* filepath,
    pksav_gen6_save_t* gen6_save
)
{
    if(!filepath || !gen6_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Make sure we can write to this file
    FILE* gen6_save_file = fopen(filepath, "wb");
    if(!gen6_save_file)
    {
        return PKSAV_ERROR_FILE_IO;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen6_save_free(
    pksav_gen6_save_t* gen6_save
)
{
    if(!gen6_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    free(gen6_save->raw);

    return PKSAV_ERROR_NONE;
}
