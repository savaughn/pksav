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

#include <pksav/gen1/items.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum pksav_gen1_save_type
{
    PKSAV_GEN1_RED_BLUE,
    PKSAV_GEN1_YELLOW
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

/*!
 * @brief Bitmasks for checking if the player has a specific badge.
 *
 * The pksav_gen1_save_t.badges field points to the byte in the save file
 * that stores this bitmap. Use these enum values on this byte to access and
 * set whether or not the player has this badge.
 */
enum pksav_gen1_badge_mask
{
    //! Earth Badge (Giovanni, Badge #8)
    PKSAV_GEN1_EARTH_BADGE    = 0x01,
    //! Volcano Badge (Blaine, Badge #7)
    PKSAV_GEN1_VOLCANO_BADGE  = 0x02,
    //! Marsh Badge (Sabrina, Badge #6)
    PKSAV_GEN1_MARSH_BADGE    = 0x04,
    //! Soul Badge (Koga, Badge #5)
    PKSAV_GEN1_SOUL_BADGE     = 0x08,
    //! Rainbow Badge (Erika, Badge #4)
    PKSAV_GEN1_RAINBOW_BADGE  = 0x10,
    //! Thunder Badge (Lt. Surge, Badge #3)
    PKSAV_GEN1_THUNDER_BADGE  = 0x20,
    //! Cascade Badge (Misty, Badge #2)
    PKSAV_GEN1_CASCADE_BADGE  = 0x40,
    //! Boulder Badge (Brock, Badge #1)
    PKSAV_GEN1_BOULDER_BADGE  = 0x80
};

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
    struct pksav_gen1_pokemon_box* pokemon_box_ptrs[12];

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

struct pksav_gen1_save_internal
{
    uint8_t* raw;
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

    //! A pointer to the amount of time this save file has been played.
    struct pksav_gen1_time* time_played_ptr;

    /*!
     * @brief A pointer to how many casino coins the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 2. It should be set with ::pksav_to_bcd, with a maximum value of 999.
     */
    uint8_t* casino_coins_ptr;
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
     * @brief A pointer to Pikachu's friendship level in Pokémon Yellow.
     *
     * In Pokémon Red/Blue, this field is unused and is set to 0.
     */
    uint8_t* pikachu_friendship_ptr;

    // TODO: implement
    uint8_t* options_ptr;

    struct pksav_gen1_save_internal _internal;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Determines whether the given buffer is from a valid Generation I save file.
 *
 * This is determining by checking the data's checksum.
 *
 * \param buffer buffer to be checked
 * \param buffer_len length of the buffer passed in
 * \param result_out whether or not the given buffer has a valid Generation I save file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_buffer_is_gen1_save(
    const uint8_t* buffer,
    size_t buffer_len,
    bool* result_out
);

/*!
 * @brief Determines whether the file at the given path is a valid Generation I save file.
 *
 * This is determined simply by checking the file's checksum.
 *
 * \param filepath path to the file to be checked
 * \param result_out whether or not the given buffer has a valid Generation I save file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_file_is_gen1_save(
    const char* filepath,
    bool* result_out
);

/*!
 * @brief Loads the save file at the given path into the given PKSav struct.
 *
 * Upon a failure state, the given pksav_gen1_save_t will be left in an undefined state.
 *
 * \param filepath path to the file to be loaded
 * \param gen1_save PKSav struct in which to load file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gen1_save is NULL
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs reading the file
 * \returns ::PKSAV_ERROR_INVALID_SAVE if the given file is not a valid Generation I save file
 */
PKSAV_API pksav_error_t pksav_gen1_save_load(
    const char* filepath,
    struct pksav_gen1_save* gen1_save
);

/*!
 * @brief Saves a Generation I save file to the given path.
 *
 * Upon a failure state, the save file is not guaranteed to have been properly written.
 *
 * \param filepath where the save file should be written
 * \param gen1_save the same to be written
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gen1_save is NULL
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs writing the file
 */
PKSAV_API pksav_error_t pksav_gen1_save_save(
    const char* filepath,
    struct pksav_gen1_save* gen1_save
);

/*!
 * @brief Frees memory allocated for a struct pksav_gen1_save.
 *
 * \param gen1_save save whose memory should be freed
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if gen1_save is NULL
 */
PKSAV_API pksav_error_t pksav_gen1_save_free(
    struct pksav_gen1_save* gen1_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_SAVE_H */
