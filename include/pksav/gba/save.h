/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_SAVE_H
#define PKSAV_GBA_SAVE_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <pksav/gba/items.h>
#include <pksav/gba/save_structs.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*!
 * @brief An enum describing each type of Game Boy Advance game.
 *
 * This enum is to be used with ::pksav_file_is_gba_save and
 * ::pksav_gba_save_load.
 *
 * Although Pokémon Emerald is very similar to Ruby/Sapphire, this
 * enum distinguishes games based on how data is stored internally.
 */
typedef enum {
    //! Pokémon Ruby/Sapphire
    PKSAV_GBA_RS = 0,
    //! Pokémon Emerald
    PKSAV_GBA_EMERALD,
    //! Pokémon FireRed/LeafGreen
    PKSAV_GBA_FRLG
} pksav_gba_game_t;

// How many bytes in each section are read for the checksum
#ifndef __DOXYGEN__
static const uint16_t pksav_gba_section_sizes[14] = {
    3884,3968,3968,3968,3848,3968,3968,
    3968,3968,3968,3968,3968,3968,2000
};
#endif

/*!
 * @brief The structure representing a Game Boy Advance save.
 *
 * The pointers in this structure should not be used before passing it
 * into ::pksav_gba_save_load and should not be used after passing it
 * into ::pksav_gba_save_save. Doing so will result in undefined behavior.
 */
typedef struct {
    pksav_gba_trainer_info_t* trainer_info;
    uint8_t* rival_name;

    pksav_gba_pokemon_party_t* pokemon_party;
    pksav_gba_pokemon_pc_t* pokemon_pc;

    pksav_gba_item_storage_t* item_storage;

    uint32_t* money;
    uint16_t* casino_coins;

    pksav_gba_game_t gba_game;

    // Do not edit these
    uint32_t security_key;
    uint8_t shuffled_section_nums[14];
    bool small_save;
    bool from_first_slot;
    pksav_gba_save_slot_t* unshuffled;
    uint8_t* raw;
} pksav_gba_save_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Checks if the given buffer is a valid Game Boy Advance save.
 *
 * \param buffer buffer to check
 * \param buffer_len size of the buffer to check
 * \param gba_game which type of Game Boy Advance game to test for
 */
PKSAV_API bool pksav_buffer_is_gba_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gba_game_t gba_game
);

/*!
 * @brief Checks if the given file is a valid Game Boy Advance save.
 *
 * \param filepath path of the file to check
 * \param gba_game which type of Game Boy Advance game to test for
 */
PKSAV_API bool pksav_file_is_gba_save(
    const char* filepath,
    pksav_gba_game_t gba_game
);

/*!
 * @brief Loads the save file at the given path and populates the given save struct
 *
 * This function takes care of all internal storage details, removing the need for a user
 * to worry about unshuffling sections, decrypting Pokémon and items, etc.
 *
 * At the end of the function, all pointers in the gba_save parameter will point to
 * completely contiguous and decrypted data.
 *
 * \param filepath path of the file to load
 * \param gba_save pointer to save struct to populate
 * \returns ::PKSAV_ERROR_NONE upon completion
 * \returns ::PKSAV_ERROR_FILE_IO if an error occurs reading the file
 * \returns ::PKSAV_ERROR_INVALID_SAVE if the file is not a valid Game Boy Advance save
 */
PKSAV_API pksav_error_t pksav_gba_save_load(
    const char* filepath,
    pksav_gba_save_t* gba_save
);

/*!
 * @brief Saves the given save file to the given path
 *
 * This function takes care of all internal storage details, removing the need for a
 * user to worry about shuffling sections, encrypting Pokémon and items, etc.
 *
 * This function will save the data into the save slot opposite that from which it was
 * loaded, leaving the original save intact. Its save counter will be incremented, so
 * the game will load this save slot instead of the previous one.
 *
 * \param filepath where to save the save file
 * \param gba_save pointer to the save struct to save
 * \returns ::PKSAV_ERROR_NONE upon completion
 * \returns ::PKSAV_ERROR_FILE_IO if an error occurs writing the file
 */
PKSAV_API pksav_error_t pksav_gba_save_save(
    const char* filepath,
    pksav_gba_save_t* gba_save
);

/*!
 * @brief Frees memory allocated by ::pksav_gba_save_load.
 *
 * After using this function, the given pksav_gba_save_t should not be
 * used unless passed into ::pksav_gba_save_load, as all pointers in
 * the struct will be invalid. Attempting to access or modify them will
 * result in undefined behavior.
 *
 * \param gba_save the save struct to free
 */
static PKSAV_INLINE void pksav_gba_save_free(
    pksav_gba_save_t* gba_save
) {
    free(gba_save->pokemon_pc);
    free(gba_save->unshuffled);
    free(gba_save->raw);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
