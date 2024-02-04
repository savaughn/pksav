/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_SAVE_H
#define pksav_gen3_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/common/trainer_id.h>

#include <pksav/gen3/common.h>
#include <pksav/gen3/daycare.h>
#include <pksav/gen3/items.h>
#include <pksav/gen3/map.h>
#include <pksav/gen3/pokedex.h>
#include <pksav/gen3/pokemon.h>
#include <pksav/gen3/roamer.h>
#include <pksav/gen3/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define pksav_gen3_SAVE_SIZE (0x10000)

#define pksav_gen3_RIVAL_NAME_LENGTH pksav_gen3_TRAINER_NAME_LENGTH

#define pksav_gen3_SAVE_MONEY_MAX_VALUE        (999999)
#define pksav_gen3_SAVE_CASINO_COINS_MAX_VALUE (9999)

struct pksav_gen3_options
{
    uint8_t* p_button_mode;

    uint8_t* p_text_options;

    uint8_t* p_sound_battle_options;
};

struct pksav_gen3_pokemon_storage
{
    struct pksav_gen3_pokemon_party* p_party;

    struct pksav_gen3_pokemon_pc* p_pc;

    union pksav_gen3_daycare* p_daycare;
};

struct pksav_gen3_item_storage
{
    union pksav_gen3_item_bag* p_bag;

    struct pksav_gen3_item_pc* p_pc;

    uint16_t* p_registered_item;
};

struct pksav_gen3_player_info
{
    union pksav_trainer_id* p_id;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen3 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen3 with a num_chars
     * value of 7.
     */
    uint8_t* p_name;

    uint8_t* p_gender;

    uint32_t* p_money;

    struct pksav_gen3_player_location_info* p_location_info;
};

/*struct pksav_gen3_rs_fields
{
};

struct pksav_gen3_emerald_fields
{
};*/

struct pksav_gen3_frlg_fields
{
    uint8_t* p_rival_name;
};

struct pksav_gen3_misc_fields
{
    uint16_t* p_casino_coins;

    struct pksav_gen3_roamer* p_roamer;

    //struct pksav_gen3_rs_fields rs_fields;
    //struct pksav_gen3_emerald_fields emerald_fields;
    struct pksav_gen3_frlg_fields frlg_fields;
};

struct pksav_gen3_save
{
    enum pksav_gen3_save_type save_type;

    struct pksav_gen3_time* p_time_played;

    struct pksav_gen3_options options;

    struct pksav_gen3_item_storage item_storage;

    struct pksav_gen3_pokemon_storage pokemon_storage;

    struct pksav_gen3_pokedex pokedex;

    struct pksav_gen3_player_info player_info;

    struct pksav_gen3_misc_fields misc_fields;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_gen3_get_buffer_save_type(
    const uint8_t* p_buffer,
    size_t buffer_len,
    enum pksav_gen3_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gen3_get_file_save_type(
    const char* p_filepath,
    enum pksav_gen3_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gen3_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    struct pksav_gen3_save* p_gen3_save_out
);

PKSAV_API enum pksav_error pksav_gen3_load_save_from_file(
    const char* p_filepath,
    struct pksav_gen3_save* p_gen3_save_out
);

PKSAV_API enum pksav_error pksav_gen3_save_save(
    const char* p_filepath,
    struct pksav_gen3_save* p_gen3_save
);

PKSAV_API enum pksav_error pksav_gen3_free_save(
    struct pksav_gen3_save* p_gen3_save
);

#ifdef __cplusplus
}
#endif

#endif /* pksav_gen3_SAVE_H */
