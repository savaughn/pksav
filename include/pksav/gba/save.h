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
#include <pksav/gba/pokemon.h>

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
 * @brief Mask to set the first National Pokédex unlocked field in Ruby/Sapphire/Emerald.
 *
 * Use this mask to set the data at pksav_gba_save_t.rse_nat_pokedex_unlockedA. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.nat_pokedex_unlockedB and pksav_gba_save_t.nat_pokedex_unlockedC.
 */
#define PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_A (uint16_t)(0x01DA)

/*!
 * @brief Mask to set the second National Pokédex unlocked field in Ruby/Sapphire/Emerald.
 *
 * Use this mask to set the data at pksav_gba_save_t.nat_pokedex_unlockedB. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.rse_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedC.
 */
#define PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_B (uint8_t)(1 << 6)

/*!
 * @brief Mask to set the third National Pokédex unlocked field in Ruby/Sapphire/Emerald.
 *
 * Use this mask to set the data at pksav_gba_save_t.nat_pokedex_unlockedC. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.rse_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedB.
 */
#define PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_C (uint16_t)(0x0203)

/*!
 * @brief Mask to set the first National Pokédex unlocked field in FireRed/LeafGreen.
 *
 * Use this mask to set the data at pksav_gba_save_t.frlg_nat_pokedex_unlockedA. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.nat_pokedex_unlockedB and pksav_gba_save_t.nat_pokedex_unlockedC.
 */
#define PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_A (uint8_t)(0xB9)

/*!
 * @brief Mask to set the second National Pokédex unlocked field in FireRed/LeafGreen.
 *
 * Use this mask to set the data at pksav_gba_save_t.nat_pokedex_unlockedB. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.frlg_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedC.
 */
#define PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_B (uint8_t)(0x01)

/*!
 * @brief Mask to set the third National Pokédex unlocked field in FireRed/LeafGreen.
 *
 * Use this mask to set the data at pksav_gba_save_t.nat_pokedex_unlockedC. Any time
 * this value changes, the same corresponding action must be taken on the fields
 * pksav_gba_save_t.frlg_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedC.
 */
#define PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_C (uint16_t)(0x5862)

/*!
 * @brief The structure representing a Game Boy Advance save.
 *
 * The pointers in this structure should not be used before passing it
 * into ::pksav_gba_save_load and should not be used after passing it
 * into ::pksav_gba_save_save. Doing so will result in undefined behavior.
 */
typedef struct {
    //! Information on the player character.
    pksav_gba_trainer_info_t* trainer_info;

    /*!
     * @brief The rival's name in FireRed/LeafGreen.
     *
     * This name should be accessed with ::pksav_text_from_gba or
     * ::pksav_widetext_from_gba. This name should be modified with
     * ::pksav_text_to_gba or ::pksav_widetext_to_gba.
     *
     * In either case, the num_chars parameter should be 7.
     *
     * For Ruby/Sapphire/Emerald games, this pointer is set to NULL.
     */
    uint8_t* rival_name;

    //! The trainer's Pokémon party.
    pksav_gba_pokemon_party_t* pokemon_party;

    //! The trainer's Pokémon PC.
    pksav_gba_pokemon_pc_t* pokemon_pc;

    //! The trainer's item bag and PC.
    pksav_gba_item_storage_t* item_storage;

    //! The trainer's money (valid values 0-999999).
    uint32_t* money;

    //! The trainer's casino coins (valid values 0-999).
    uint16_t* casino_coins;

    /*!
     * @brief A pointer to the list of Pokémon owned by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     */
    uint8_t* pokedex_owned;

    /*!
     * @brief A pointer to the first list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     *
     * Any action taken on this list should also be taken on
     * pksav_gba_save_t.pokedex_seenB and pksav_gba_save_t.pokedex_seenC.
     */
    uint8_t* pokedex_seenA;

    /*!
     * @brief A pointer to the second list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     *
     * Any action taken on this list should also be taken on
     * pksav_gba_save_t.pokedex_seenA and pksav_gba_save_t.pokedex_seenC.
     */
    uint8_t* pokedex_seenB;

    /*!
     * @brief A pointer to the third list of Pokémon seen by the trainer.
     *
     * This list should be accessed with ::pksav_get_pokedex_bit and set with
     * ::pksav_set_pokedex_bit.
     *
     * Any action taken on this list should also be taken on
     * pksav_gba_save_t.pokedex_seenA and pksav_gba_save_t.pokedex_seenB.
     */
    uint8_t* pokedex_seenC;

    /*!
     * @brief The first value that indicates if the National Pokédex is unlocked in
     *        Ruby/Sapphire/Emerald.
     *
     * This value should be set or unset with ::PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_A.
     *
     * Any action taken on this value should be taken on pksav_gba_save_t.nat_pokedex_unlockedB
     * and pksav_gba_save_t.nat_pokedex_unlockedC.
     */
    uint16_t* rse_nat_pokedex_unlockedA;

    /*!
     * @brief The first value that indicates if the National Pokédex is unlocked in
     *        FireRed/LeafGreen.
     *
     * This value should be set or unset with ::PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_A.
     *
     * Any action taken on this value should be taken on pksav_gba_save_t.nat_pokedex_unlockedB
     * and pksav_gba_save_t.nat_pokedex_unlockedC.
     */
    uint8_t* frlg_nat_pokedex_unlockedA;

    /*!
     * @brief The second value that indicates if the National Pokédex is unlocked.
     *
     * For Ruby/Sapphire/Emerald saves, this value should be set or unset with
     * ::PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_B. For FireRed/Leafgreen saves, this value
     * should be set with ::PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_B.
     *
     * For Ruby/Sapphire/Emerald saves, any action taken on this value should be taken
     * on pksav_gba_save_t.rse_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedC.
     *
     * For FireRed/LeafGreen saves, any action taken on this value should be taken
     * on pksav_gba_save_t.frlg_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedC.
     */
    uint8_t* nat_pokedex_unlockedB;

    /*!
     * @brief The third value that indicates if the National Pokédex is unlocked.
     *
     * For Ruby/Sapphire/Emerald saves, this value should be set or unset with
     * ::PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_MASK_C. For FireRed/Leafgreen saves, this value
     * should be set with ::PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_MASK_C.
     *
     * For Ruby/Sapphire/Emerald saves, any action taken on this value should be taken
     * on pksav_gba_save_t.rse_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedB.
     *
     * For FireRed/LeafGreen saves, any action taken on this value should be taken
     * on pksav_gba_save_t.frlg_nat_pokedex_unlockedA and pksav_gba_save_t.nat_pokedex_unlockedB.
     */
    uint16_t* nat_pokedex_unlockedC;

    //! @brief Which of the three game types this save corresponds to.
    pksav_gba_game_t gba_game;

    // Do not edit these
#ifndef __DOXYGEN__
    uint32_t security_key;
    uint8_t shuffled_section_nums[14];
    bool small_save;
    bool from_first_slot;
    pksav_gba_save_slot_t* unshuffled;
    uint8_t* raw;
#endif
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
 * \param result_out whether or not the buffer is a valid save
 * \returns ::PKSAV_ERROR_NONE upon success, no matter the result
 * \returns ::PKSAV_ERROR_NULL_POINTER if buffer or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_buffer_is_gba_save(
    const uint8_t* buffer,
    size_t buffer_len,
    pksav_gba_game_t gba_game,
    bool* result_out
);

/*!
 * @brief Checks if the given file is a valid Game Boy Advance save.
 *
 * \param filepath path of the file to check
 * \param gba_game which type of Game Boy Advance game to test for
 * \param result_out whether or not the file is a valid save
 * \returns ::PKSAV_ERROR_NONE upon success, no matter the result
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or result_out is NULL
 */
PKSAV_API pksav_error_t pksav_file_is_gba_save(
    const char* filepath,
    pksav_gba_game_t gba_game,
    bool* result_out
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
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gba_save is NULL
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
 * \returns ::PKSAV_ERROR_NULL_POINTER if filepath or gba_save is NULL
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
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_NULL_POINTER if gba_save is NULL
 */
PKSAV_API pksav_error_t pksav_gba_save_free(
    pksav_gba_save_t* gba_save
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GBA_SAVE_H */
