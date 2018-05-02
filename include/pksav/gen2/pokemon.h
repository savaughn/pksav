/*
 * Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_POKEMON_H
#define PKSAV_GEN2_POKEMON_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <time.h>

#define PKSAV_GEN2_POKEMON_EGG_INDEX (0xFD)

#define PKSAV_GEN2_BOX_NUM_POKEMON (20)
#define PKSAV_GEN2_PARTY_NUM_POKEMON (6)

#define PKSAV_GEN2_POKEMON_NICKNAME_LENGTH (10)

#define PKSAV_GEN2_POKEMON_OTNAME_LENGTH (7)
#define PKSAV_GEN2_POKEMON_OTNAME_STORAGE_LENGTH (10)

#define PKSAV_GEN2_NUM_POKEMON_BOXES (14)
#define PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH (8)

#define PKSAV_GEN2_POKEMON_NUM_MOVES (4)

#define PKSAV_GEN2_POKEMON_EXPERIENCE_BUFFER_SIZE (3)

/*!
 * @brief The mask for a move's PP in the PP field.
 *
 * Mask the value of one of the indices of struct pksav_gen2_pc_pokemon.move_pps to
 * get the PP of that move.
 */
#define PKSAV_GEN2_POKEMON_MOVE_PP_MASK ((uint8_t)0x3F)

/*!
 * @brief The mask for the number of PP Ups used on a move (0-3).
 *
 * Mask the value of one of the indices of struct pksav_gen2_pc_pokemon.move_pps
 * and shift it right by ::PKSAV_GEN2_POKEMON_MOVE_PP_UP_MASK to get the number of PP
 * Ups used.
 */
#define PKSAV_GEN2_POKEMON_MOVE_PP_UP_MASK ((uint8_t)0xC0)

/*!
 * @brief The offset of the number of PP UPs used on a move.
 *
 * Mask the value of one of the indices of struct pksav_gen2_pc_pokemon.move_pps
 * with ::PKSAV_GEN2_POKEMON_MOVE_PP_UP_OFFSET and shift it right by this to get the
 * number of PP Ups used.
 */
#define PKSAV_GEN2_POKEMON_MOVE_PP_UP_OFFSET (6)

#define PKSAV_GEN2_POKEMON_MOVE_PP_UP(field) \
    (((field) & PKSAV_GEN2_POKEMON_MOVE_PP_UP_MASK) >> PKSAV_GEN2_POKEMON_MOVE_PP_UP_OFFSET)

/*!
 * @brief The mask for the level at which a Pokémon was caught.
 *
 * Mask struct pksav_gen2_pc_pokemon.caught_data with this mask and shift it
 * right by ::PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_OFFSET to get the value.
 */
#define PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_MASK   ((uint16_t)0x3F00)

/*!
 * @brief The offset for the level at which a Pokémon was caught.
 *
 * Mask struct pksav_gen2_pc_pokemon.caught_data with ::PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_MASK
 * and shift it right by this offset to get the value.
 */
#define PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_OFFSET (8)

#define PKSAV_GEN2_POKEMON_LEVEL_CAUGHT(field) \
    (((field) & PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_MASK) >> PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_OFFSET)

/*!
 * @brief The mask for setting a Pokémon's original trainer's gender.
 *
 * Mask struct pksav_gen2_pc_pokemon.caught_data with this mask to set the trainer's
 * gender to female. Unmask it to set the trainer's gender to male.
 */
#define PKSAV_GEN2_POKEMON_OT_GENDER_MASK ((uint16_t)0x0080)

/*!
 * @brief The mask for the index of the location at which the Pokémon was caught.
 *
 * Mask struct pksav_gen2_pc_pokemon.caught_data with this mask to get or set the
 * location index.
 */
#define PKSAV_GEN2_POKEMON_LOCATION_MASK ((uint16_t)0x007F)

#pragma pack(push,1)

/*!
 * @brief Native format for the persistent data of a Pokémon in Generation II.
 *
 * This data is available both when the Pokémon is in the trainer's party or in the
 * PC.
 */
struct pksav_gen2_pc_pokemon
{
    /*!
     * @brief Species index.
     *
     * This value is the same whether or not the Pokémon is in an egg. If this is
     * the case, the value is reflected in struct pksav_gen2_pokemon_party.species or
     * struct pksav_gen2_pokemon_box.species.
     */
    uint8_t species;
    /*!
     * @brief Held item index.
     *
     * When a Generation I Pokémon is traded into a Generation II game, the
     * struct pksav_gen2_pc_pokemon.catch_rate field is placed here. As such, there is
     * a set held uitem corresponding to each species when trading up.
     */
    uint8_t held_item;
    /*!
     * @brief Indices for each of this Pokémon's moves.
     */
    uint8_t moves[PKSAV_GEN2_POKEMON_NUM_MOVES];
    /*!
     * @brief The Pokémon's original trainer's ID (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ot_id;
    /*!
     * @brief The Pokémon's current total experience points (stored in Base-256).
     *
     * This value should be accessed with ::pksav_import_base256 (with a num_bytes
     * value of 3) and set with ::pksav_export_base256.
     */
    uint8_t exp[PKSAV_GEN2_POKEMON_EXPERIENCE_BUFFER_SIZE];
    /*!
     * @brief The Pokémon's HP EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_hp;
    /*!
     * @brief The Pokémon's Attack EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_atk;
    /*!
     * @brief The Pokémon's Defense EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_def;
    /*!
     * @brief The Pokémon's Speed EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spd;
    /*!
     * @brief The Pokémon's Special EV stat (stored in big-endian).
     *
     * The Special stat was removed in Generation II, but this field is kept in
     * for compatibility with Generation I.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spcl;
    /*!
     * @brief The Pokémon's IV's, stored as a bitset.
     *
     * These values should be accessed with ::pksav_get_gb_IV and set
     * with ::pksav_set_gb_IV.
     */
    uint16_t iv_data;
    /*!
     * @brief The Pokémon's PPs and number of PP Ups used for each move.
     *
     * Mask an index with ::PKSAV_GEN2_MOVE_PP_MASK to get the move PP.
     *
     * Mask an index with ::PKSAV_GEN2_MOVE_PP_UP_MASK to get the number of
     * PP Ups applied to the move.
     */
    uint8_t move_pps[PKSAV_GEN2_POKEMON_NUM_MOVES];
    /*!
     * @brief A Pokémon's friendship/happiness value.
     *
     * This value is used in certain in-game events and is used to determine the power
     * of Return and Frustration.
     */
    uint8_t friendship;
    /*!
     * @brief The Pokémon's Pokérus strain and duration.
     *
     * See <pksav/common/pokerus.h> for more details.
     */
    uint8_t pokerus;
    /*!
     * @brief Data about where and when a Pokémon was caught.
     *
     * The bitfield is arranged as follows:
     *  * 0-6: location caught
     *  * 7: 0 if original trainer is male, 1 if original trainer is female
     *  * 8-13: level caught
     *  * 14-15: time caught
     *    * 1: 4:00 AM - 9:59 AM
     *    * 2: 10:00 AM - 5:59 PM
     *    * 3: 6:00 PM - 3:59 AM
     *
     * This data is only set in Crystal. Pokémon caught in Gold/Silver or traded up from
     * Generation I will have this value set to 0.
     */
    uint16_t caught_data;
    /*!
     * @brief The Pokémon's level.
     */
    uint8_t level;
};

/*!
 * @brief Data generated when a Pokémon is added to a trainer's party in Generation III.
 *
 * All of this information is generated from values stored in
 * struct pksav_gen2_pc_pokemon.
 */
struct pksav_gen2_pokemon_party_data
{
    /*!
     * @brief The Pokémon's status ailments, if any.
     *
     * The enum ::struct pksav_gb_condition contains all valid values for this field.
     */
    uint8_t condition;
    //! Unused.
    uint8_t unused;
    /*!
     * @brief The Pokémon's current HP.
     *
     * This value should never be higher than struct pksav_gen2_pokemon_party_data.max_hp.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t current_hp;
    /*!
     * @brief The Pokémon's maximum HP.
     *
     * This value is calculated using the Pokémon's base HP stat, HP
     * EV, and HP IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t max_hp;
    /*!
     * @brief The Pokémon's Attack.
     *
     * This value is calculated using the Pokémon's base Attack stat, Attack
     * EV, and Attack IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t atk;
    /*!
     * @brief The Pokémon's Defense.
     *
     * This value is calculated using the Pokémon's base Defense stat, Defense
     * EV, and Defense IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t def;
    /*!
     * @brief The Pokémon's Speed.
     *
     * This value is calculated using the Pokémon's base Speed stat, Speed
     * EV, and Speed IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spd;
    /*!
     * @brief The Pokémon's Special Attack.
     *
     * This value is calculated using the Pokémon's base Special Attack stat,
     * Special EV, and Special IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spatk;
    /*!
     * @brief The Pokémon's Special Defense.
     *
     * This value is calculated using the Pokémon's base Special Defense stat,
     * Special EV, and Special IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spdef;
};

//! Native format for a Pokémon in the trainer's party in Generation III.
struct pksav_gen2_party_pokemon
{
    /*!
     * @brief PC data.
     *
     * This data is accessible whether the Pokémon is in the PC or party.
     */
    struct pksav_gen2_pc_pokemon pc_data;

    /*!
     * @brief Party data.
     *
     * This data is generated when the Pokémon is moved into the party. As
     * all of it can be generated from the PC data, it is not stored until
     * it needs to be used.
     */
    struct pksav_gen2_pokemon_party_data party_data;
};

//! Native format for a trainer's Pokémon party in Generation II.
struct pksav_gen2_pokemon_party
{
    //! The actual number of Pokémon in the party (0-6).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the party.
     *
     * When the party is viewed in-game, it is this value that determines
     * what Pokémon is shown, not the struct pksav_gen2_pc_pokemon.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in struct pksav_gen2_pc_pokemon will correspond to its species when hatched.
     *
     * The first index after the last Pokémon in the box should always be
     * set to 0xFF.
     */
    uint8_t species[PKSAV_GEN2_PARTY_NUM_POKEMON + 1];
    //! The actual Pokémon in the party.
    struct pksav_gen2_party_pokemon party[PKSAV_GEN2_PARTY_NUM_POKEMON];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t otnames[PKSAV_GEN2_PARTY_NUM_POKEMON][PKSAV_GEN2_POKEMON_OTNAME_STORAGE_LENGTH + 1];
    /*!
     * @brief The nicknames of each Pokémon in the party.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t nicknames[PKSAV_GEN2_PARTY_NUM_POKEMON][PKSAV_GEN2_POKEMON_NICKNAME_LENGTH + 1];
};

//! Native format for a Pokémon PC box in Generation II.
struct pksav_gen2_pokemon_box
{
    //! The actual number of Pokémon in the box (0-20).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the box.
     *
     * When the box is viewed in-game, it is this value that determines
     * what Pokémon is showed, not the struct pksav_gen2_pc_pokemon.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in struct pksav_gen2_pc_pokemon will correspond to its species when hatched.
     *
     * The first index after the last Pokémon in the box should always be
     * set to 0xFF.
     */
    uint8_t species[PKSAV_GEN2_BOX_NUM_POKEMON + 1];
    //! The actual Pokémon in the box.
    struct pksav_gen2_pc_pokemon entries[PKSAV_GEN2_BOX_NUM_POKEMON];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t otnames[PKSAV_GEN2_BOX_NUM_POKEMON][PKSAV_GEN2_POKEMON_OTNAME_STORAGE_LENGTH + 1];
    /*!
     * @brief The nicknames of each Pokémon in the box.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t nicknames[PKSAV_GEN2_BOX_NUM_POKEMON][PKSAV_GEN2_POKEMON_NICKNAME_LENGTH + 1];
    //! Padding.
    uint8_t padding[2];
};

//! List of Pokémon box names in Generation II.
struct pksav_gen2_pokemon_box_names
{
    /*!
     * @brief Each box name.
     *
     * To access a name, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 9.
     */
    uint8_t names[PKSAV_GEN2_NUM_POKEMON_BOXES][PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH + 1];
};

#pragma pack(pop)

#endif /* PKSAV_GEN2_POKEMON_H */
