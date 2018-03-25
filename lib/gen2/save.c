/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/fs.h"

#include <pksav/gen2/save.h>

#include <pksav/math/endian.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define PKSAV_GS_CHECKSUM1 0x2D69
#define PKSAV_GS_CHECKSUM2 0x7E6D

#define PKSAV_CRYSTAL_CHECKSUM1 0x2D02
#define PKSAV_CRYSTAL_CHECKSUM2 0x1F0D

#define PKSAV_GEN2_DATA(save,offset) save->raw[pksav_gen2_offsets[offset][save->gen2_game]]

struct pksav_gen2_save_internal
{
    uint8_t* raw_save_ptr;

    uint8_t* checksum1_ptr;
    uint8_t* checksum2_ptr;
};

enum pksav_gen2_field
{
    PKSAV_GEN2_PLAYER_ID = 0,
    PKSAV_GEN2_PLAYER_NAME,
    PKSAV_GEN2_RIVAL_NAME,
    PKSAV_GEN2_DAYLIGHT_SAVINGS,
    PKSAV_GEN2_TIME_PLAYED,
    PKSAV_GEN2_PLAYER_PALETTE,
    PKSAV_GEN2_MONEY,
    PKSAV_GEN2_ITEM_BAG,
    PKSAV_GEN2_ITEM_PC,
    PKSAV_GEN2_CURRENT_POKEMON_BOX_NUM,
    PKSAV_GEN2_PC_BOX_NAMES,
    PKSAV_GEN2_POKEMON_PARTY,
    PKSAV_GEN2_POKEDEX_OWNED,
    PKSAV_GEN2_POKEDEX_SEEN,
    PKSAV_GEN2_CURRENT_POKEMON_BOX,
    PKSAV_GEN2_PLAYER_GENDER,
    PKSAV_GEN2_POKEMON_PC_FIRST_HALF,
    PKSAV_GEN2_POKEMON_PC_SECOND_HALF,
    PKSAV_GEN2_CHECKSUM1,
    PKSAV_GEN2_CHECKSUM2
};

static const uint16_t pksav_gen2_offsets[21][2] =
{
    {0x2009,0x2009}, // Player ID
    {0x200B,0x200B}, // Player name
    {0x2021,0x2021}, // Rival name
    {0x2037,0x2037}, // Daylight savings
    {0x2053,0x2054}, // Time played
    {0x206B,0x206A}, // Player palette
    {0x23DB,0x23DB}, // Money
    {0x23E6,0x23E7}, // Item bag
    {0x247E,0x247F}, // Item PC
    {0x2724,0x2700}, // Current Pokemon box number
    {0x2727,0x2703}, // PC box names
    {0x288A,0x2865}, // Pokemon party
    {0x2A4C,0x2A27}, // Pokedex owned
    {0x2A6C,0x2A47}, // Pokedex seen
    {0x2D6C,0x2D10}, // Current Pokemon box list
    {0x3E3D,0x3E3D}, // Player gender (Crystal only)
    {0x4000,0x4000}, // Pokemon PC (first half)
    {0x6000,0x6000}, // Pokemon PC (second half)
    {0x2D69,0x2D0D}, // Checksum 1
    {0x7E6D,0x1F0D}  // Checksum 2
};

static void _pksav_gen2_get_save_checksums(
    enum pksav_gen2_save_type save_type,
    const uint8_t* buffer,
    uint16_t* checksum1_out,
    uint16_t* checksum2_out
)
{
    assert(buffer != NULL);
    assert(checksum1_out != NULL);
    assert(checksum2_out != NULL);

    *checksum1_out = 0;
    *checksum2_out = 0;

    switch(save_type)
    {
        case PKSAV_GEN2_SAVE_TYPE_GS:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2D68; ++buffer_index)
            {
                *checksum1_out += buffer[buffer_index];
            }

            for(size_t buffer_index = 0x0C6B; buffer_index <= 0x17EC; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            for(size_t buffer_index = 0x3D96; buffer_index <= 0x3F3F; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            for(size_t buffer_index = 0x7E39; buffer_index <= 0x7E6C; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            break;

        case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
            for(size_t buffer_index = 0x2009; buffer_index <= 0x2B82; ++buffer_index)
            {
                *checksum1_out += buffer[buffer_index];
            }

            for(size_t buffer_index = 0x1209; buffer_index <= 0x1D82; ++buffer_index)
            {
                *checksum2_out += buffer[buffer_index];
            }
            break;

        default:
            assert(false);
    }

    *checksum1_out = pksav_littleendian16(*checksum1_out);
    *checksum2_out = pksav_littleendian16(*checksum2_out);
}

enum pksav_error pksav_gen2_get_buffer_save_type(
    const uint8_t* buffer,
    size_t buffer_len,
    enum pksav_gen2_save_type* save_type_out
)
{
    if(!buffer || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *save_type_out = PKSAV_GEN2_SAVE_TYPE_NONE;
    if(buffer_len >= PKSAV_GEN2_SAVE_SIZE)
    {
        bool is_type_found = false;
        for(enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_GS;
            (save_type <= PKSAV_GEN2_SAVE_TYPE_CRYSTAL) && !is_type_found;
            ++save_type)
        {
            uint16_t buffer_checksum1 = 0;
            uint16_t buffer_checksum2 = 0;
            _pksav_gen2_get_save_checksums(
                save_type,
                buffer,
                &buffer_checksum1,
                &buffer_checksum2
            );

            size_t checksum1_index = 0;
            size_t checksum2_index = 0;
            switch(save_type)
            {
                case PKSAV_GEN2_SAVE_TYPE_GS:
                    checksum1_index = PKSAV_GS_CHECKSUM1;
                    checksum2_index = PKSAV_GS_CHECKSUM2;
                    break;

                case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
                    checksum1_index = PKSAV_CRYSTAL_CHECKSUM1;
                    checksum2_index = PKSAV_CRYSTAL_CHECKSUM2;
                    break;

                default:
                    assert(false);
            }

            const uint16_t* checksum1_ptr = (const uint16_t*)&buffer[checksum1_index];
            const uint16_t* checksum2_ptr = (const uint16_t*)&buffer[checksum2_index];

            uint16_t checksum1 = pksav_littleendian16(*checksum1_ptr);
            uint16_t checksum2 = pksav_littleendian16(*checksum2_ptr);

            switch(save_type)
            {
                case PKSAV_GEN2_SAVE_TYPE_GS:
                    is_type_found = (checksum1 == buffer_checksum1) &&
                                    (checksum2 == buffer_checksum2);
                    break;

                /*
                 * From what I've seen, valid Crystal saves don't always have both
                 * checksums set correctly.
                 */
                case PKSAV_GEN2_SAVE_TYPE_CRYSTAL:
                    is_type_found = (checksum1 == buffer_checksum1) ||
                                    (checksum2 == buffer_checksum2);
                    break;

                default:
                    assert(false);
            }

            if(is_type_found)
            {
                *save_type_out = save_type;
            }
        }
    }

    return error;
}

enum pksav_error pksav_gen2_get_file_save_type(
    const char* filepath,
    enum pksav_gen2_save_type* save_type_out
)
{
    if(!filepath || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &file_buffer,
                &buffer_len
            );
    if(!error)
    {
        assert(file_buffer != NULL);

        enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        error = pksav_gen2_get_buffer_save_type(
                    file_buffer,
                    buffer_len,
                    &save_type
                );
        free(file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *save_type_out = save_type;
        }
    }

    return error;
}

/*enum pksav_error pksav_gen2_save_load(
    const char* filepath,
    struct pksav_gen2_save* gen2_save
) {
    if(!filepath || !gen2_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Read the file and make sure it's valid
    FILE* gen2_save_file = fopen(filepath, "rb");
    if(!gen2_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen2_save_file, 0, SEEK_END);

    if(ftell(gen2_save_file) < PKSAV_GEN2_SAVE_SIZE) {
        fclose(gen2_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen2_save->raw = calloc(PKSAV_GEN2_SAVE_SIZE, 1);
    fseek(gen2_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gen2_save->raw, 1, PKSAV_GEN2_SAVE_SIZE, gen2_save_file);
    fclose(gen2_save_file);
    if(num_read != PKSAV_GEN2_SAVE_SIZE) {
        return PKSAV_ERROR_FILE_IO;
    }

    enum pksav_error error_code = PKSAV_ERROR_NONE;
    bool is_valid = false;

    error_code = pksav_buffer_is_gen2_save(
                     gen2_save->raw,
                     PKSAV_GEN2_SAVE_SIZE,
                     false,
                     &is_valid
                 );
    if(error_code) {
        free(gen2_save->raw);
        return error_code;
    } else if(is_valid) {
        gen2_save->gen2_game = PKSAV_GEN2_GS;
    }

    if(!is_valid) {
        error_code = pksav_buffer_is_gen2_save(
                         gen2_save->raw,
                         PKSAV_GEN2_SAVE_SIZE,
                         true,
                         &is_valid
                     );
        if(error_code) {
            free(gen2_save->raw);
            return error_code;
        } else if(is_valid) {
            gen2_save->gen2_game = PKSAV_GEN2_CRYSTAL;
        } else {
            free(gen2_save->raw);
            return PKSAV_ERROR_INVALID_SAVE;
        }
    }

    // Set pointers
    gen2_save->pokemon_party = (struct pksav_gen2_pokemon_party*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_POKEMON_PARTY);
    gen2_save->current_pokemon_box_num = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_CURRENT_POKEMON_BOX_NUM);
    gen2_save->current_pokemon_box = (struct pksav_gen2_pokemon_box*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_CURRENT_POKEMON_BOX);

    for(uint8_t i = 0; i < 7; ++i) {
        uint16_t offset = struct pksav_gen2_offsets[PKSAV_GEN2_POKEMON_PC_FIRST_HALF][0] + (sizeof(pksav_gen2_pokemon_box)*i);
        gen2_save->pokemon_boxes[i] = (struct pksav_gen2_pokemon_box*)&gen2_save->raw[offset];
    }
    for(uint8_t i = 7; i < 14; ++i) {
        uint16_t offset = struct pksav_gen2_offsets[PKSAV_GEN2_POKEMON_PC_SECOND_HALF][0] + (sizeof(pksav_gen2_pokemon_box)*(i-7));
        gen2_save->pokemon_boxes[i] = (struct pksav_gen2_pokemon_box*)&gen2_save->raw[offset];
    }

    gen2_save->pokemon_box_names = (struct pksav_gen2_pokemon_box_names*)&PKSAV_GEN2_DATA(gen2_save, PKSAV_GEN2_PC_BOX_NAMES);
    gen2_save->item_bag = (struct pksav_gen2_item_bag*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_ITEM_BAG);
    gen2_save->item_pc = (struct pksav_gen2_item_pc*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_ITEM_PC);
    gen2_save->pokedex_seen = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_POKEDEX_SEEN);
    gen2_save->pokedex_owned = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_POKEDEX_OWNED);
    gen2_save->daylight_savings = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_DAYLIGHT_SAVINGS);
    gen2_save->time_played = (struct pksav_gen2_time_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2IME_PLAYED);
    gen2_save->money = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_MONEY);
    gen2_save->trainer_id = (uint16_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_PLAYER_ID);
    gen2_save->trainer_name = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_PLAYER_NAME);
    gen2_save->rival_name = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_RIVAL_NAME);

    if(gen2_save->gen2_game == PKSAV_GEN2_CRYSTAL) {
        gen2_save->trainer_gender = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_PLAYER_GENDER);
    } else {
        gen2_save->trainer_gender = NULL;
    }

    return PKSAV_ERROR_NONE;
}*/

/*enum pksav_error pksav_gen2_save_save(
    const char* filepath,
    struct pksav_gen2_save* gen2_save
) {
    if(!filepath || !gen2_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Make sure we can write to this file
    FILE* gen2_save_file = fopen(filepath, "wb");
    if(!gen2_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    // Set checksum
    _pksav_gen2_set_save_checksums(
        gen2_save->gen2_game,
        gen2_save->raw
    );

    // Write to file
    fwrite((void*)gen2_save->raw, 1, PKSAV_GEN2_SAVE_SIZE, gen2_save_file);
    fclose(gen2_save_file);

    return PKSAV_ERROR_NONE;
}*/

/*enum pksav_error pksav_gen2_save_free(
    struct pksav_gen2_save* gen2_save
) {
    if(!gen2_save || !gen2_save->raw) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Free dynamically allocated memory
    free(gen2_save->raw);

    // Set all pointer members to NULL
    gen2_save->pokemon_party = NULL;
    gen2_save->current_pokemon_box_num = NULL;
    gen2_save->current_pokemon_box = NULL;
    for(int i = 0; i < 14; ++i) {
        gen2_save->pokemon_boxes[i] = NULL;
    }
    gen2_save->pokemon_box_names = NULL;
    gen2_save->item_bag = NULL;
    gen2_save->item_pc = NULL;
    gen2_save->pokedex_seen = NULL;
    gen2_save->pokedex_owned = NULL;
    gen2_save->trainer_name = NULL;
    gen2_save->trainer_id = NULL;
    gen2_save->trainer_gender = NULL;
    gen2_save->money = NULL;
    gen2_save->rival_name = NULL;
    gen2_save->daylight_savings = NULL;
    gen2_save->time_played = NULL;
    gen2_save->raw = NULL;

    return PKSAV_ERROR_NONE;
}*/
