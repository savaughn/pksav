/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_SAVE_H
#define PKSAV_GEN1_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gen1/badges.h>
#include <pksav/gen1/items.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GEN1_SAVE_SIZE 0x8000
#define PKSAV_GEN1_NUM_POKEMON_BOXES 12

enum pksav_gen1_save_type
{
    PKSAV_GEN1_SAVE_TYPE_NONE,
    PKSAV_GEN1_SAVE_TYPE_RED_BLUE,
    PKSAV_GEN1_SAVE_TYPE_YELLOW
};

/*!
 * @brief Mask for getting the current Pokémon box number.
 *
 * In the field for storing the current Pokémon box number, the relevant
 * value is only stored in the lower four bytes.
 *
 * This is used with the pksav_gen1_save_t.current_pokemon_box_num field.
 */
#define PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK ((uint8_t)0x0F)

struct pksav_gen1_pokedex_lists
{
    /*!
     * @brief A pointer to the list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* seen_ptr;

    /*!
     * @brief A pointer to the list of Pokémon owned by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* owned_ptr;
};

struct pksav_gen1_pokemon_pc
{
    /*!
     * @brief Pointers to the trainer's Pokémon boxes.
     *
     * The boxes are not stored contiguously in the save file, so these pointers
     * point to their actual positions in the file.
     */
    struct pksav_gen1_pokemon_box* pokemon_box_ptrs[PKSAV_GEN1_NUM_POKEMON_BOXES];

    /*!
     * @brief The number of the current Pokémon box (0-11).
     *
     * There are 12 Pokémon boxes, and this value (0-based) shows which one will be
     * deposited and withdrawn from.
     *
     * The ::PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK should be used to access or set
     * this value.
     */
    uint8_t* current_pokemon_box_num_ptr;

    /*!
     * @brief A pointer to the current Pokémon box.
     *
     * Pokémon are only deposited or withdrawn from this box, and its data is
     * switched out when the current box is changed.
     */
    struct pksav_gen1_pokemon_box* current_pokemon_box_ptr;
};

struct pksav_gen1_item_storage
{
    //! A pointer to the trainer's item bag.
    struct pksav_gen1_item_bag* item_bag_ptr;

    //! A pointer to the trainer's item PC.
    struct pksav_gen1_item_pc* item_pc_ptr;
};

struct pksav_gen1_trainer_info
{
    /*!
     * @brief A pointer to the trainer's ID (stored in big-endian).
     *
     * This value should be accessed and modified with ::pksav_bigendian16.
     */
    uint16_t* trainer_id_ptr;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen1 with a num_chars
     * value of 7.
     *
     * This value should be set with ::pksav_text_to_gen1 with a num_chars
     * value of 7.
     */
    uint8_t* trainer_name_ptr;

    /*!
     * @brief A pointer to how much money the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 3. It should be set with ::pksav_to_bcd, with a maximum value of 999999.
     */
    uint8_t* money_ptr;

    /*!
     * @brief A pointer to the list of badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen1_badge_t enum.
     */
    uint8_t* badges_ptr;
};

struct pksav_gen1_misc_fields
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
    uint8_t* rival_name_ptr;

    /*!
     * @brief A pointer to how many casino coins the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 2. It should be set with ::pksav_to_bcd, with a maximum value of 999.
     */
    uint8_t* casino_coins_ptr;

    /*!
     * @brief A pointer to Pikachu's friendship level in Pokémon Yellow.
     *
     * In Pokémon Red/Blue, this field is unused and is set to 0.
     */
    uint8_t* pikachu_friendship_ptr;
};

struct pksav_gen1_save_internal
{
    uint8_t* raw_save_ptr;
    uint8_t* checksum_ptr;
};

/*!
 * @brief The primary PKSav struct for interacting with Generation I save files.
 *
 * After passing the struct into ::pksav_gen1_save_load, it will hold the binary
 * data for the save file and provide pointers to all relevant offsets within the
 * file. Accessing these pointers before calling ::pksav_gen1_save_load will result
 * in undefined behavior.
 *
 * To save the save file's data to another location, pass the struct into
 * ::pksav_gen1_save_save, along with a filepath.
 *
 * Once you are finished using the struct, pass it into ::pksav_gen1_save_free to
 * free the memory allocated by ::pksav_gen1_save_load.
 */
struct pksav_gen1_save
{
    enum pksav_gen1_save_type save_type;

    struct pksav_gen1_item_storage item_storage;

    struct pksav_gen1_pokemon_pc pokemon_pc;

    struct pksav_gen1_pokedex_lists pokedex_lists;

    struct pksav_gen1_trainer_info trainer_info;

    struct pksav_gen1_misc_fields misc_fields;

    //! A pointer to the amount of time this save file has been played.
    struct pksav_gen1_time* time_played_ptr;

    // TODO: implement
    uint8_t* options_ptr;

    struct pksav_gen1_save_internal _internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API enum pksav_error pksav_gen1_is_buffer_valid_save(
    const uint8_t* buffer,
    size_t buffer_len,
    bool* result_out
);

PKSAV_API enum pksav_error pksav_gen1_is_file_valid_save(
    const char* filepath,
    bool* result_out
);

PKSAV_API enum pksav_error pksav_gen1_load_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save_out
);

PKSAV_API enum pksav_error pksav_gen1_save_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save_ptr
);

PKSAV_API enum pksav_error pksav_gen1_free_save(
    struct pksav_gen1_save* gen1_save_ptr
);

PKSAV_API enum pksav_error pksav_gen1_pokemon_pc_set_current_box(
    struct pksav_gen1_pokemon_pc* gen1_pokemon_pc_ptr,
    uint8_t new_current_box_num
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_SAVE_H */
