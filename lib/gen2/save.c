/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen2/save.h>

#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define PKSAV_GS_CHECKSUM1 0x2D69
#define PKSAV_GS_CHECKSUM2 0x7E6D

#define PKSAV_CRYSTAL_CHECKSUM1 0x2D02
#define PKSAV_CRYSTAL_CHECKSUM2 0x1F0D

#define PKSAV_GEN2_SAVE_SIZE 0x8000

#define PKSAV_GEN2_DATA(save,offset) save->raw[pksav_gen2_offsets[offset][save->gen2_game]]

typedef enum {
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
} pksav_gen2_field_t;

static const uint16_t pksav_gen2_offsets[21][2] = {
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
    {0x2D69,0x2D02}, // Checksum 1
    {0x7E6D,0x1F0D}  // Checksum 2
};

typedef struct {
    uint16_t first;
    uint16_t second;
} pksav_gen2_checksums_t;

static void _pksav_gen2_get_save_checksums(
    bool crystal,
    const uint8_t* data,
    pksav_gen2_checksums_t* checksums_out
) {
    checksums_out->first = checksums_out->second = 0;

    if(crystal) {
        for(uint16_t i = 0x2009; i <= 0x2B82; ++i) {
            checksums_out->first += data[i];
        }

        for(uint16_t i = 0x1209; i <= 0x1D82; ++i) {
            checksums_out->second += data[i];
        }
    } else {
        for(uint16_t i = 0x2009; i <= 0x2D68; ++i) {
            checksums_out->first += data[i];
        }

        for(uint16_t i = 0x0C6B; i <= 0x17EC; ++i) {
            checksums_out->second += data[i];
        }
        for(uint16_t i = 0x3D96; i <= 0x3F3F; ++i) {
            checksums_out->second += data[i];
        }
        for(uint16_t i = 0x7E39; i <= 0x7E6C; ++i) {
            checksums_out->second += data[i];
        }

        checksums_out->first  = pksav_littleendian16(checksums_out->first);
        checksums_out->second = pksav_littleendian16(checksums_out->second);
    }
}

static void _pksav_gen2_set_save_checksums(
    bool crystal,
    uint8_t* data
) {
    uint16_t checksum1_index = crystal ? PKSAV_CRYSTAL_CHECKSUM1
                                       : PKSAV_GS_CHECKSUM1;
    uint16_t checksum2_index = crystal ? PKSAV_CRYSTAL_CHECKSUM2
                                       : PKSAV_GS_CHECKSUM2;

    pksav_gen2_checksums_t checksums;
    _pksav_gen2_get_save_checksums(crystal, data, &checksums);

    *((uint16_t*)&data[checksum1_index]) = checksums.first;
    *((uint16_t*)&data[checksum2_index]) = checksums.second;
}

static bool _pksav_file_is_gen2_save(
    bool crystal,
    const uint8_t* data
) {
    pksav_gen2_checksums_t checksums;
    _pksav_gen2_get_save_checksums(crystal, data, &checksums);

    uint16_t actual_checksum1 = pksav_littleendian16(
                                    data[crystal ? PKSAV_CRYSTAL_CHECKSUM1 : PKSAV_GS_CHECKSUM1]
                                );
    uint16_t actual_checksum2 = pksav_littleendian16(
                                    data[crystal ? PKSAV_CRYSTAL_CHECKSUM2 : PKSAV_GS_CHECKSUM2]
                                );

    return (checksums.first == actual_checksum1 &&
            checksums.second == actual_checksum2);
}

bool pksav_file_is_gen2_save(
    const char* filepath,
    bool crystal
) {
    FILE* gen2_save = fopen(filepath, "r");
    if(!gen2_save) {
        return false;
    }

    fseek(gen2_save, SEEK_END, 0);

    if(ftell(gen2_save) < PKSAV_GEN2_SAVE_SIZE) {
        return false;
    }

    uint8_t* gen2_save_data = malloc(PKSAV_GEN2_SAVE_SIZE);
    fseek(gen2_save, SEEK_SET, 0);
    size_t num_read = fread((void*)gen2_save_data, 1, PKSAV_GEN2_SAVE_SIZE, gen2_save);
    fclose(gen2_save);

    bool ret = false;
    if(num_read == PKSAV_GEN2_SAVE_SIZE) {
        ret = _pksav_file_is_gen2_save(crystal, gen2_save_data);
    }

    free(gen2_save_data);
    return ret;
}

pksav_error_t pksav_gen2_save_load(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
) {
    // Read the file and make sure it's valid
    FILE* gen2_save_file = fopen(filepath, "r");
    if(!gen2_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gen2_save_file, SEEK_END, 0);

    if(ftell(gen2_save_file) < PKSAV_GEN2_SAVE_SIZE) {
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen2_save->raw = malloc(PKSAV_GEN2_SAVE_SIZE);
    fseek(gen2_save_file, SEEK_SET, 0);
    size_t num_read = fread((void*)gen2_save->raw, 1, PKSAV_GEN2_SAVE_SIZE, gen2_save_file);
    fclose(gen2_save_file);
    if(num_read != PKSAV_GEN2_SAVE_SIZE) {
        return PKSAV_ERROR_FILE_IO;
    }

    if(_pksav_file_is_gen2_save(false, gen2_save->raw)) {
        gen2_save->gen2_game = PKSAV_GEN2_GS;
    } else if(_pksav_file_is_gen2_save(true, gen2_save->raw)) {
        gen2_save->gen2_game = PKSAV_GEN2_CRYSTAL;
    } else {
        free(gen2_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    // Set pointers
    gen2_save->pokemon_party = (pksav_gen2_pokemon_party_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_POKEMON_PARTY);

    uint16_t pokemon_pc_first_half = pksav_gen2_offsets[PKSAV_GEN2_POKEMON_PC_FIRST_HALF][gen2_save->gen2_game];
    uint16_t pokemon_pc_second_half = pksav_gen2_offsets[PKSAV_GEN2_POKEMON_PC_SECOND_HALF][gen2_save->gen2_game];

    for(uint8_t i = 0; i < 6; ++i) {
        uint16_t offset = pokemon_pc_first_half + (sizeof(pksav_gen2_pokemon_box_t)*i);
        gen2_save->pokemon_boxes[i] = (pksav_gen2_pokemon_box_t*)&gen2_save->raw[offset];
    }
    for(uint8_t i = 6; i < 12; ++i) {
        uint16_t offset = pokemon_pc_second_half + (sizeof(pksav_gen2_pokemon_box_t)*(i-6));
        gen2_save->pokemon_boxes[i] = (pksav_gen2_pokemon_box_t*)&gen2_save->raw[offset];
    }

    gen2_save->item_bag = (pksav_gen2_item_bag_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_ITEM_BAG);
    gen2_save->item_pc = (pksav_gen2_item_pc_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_ITEM_PC);
    gen2_save->time_played = (pksav_gen2_time_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_TIME_PLAYED);
    gen2_save->money = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_MONEY);
    gen2_save->trainer_id = (uint16_t*)&PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_PLAYER_ID);
    gen2_save->trainer_name = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_PLAYER_NAME);
    gen2_save->rival_name = &PKSAV_GEN2_DATA(gen2_save,PKSAV_GEN2_RIVAL_NAME);

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gen2_save_save(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
) {
    // Make sure we can write to this file
    FILE* gen2_save_file = fopen(filepath, "w");
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
}
