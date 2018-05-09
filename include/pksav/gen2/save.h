/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_SAVE_H
#define PKSAV_GEN2_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen2/items.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GEN2_SAVE_SIZE (0x8000)

#define PKSAV_GEN2_TRAINER_NAME_LENGTH (7)
#define PKSAV_GEN2_RIVAL_NAME_LENGTH   (7)

#define PKSAV_GEN2_SAVE_MONEY_BUFFER_SIZE_BYTES (3)
#define PKSAV_GEN2_SAVE_MONEY_MAX_VALUE         (999999)

#define PKSAV_GEN2_SAVE_CASINO_COINS_BUFFER_SIZE_BYTES (2)
#define PKSAV_GEN2_SAVE_CASINO_COINS_MAX_VALUE (9999)

#define PKSAV_GEN2_POKEDEX_BUFFER_SIZE_BYTES ((251 / 8) + 1)

#define PKSAV_GEN2_DAYLIGHT_SAVINGS_TIME_MASK ((uint8_t)(1 << 7))

enum pksav_gen2_save_type
{
    PKSAV_GEN2_SAVE_TYPE_NONE,
    PKSAV_GEN2_SAVE_TYPE_GS,
    PKSAV_GEN2_SAVE_TYPE_CRYSTAL
};

enum pksav_gen2_gender
{
    PKSAV_GEN2_GENDER_MALE,
    PKSAV_GEN2_GENDER_FEMALE
};

struct pksav_gen2_save_time
{
    //! A pointer to the amount of time this save file has been played.
    struct pksav_gen2_time* p_time_played;

    uint8_t* p_daylight_savings;
};

struct pksav_gen2_options
{
    uint8_t* p_misc_options;

    uint8_t* p_textbox_frame_index;

    uint8_t* p_gbprinter_brightness;

    uint8_t* p_menu_account;
};

struct pksav_gen2_pokedex_lists
{
    /*!
     * @brief A pointer to the list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* p_seen;

    /*!
     * @brief A pointer to the list of Pokémon owned by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* p_owned;
};

struct pksav_gen2_pokemon_storage
{
    struct pksav_gen2_pokemon_party* p_party;

    /*!
     * @brief Pointers to the trainer's Pokémon boxes.
     *
     * The boxes are not stored contiguously in the save file, so these pointers
     * point to their actual positions in the file.
     */
    struct pksav_gen2_pokemon_box* pp_boxes[PKSAV_GEN2_NUM_POKEMON_BOXES];

    struct pksav_gen2_pokemon_box_names* p_box_names;

    /*!
     * @brief The number of the current Pokémon box (0-11).
     *
     * There are 12 Pokémon boxes, and this value (0-based) shows which one will be
     * deposited and withdrawn from.
     *
     * The ::PKSAV_GEN2_CURRENT_POKEMON_BOX_NUM_MASK should be used to access or set
     * this value.
     */
    uint8_t* p_current_box_num;

    /*!
     * @brief A pointer to the current Pokémon box.
     *
     * Pokémon are only deposited or withdrawn from this box, and its data is
     * switched out when the current box is changed.
     */
    struct pksav_gen2_pokemon_box* p_current_box;
};

struct pksav_gen2_item_storage
{
    //! A pointer to the trainer's item bag.
    struct pksav_gen2_item_bag* p_item_bag;

    //! A pointer to the trainer's item PC.
    struct pksav_gen2_item_pc* p_item_pc;
};

struct pksav_gen2_trainer_info
{
    /*!
     * @brief A pointer to the trainer's ID (stored in big-endian).
     *
     * This value should be accessed and modified with ::pksav_bigendian16.
     */
    uint16_t* p_id;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen2 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen2 with a num_chars
     * value of 7.
     */
    uint8_t* p_name;

    uint8_t* p_gender;

    uint8_t* p_palette;

    /*!
     * @brief A pointer to how much money the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_import_bcd, with a num_bytes value
     * of 3. It should be set with ::pksav_export_bcd, with a maximum value of 999999.
     */
    uint8_t* p_money;

    /*!
     * @brief A pointer to the list of Johto badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen2_johto_badge_t enum.
     */
    uint8_t* p_johto_badges;

    /*!
     * @brief A pointer to the list of Kanto badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen2_kanto_badge_t enum.
     */
    uint8_t* p_kanto_badges;
};

struct pksav_gen2_misc_fields
{
    /*!
     * @brief A pointer to the rival's name.
     *
     * This value should be accessed with ::pksav_text_from_gen1 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen1 with a num_chars
     * value of 7.
     */
    uint8_t* p_rival_name;

    uint8_t* p_money_with_mom;

    uint8_t* p_mom_money_policy;

    uint8_t* p_casino_coins;
};

/*!
 * @brief The primary PKSav struct for interacting with Generation II save files.
 *
 * After passing the struct into ::pksav_gen2_save_load, it will hold the binary
 * data for the save file and provide pointers to all relevant offsets within the
 * file. Accessing these pointers before calling ::pksav_gen2_save_load will result
 * in undefined behavior.
 *
 * To save the save file's data to another location, pass the struct into
 * ::pksav_gen2_save_save, along with a p_filepath.
 *
 * Once you are finished using the struct, pass it into ::pksav_gen2_save_free to
 * free the memory allocated by ::pksav_gen2_save_load.
 */
struct pksav_gen2_save
{
    enum pksav_gen2_save_type save_type;

    struct pksav_gen2_options options;

    struct pksav_gen2_save_time save_time;

    struct pksav_gen2_item_storage item_storage;

    struct pksav_gen2_pokemon_storage pokemon_storage;

    struct pksav_gen2_pokedex_lists pokedex_lists;

    struct pksav_gen2_trainer_info trainer_info;

    struct pksav_gen2_misc_fields misc_fields;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_gen2_get_buffer_save_type(
    const uint8_t* buffer,
    size_t buffer_len,
    enum pksav_gen2_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gen2_get_file_save_type(
    const char* p_filepath,
    enum pksav_gen2_save_type* p_save_type_out
);

PKSAV_API enum pksav_error pksav_gen2_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    struct pksav_gen2_save* p_gen2_save_out
);

PKSAV_API enum pksav_error pksav_gen2_load_save_from_file(
    const char* p_filepath,
    struct pksav_gen2_save* p_gen2_save_out
);

PKSAV_API enum pksav_error pksav_gen2_save_save(
    const char* p_filepath,
    struct pksav_gen2_save* p_gen2_save
);

PKSAV_API enum pksav_error pksav_gen2_free_save(
    struct pksav_gen2_save* p_gen2_save
);

PKSAV_API enum pksav_error pksav_gen2_pokemon_storage_set_current_box(
    struct pksav_gen2_pokemon_storage* p_gen2_pokemon_storage,
    uint8_t new_current_box_num
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_H */
