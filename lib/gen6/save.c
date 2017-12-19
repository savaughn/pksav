/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen6/save.h>

#include <pksav/math/endian.h>

#include <stdio.h>

/*
 * Offsets (TODO)
 */

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

static const uint16_t pksav_gen6_offsets[][2] =
{
    {0,0}
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

    if(buffer_len < PKSAV_GEN6_SAVE_SIZE)
    {
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    (void)gen6_game;

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

    FILE* gen6_save = fopen(filepath, "rb");
    if(!gen6_save)
    {
        *result_out = false;
        return PKSAV_ERROR_NONE;
    }

    fseek(gen6_save, 0, SEEK_END);
    size_t filesize = ftell(gen6_save);

    if(filesize < PKSAV_GEN6_SAVE_SIZE)
    {
        fclose(gen6_save);
        return false;
    }

    uint8_t* gen6_save_data = malloc(filesize);
    fseek(gen6_save, 0, SEEK_SET);
    size_t num_read = fread((void*)gen6_save_data, 1, filesize, gen6_save);
    fclose(gen6_save);

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

    if(filesize < PKSAV_GEN6_SAVE_SIZE)
    {
        fclose(gen6_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen6_save_out->raw = malloc(filesize);
    fseek(gen6_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gen6_save_out->raw, 1, filesize, gen6_save_file);
    fclose(gen6_save_file);
    if(num_read != filesize)
    {
        free(gen6_save_out->raw);
        return PKSAV_ERROR_FILE_IO;
    }

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
