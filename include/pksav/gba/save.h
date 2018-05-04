/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_H
#define PKSAV_GBA_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/common/trainer_id.h>

#include <pksav/gba/items.h>
#include <pksav/gba/pokedex.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save_type.h>
#include <pksav/gba/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GBA_SAVE_SIZE (0x10000)

#define PKSAV_GBA_TRAINER_NAME_LENGTH (7)
#define PKSAV_GBA_RIVAL_NAME_LENGTH (7)

#define PKSAV_GBA_SAVE_MONEY_MAX_VALUE (999999)
#define PKSAV_GBA_SAVE_CASINO_COINS_MAX_VALUE (9999)

struct pksav_gba_options
{
    uint8_t* p_button_mode;

    uint8_t* p_text_options;

    uint8_t* p_sound_battle_options;
};

struct pksav_gba_pokemon_storage
{
    struct pksav_gba_pokemon_party* p_party;

    struct pksav_gba_pokemon_pc* p_pc;
};

struct pksav_gba_item_storage
{
    union pksav_gba_item_bag* p_bag;

    struct pksav_gba_item_pc* p_pc;
};

struct pksav_gba_trainer_info
{
    union pksav_trainer_id* p_id;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gba with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gba with a num_chars
     * value of 7.
     */
    uint8_t* p_name;

    uint8_t* p_gender;

    uint32_t* p_money;
};

struct pksav_gba_misc_fields
{
    uint8_t* p_rival_name;

    uint16_t* p_casino_coins;
};

struct pksav_gba_save
{
    enum pksav_gba_save_type save_type;

    struct pksav_gba_time* p_time_played;

    struct pksav_gba_options options;

    struct pksav_gba_item_storage item_storage;

    struct pksav_gba_pokemon_storage pokemon_storage;

    struct pksav_gba_pokedex pokedex;

    struct pksav_gba_trainer_info trainer_info;

    struct pksav_gba_misc_fields misc_fields;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_gba_get_buffer_save_type(
    const uint8_t* p_buffer,
    size_t buffer_len,
    enum pksav_gba_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gba_get_file_save_type(
    const char* p_filepath,
    enum pksav_gba_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gba_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    struct pksav_gba_save* p_gba_save_out
);

PKSAV_API enum pksav_error pksav_gba_load_save_from_file(
    const char* p_filepath,
    struct pksav_gba_save* p_gba_save_out
);

PKSAV_API enum pksav_error pksav_gba_save_save(
    const char* p_filepath,
    struct pksav_gba_save* p_gba_save
);

PKSAV_API enum pksav_error pksav_gba_free_save(
    struct pksav_gba_save* p_gba_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
