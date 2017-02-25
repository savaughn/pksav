/*!
 * @file    pksav/gen2/save.h
 * @ingroup PKSav
 * @brief   Functions for interacting with Generation II save files.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pksav/gen2/text.h>
#include <pksav/gen2/time.h>

#include <pksav/math/bcd.h>
#include <pksav/math/endian.h>

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PKSAV_GEN2_MALE = 0,
    PKSAV_GEN2_FEMALE
} pksav_gen2_gender_t;

typedef enum {
    PKSAV_GEN2_GS = 0,
    PKSAV_GEN2_CRYSTAL
} pksav_gen2_game_t;

/*!
 * @brief The primary PKSav struct for interacting with Generation II save files.
 *
 * After passing the struct into ::pksav_gen2_save_load, it will hold the binary
 * data for the save file and provide pointers to all relevant offsets within the
 * file. Accessing these pointers before calling ::pksav_gen2_save_load will result
 * in undefined behavior.
 *
 * To save the save file's data to another location, pass the struct into
 * ::pksav_gen2_save_save, along with a filepath.
 *
 * Once you are finished using the struct, pass it into ::pksav_gen2_save_free to
 * free the memory allocated by ::pksav_gen2_save_load.
 */
typedef struct {
    //! A pointer to the trainer's Pokémon party.
    pksav_gen2_pokemon_party_t* pokemon_party;

    /*!
     * @brief The number of the current Pokémon box (0-13).
     *
     * There are 14 Pokémon boxes, and this value (0-based) shows which one will be
     * deposited and withdrawn from.
     */
    uint8_t* current_pokemon_box_num;

    /*!
     * @brief A pointer to the current Pokémon box.
     *
     * Pokémon are only deposited or withdrawn from this box, and its data is
     * switched out when the current box is changed.
     */
    pksav_gen2_pokemon_box_t* current_pokemon_box;

    //! A pointer to the Pokémon PC.
    pksav_gen2_pokemon_box_t* pokemon_boxes[14];

    //! A pointer to the Pokémon PC's box names.
    pksav_gen2_pokemon_box_names_t* pokemon_box_names;

    //! A pointer to the trainer's item bag.
    pksav_gen2_item_bag_t* item_bag;

    //! A pointer to the trainer's item PC.
    pksav_gen2_item_pc_t* item_pc;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen2 or
     * ::pksav_widetext_from_gen2, both with a num_chars value of 7.
     *
     * This value should be set with ::pksav_text_to_gen2 or
     * ::pksav_widetext_to_gen2, both with a num_chars value of 7.
     */
    uint8_t* trainer_name;

    /*!
     * @brief A pointer to the trainer's ID (stored in big-endian).
     *
     * This value should be accessed and modified with ::pksav_bigendian16.
     */
    uint16_t* trainer_id;

    /*!
     * @brief A pointer to the trainer's gender.
     *
     * As Crystal was the first game in the franchise to introduce a female player
     * character, this value is set to NULL for Gold and Silver saves.
     *
     * For Crystal saves, valid values are ::PKSAV_GEN2_MALE or ::PKSAV_GEN2_FEMALE.
     */
    uint8_t* trainer_gender;

    /*!
     * @brief A pointer to how much money the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 3. It should be set with ::pksav_to_bcd, with a maximum value of 999999.
     */
    uint8_t* money;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen2 or
     * ::pksav_widetext_from_gen2, both with a num_chars value of 7.
     *
     * This value should be set with ::pksav_text_to_gen2 or
     * ::pksav_widetext_to_gen2, both with a num_chars value of 7.
     */
    uint8_t* rival_name;

    /*!
     * @brief Whether daylight savings time is set.
     *
     * Valid values are 0 or 1.
     */
    uint8_t* daylight_savings;

    //! A pointer to the amount of time this save file has been played.
    pksav_gen2_time_t* time_played;

    //! Which Generation II save group this struct corresponds to.
    pksav_gen2_game_t gen2_game;

    #ifndef __DOXYGEN__
    uint8_t* raw;
    #endif
} pksav_gen2_save_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Determines whether the given buffer is from a valid Generation II save file.
 *
 * This is determining by checking the data's size and checksums. The data checked differs
 * between Gold/Silver and Crystal.
 *
 * \param buffer buffer to be checked
 * \param buffer_len length of the buffer passed in
 * \param crystal Whether to check Crystal's checksum locations
 * \param result_out whether or not the given buffer has a valid Generation II save file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_buffer_is_gen2_save(
    const uint8_t* buffer,
    size_t buffer_len,
    bool crystal,
    bool* result_out
);

/*!
 * @brief Determines whether the file at the given path is a valid Generation I save file.
 *
 * This is determined by checking the file's size and checksum. The data checked differs
 * between Gold/Silver and Crystal.
 *
 * \param filepath path to the file to be checked
 * \param crystal Whether to check Crystal's checksum locations
 * \param result_out whether or not the given buffer has a valid Generation I save file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_file_is_gen2_save(
    const char* filepath,
    bool crystal,
    bool* result_out
);

/*!
 * @brief Loads the save file at the given path into the given PKSav struct.
 *
 * Upon a failure state, the given pksav_gen2_save_t will be left in an undefined state.
 *
 * \param filepath path to the file to be loaded
 * \param gen2_save PKSav struct in which to load file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gen2_save is NULL
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs reading the file
 * \returns ::PKSAV_ERROR_INVALID_SAVE if the given file is not a valid Generation II save file
 */
PKSAV_API pksav_error_t pksav_gen2_save_load(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
);

/*!
 * @brief Saves a Generation II save file to the given path.
 *
 * Upon a failure state, the save file is not guaranteed to have been properly written.
 *
 * \param filepath where the save file should be written
 * \param gen2_save the same to be written
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gen2_save is NULL
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs writing the file
 */
PKSAV_API pksav_error_t pksav_gen2_save_save(
    const char* filepath,
    pksav_gen2_save_t* gen2_save
);

/*!
 * @brief Frees memory allocated for a pksav_gen2_save_t.
 *
 * After this function is called, all pointer members of the given save file will
 * be set to NULL.
 *
 * \param gen2_save save whose memory should be freed
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if gen2_save is NULL
 */
PKSAV_API pksav_error_t pksav_gen2_save_free(
    pksav_gen2_save_t* gen2_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN2_SAVE_H */
