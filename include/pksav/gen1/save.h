/*!
 * @file    pksav/gen1/save.h
 * @ingroup PKSav
 * @brief   Functions for interacting with Generation I save files.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pksav/gen1/text.h>

#include <pksav/math/bcd.h>
#include <pksav/math/endian.h>

#include <stdbool.h>
#include <stdint.h>

#pragma pack(push,1)

//! Native representation of how much time has passed in a Generation I game.
typedef struct {
    /*!
     * @brief The number of hours (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t hours;
    //! The number of minutes (0-59).
    uint8_t minutes;
    //! The number of second (0-59).
    uint8_t seconds;
} pksav_gen1_time_t;

#pragma pack(pop)

/*
 * Savefile size and locations of fields within file
 */

#define PKSAV_GEN1_SAVE_SIZE 0x8000

//! The offsets of various fields in a Generation I save file.
typedef enum {
    /*!
     * @brief Where the player's name is stored in-game.
     *
     * This field pksav_gen1_save_t.trainer_name points to this location.
     */
    PKSAV_GEN1_PLAYER_NAME             = 0x2598,
    PKSAV_GEN1_POKEDEX_OWNED           = 0x25A3,
    PKSAV_GEN1_POKEDEX_SEEN            = 0x25B6,
    /*!
     * @brief Where the player's item bag is stored in-game.
     *
     * This field pksav_gen1_save_t.item_bag points to this location.
     */
    PKSAV_GEN1_ITEM_BAG                = 0x25C9,
    /*!
     * @brief Where the player's money amount is stored in-game.
     *
     * This field pksav_gen1_save_t.money points to this location.
     */
    PKSAV_GEN1_MONEY                   = 0x25F3,
    /*!
     * @brief Where the rival's name is stored in-game.
     *
     * This field pksav_gen1_save_t.rival_name points to this location.
     */
    PKSAV_GEN1_RIVAL_NAME              = 0x25F6,
    PKSAV_GEN1_OPTIONS                 = 0x2601,
    /*!
     * @brief Where which badges the player has earned is stored in-game.
     *
     * This field pksav_gen1_save_t.badges points to this location.
     */
    PKSAV_GEN1_BADGES                  = 0x2602,
    /*!
     * @brief Where the player's trainer ID is stored in-game.
     *
     * This field pksav_gen1_save_t.trainer_id points to this location.
     */
    PKSAV_GEN1_PLAYER_ID               = 0x2605,
    /*!
     * @brief Where the friendship value for the player's Pikachu is stored in-game (Yellow only).
     *
     * In Pokémon Red/Blue, this value is unused and is always set to 0.
     *
     * This field pksav_gen1_save_t.pikachu_friendship points to this location.
     */
    PKSAV_GEN1_PIKACHU_FRIENDSHIP      = 0x271C,
    /*!
     * @brief Where the player's item PC box is stored in-game.
     *
     * This field pksav_gen1_save_t.item_pc points to this location.
     */
    PKSAV_GEN1_ITEM_PC                 = 0x27E6,
    PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM = 0x284C,
    /*!
     * @brief Where the player's amount of casino coins is stored in-game.
     *
     * This field pksav_gen1_save_t.casino_coins points to this location.
     */
    PKSAV_GEN1_CASINO_COINS            = 0x2850,
    /*!
     * @brief Where how long the save file has been played is stored in-game.
     *
     * This field pksav_gen1_save_t.time_played points to this location.
     */
    PKSAV_GEN1_TIME_PLAYED             = 0x2CED,
    /*!
     * @brief Where the player's Pokémon party is stored in-game.
     *
     * This field pksav_gen1_save_t.trainer_id points to this location.
     */
    PKSAV_GEN1_POKEMON_PARTY           = 0x2F2C,
    PKSAV_GEN1_CURRENT_POKEMON_BOX     = 0x30C0,
    //! Where the save file's checksum is stored in-game.
    PKSAV_GEN1_CHECKSUM                = 0x3523,
    //! Where the first six Pokémon PC boxes are stored in-game.
    PKSAV_GEN1_POKEMON_PC_FIRST_HALF   = 0x4000,
    //! Where the last six Pokémon PC boxes are stored in-game.
    PKSAV_GEN1_POKEMON_PC_SECOND_HALF  = 0x6000
} pksav_gen1_save_offset_t;

/*!
 * @brief Bitmasks for checking if the player has a specific badge.
 *
 * The pksav_gen1_save_t.badges field points to the byte in the save file
 * that stores this bitmap. Use these enum values on this byte to access and
 * set whether or not the player has this badge.
 */
typedef enum {
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
} pksav_gen1_badge_t;

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
typedef struct {
    //! A pointer to the trainer's Pokémon party.
    pksav_gen1_pokemon_party_t* pokemon_party;
    /*!
     * @brief Pointers to the trainer's Pokémon boxes.
     *
     * The boxes are not stored contiguously in the save file, so these pointers
     * point to their actual positions in the file.
     */
    pksav_gen1_pokemon_box_t* pokemon_boxes[12];

    //! A pointer to the trainer's item bag.
    pksav_gen1_item_bag_t* item_bag;
    //! A pointer to the trainer's item PC.
    pksav_gen1_item_pc_t* item_pc;

    //! A pointer to the amount of time this save file has been played.
    pksav_gen1_time_t* time_played;
    /*!
     * @brief A pointer to the trainer's ID (stored in big-endian).
     *
     * This value should be accessed with ::pksav_bigendian16.
     */
    uint16_t* trainer_id;

    /*!
     * @brief A pointer to how much money the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 3. It should be set with ::pksav_to_bcd, with a maximum value of 999999.
     */
    uint8_t* money;
    /*!
     * @brief A pointer to how many casino coins the trainer has (stored in BCD).
     *
     * This value should be accessed with ::pksav_from_bcd, with a num_bytes value
     * of 2. It should be set with ::pksav_to_bcd, with a maximum value of 999.
     */
    uint8_t* casino_coins;

    /*!
     * @brief A pointer to the trainer's name.
     *
     * This value should be accessed with ::pksav_text_from_gen1 or
     * ::pksav_widetext_from_gen1, both with a num_chars value of 7.
     *
     * This value should be set with ::pksav_text_to_gen1 or
     * ::pksav_widetext_to_gen1, both with a num_chars value of 7.
     */
    uint8_t* trainer_name;
    /*!
     * @brief A pointer to the rival's name.
     *
     * This value should be accessed with ::pksav_text_from_gen1 or
     * ::pksav_widetext_from_gen1, both with a num_chars value of 7.
     *
     * This value should be set with ::pksav_text_to_gen1 or
     * ::pksav_widetext_to_gen1, both with a num_chars value of 7.
     */
    uint8_t* rival_name;

    /*!
     * @brief A pointer to the list of badges the trainer has earned.
     *
     * This value should be manipulated with the bitmasks given in the
     * ::pksav_gen1_badge_t enum.
     */
    uint8_t* badges;

    /*!
     * @brief A pointer to Pikachu's friendship level in Pokémon Yellow.
     *
     * In Pokémon Red/Blue, this field is unused and is set to 0.
     */
    uint8_t* pikachu_friendship;

    /*!
     * @brief Whether or not this save file corresponds to a Pokémon Yellow game.
     *
     * This is determined by ::pksav_gen1_save_load by examining the Pikachu friendship
     * value. As this value is always 0 in Pokémon Red/Blue, the save file is determined
     * to be from Pokémon Yellow if this value is non-zero. This is reliable most of the
     * time, but technically, if the trainer's Pikachu despises him enough for this value
     * to be 0, this save file will be considered to be from Pokémon Red/Blue.
     */
    bool yellow;

    /*!
     * @brief The raw binary data for the save file.
     *
     * You should never need to edit this field, as the pointers described above should
     * point to any areas of interest.
     */
    uint8_t* raw;
} pksav_gen1_save_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Determines whether the file at the given path is a valid Generation I save file.
 *
 * This is determined simply by checking the file's checksum.
 *
 * \param filepath path to the file to be checked
 * \returns true if the file is a valid Generation I save file, false otherwise
 */
PKSAV_API bool pksav_file_is_gen1_save(
    const char* filepath
);

/*!
 * @brief Loads the save file at the given path into the given PKSav struct.
 *
 * Upon a failure state, the given pksav_gen1_save_t will be left in an undefined state.
 *
 * \param filepath path to the file to be loaded
 * \param gen1_save PKSav struct in which to load file
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs reading the file
 * \returns ::PKSAV_ERROR_INVALID_SAVE if the given file is not a valid Generation I save file
 */
PKSAV_API pksav_error_t pksav_gen1_save_load(
    const char* filepath,
    pksav_gen1_save_t* gen1_save
);

/*!
 * @brief Saves a Generation I save file to the given path.
 *
 * Upon a failure state, the save file is not guaranteed to have been properly written.
 *
 * \param filepath where the save file should be written
 * \param gen1_save the same to be written
 * \returns ::PKSAV_ERROR_NONE upon success
 * \returns ::PKSAV_ERROR_FILE_IO if a problem occurs writing the file
 */
PKSAV_API pksav_error_t pksav_gen1_save_save(
    const char* filepath,
    pksav_gen1_save_t* gen1_save
);

/*!
 * @brief Frees memory allocated for a pksav_gen1_save_t.
 *
 * \param gen1_save save whose memory should be freed
 */
static PKSAV_INLINE void pksav_gen1_save_free(
    pksav_gen1_save_t* gen1_save
) {
    free(gen1_save->raw);
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_SAVE_H */
