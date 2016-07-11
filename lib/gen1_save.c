/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/checksum.h>
#include <pksav/gen1_save.h>

#include <stdio.h>
#include <string.h>

static bool _pksav_file_is_gen1_save(
    const uint8_t* data
) {
    uint8_t checksum = data[PKSAV_GEN1_CHECKSUM];
    uint8_t actual_checksum = pksav_get_gen1_save_checksum(data);

    return (checksum == actual_checksum);
}

bool pksav_file_is_gen1_save(
    const char* filepath
) {
    FILE* gen1_save = fopen(filepath, "r");
    fseek(gen1_save, SEEK_END, 0);

    if(ftell(gen1_save) < PKSAV_GEN1_SAVE_SIZE) {
        return false;
    }

    uint8_t* gen1_save_data = malloc(PKSAV_GEN1_SAVE_SIZE);
    fseek(gen1_save, SEEK_SET, 0);
    fread((void*)gen1_save_data, 1, PKSAV_GEN1_SAVE_SIZE, gen1_save);
    bool ret = _pksav_file_is_gen1_save(gen1_save_data);

    free(gen1_save_data);
    return ret;
}

pksav_error_t pksav_gen1_save_load(
    const char* filepath,
    pksav_gen1_save_t* gen1_save
) {
    FILE* gen1_save_file = fopen(filepath, "r");
    fseek(gen1_save_file, SEEK_END, 0);

    if(ftell(gen1_save_file) < PKSAV_GEN1_SAVE_SIZE) {
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gen1_save->raw = malloc(PKSAV_GEN1_SAVE_SIZE);
    fseek(gen1_save_file, SEEK_SET, 0);
    fread((void*)gen1_save->raw, 1, PKSAV_GEN1_SAVE_SIZE, gen1_save_file);
    fclose(gen1_save_file);

    if(!_pksav_file_is_gen1_save(gen1_save->raw)) {
        free(gen1_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    return PKSAV_ERROR_NONE;
}
