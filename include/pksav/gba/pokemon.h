/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_POKEMON_H
#define PKSAV_GBA_POKEMON_H

#include <pksav/config.h>

#include <pksav/common/contest_stats.h>
#include <pksav/common/trainer_id.h>

#include <pksav/gba/ribbons.h>

#include <stdint.h>

#define PKSAV_GBA_BOX_NUM_POKEMON 30
#define PKSAV_GBA_PARTY_NUM_POKEMON 6

#define PKSAV_GBA_POKEMON_NICKNAME_LENGTH 10
#define PKSAV_GBA_POKEMON_OTNAME_LENGTH 7

#define PKSAV_GBA_NUM_POKEMON_BOXES 14
#define PKSAV_GBA_POKEMON_BOX_NAME_LENGTH 8

/*!
 * @brief The mask for determining if a Pokémon is an egg.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.iv_egg_ability
 * field to access or modify this bit.
 */
#define PKSAV_GBA_POKEMON_EGG_MASK ((uint32_t)(1 << 30))

/*!
 * @brief The mask for determining which ability a Pokémon has.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.iv_egg_ability
 * field to access or modify this bit.
 */
#define PKSAV_GBA_POKEMON_ABILITY_MASK ((uint32_t)(1 << 31))

/*!
 * @brief The mask for determining at which level a Pokémon was met.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.origin_info
 * field to access or modify this value.
 */
#define PKSAV_GBA_POKEMON_LEVEL_MET_MASK ((uint16_t)(0x7F))

/*!
 * @brief The mask for determining a Pokémon's origin game.
 *
 * To access the value, apply the mask to the
 * pksav_gba_pokemon_misc_t.origin_info field and shift the result
 * right by ::PKSAV_GBA_ORIGIN_GAME_OFFSET.
 *
 * To modify the value, apply the inverse of the mask to the
 * pksav_gba_pokemon_misc_t.origin_info field. Then shift the new
 * value left by ::PKSAV_GBA_ORIGIN_GAME_OFFSET and perform a logical-or
 * with this value and the field.
 */
#define PKSAV_GBA_POKEMON_ORIGIN_GAME_MASK ((uint16_t)(0x780))

/*!
 * @brief The offset of the bits corresponding to a Pokémon's origin game in its bitfield.
 *
 * See ::PKSAV_GBA_ORIGIN_GAME_MASK for it usage.
 */
#define PKSAV_GBA_ORIGIN_GAME_OFFSET 7

/*!
 * @brief The mask for determining which Poké Ball was used to catch a Pokémon.
 *
 * To access the value, apply the mask to the
 * pksav_gba_pokemon_misc_t.origin_info field and shift the result
 * right by ::PKSAV_GBA_BALL_OFFSET.
 *
 * To modify the value, apply the inverse of the mask to the
 * pksav_gba_pokemon_misc_t.origin_info field. Then shift the new
 * value left by ::PKSAV_GBA_BALL_OFFSET and perform a logical-or
 * with this value and the field.
 */
#define PKSAV_GBA_POKEMON_BALL_MASK ((uint16_t)(0x7800))

/*!
 * @brief The offset of the bits corresponding to a Pokémon's ball in its bitfield.
 *
 * See ::PKSAV_GBA_BALL_MASK for its usage.
 */
#define PKSAV_GBA_POKEMON_BALL_OFFSET 11

/*!
 * @brief The mask for determining the gender of a Pokémon's original trainer.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.origin_info
 * field to access or modify this bit.
 */
#define PKSAV_GBA_OTGENDER_MASK ((uint16_t)(1 << 15))

/*!
 * @brief The mask for determining whether a Mew or Deoxys will be obedient.
 *
 * If this bit is not set, a Mew or Deoxys will not listen to the player, no
 * matter how many badges he/she has. This field has no effect in any other
 * Pokémon.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.ribbons_obedience field
 * to modify this bit.
 */
#define PKSAV_GBA_POKEMON_OBEDIENCE_MASK ((uint32_t)(1 << 31))

#pragma pack(push,1)

#define PKSAV_GBA_POKEMON_PP_UP(field,move_num) \
    (((field) >> (move_num * 2)) & 3)

/*!
 * @brief Internal representation of Pokémon statistics that increase
 *        with growth in a Game Boy Advance game.
 */
struct pksav_gba_pokemon_growth_block
{
    /*!
     * @brief The Pokémon's species index.
     *
     * The index may not correspond to its National Pokédex number.
     *
     * This value is stored in little-endian, so access and modify it with
     * the function ::pksav_littleendian16.
     */
    uint16_t species;
    /*!
     * @brief The index for the Pokémon's held item.
     *
     * If the Pokémon is not holding any item, this field will be set to 0.
     *
     * This value is stored in little-endian, so access and modify it with
     * the function ::pksav_littleendian16.
     */
    uint16_t held_item;
    /*!
     * @brief The Pokémon's experience.
     *
     * This value is stored in little-endian, so access and modify it with
     * the function ::pksav_littleendian32.
     */
    uint32_t exp;
    /*!
     * @brief The number of PP Ups used for each move, stored in a bitfield.
     *
     * A PP Max being used on a move automatically sets the value to 3.
     *
     * The bitfield is arranges as follows:
     *  * 0-1: Move 1 PP
     *  * 2-3: Move 2 PP
     *  * 4-5: Move 3 PP
     *  * 6-7: Move 4 PP
     */
    uint8_t pp_up;
    //! The Pokémon's happiness/friendship (0-255).
    uint8_t friendship;
    //! Unknown.
    uint16_t unknown_0xA;
};

/*!
 * @brief Internal representation of a Pokémon's moves and current PP in a
 *        Game Boy Advance game.
 */
struct pksav_gba_pokemon_attacks_block
{
    /*!
     * @brief Indices of the Pokémon's moves.
     *
     * If a move slot does not have a move, its value will be 0.
     *
     * These values are stored in little-endian, so access and modify them
     * with the function ::pksav_littleendian16.
     */
    uint16_t moves[4];
    /*!
     * @brief The PP for each of the Pokémon's moves.
     *
     * The maximum value for each slot is dependent on the move.
     */
    uint8_t move_pps[4];
};

/*!
 * @brief Internal representation of a Pokémon's EVs and contest stats
 *        in a Game Boy Advance game.
 */
struct pksav_gba_pokemon_effort_block
{
    //! A Pokémon's HP EV.
    uint8_t ev_hp;
    //! A Pokémon's Attack EV.
    uint8_t ev_atk;
    //! A Pokémon's Defense EV.
    uint8_t ev_def;
    //! A Pokémon's Speed EV.
    uint8_t ev_spd;
    //! A Pokémon's Special Attack EV.
    uint8_t ev_spatk;
    //! A Pokémon's Special Defense EV.
    uint8_t ev_spdef;
    //! A Pokémon's contest stats.
    struct pksav_contest_stats contest_stats;
};

#define PKSAV_GBA_POKEMON_ORIGIN_GAME(field) \
    ((field) & PKSAV_GBA_POKEMON_ORIGIN_GAME_MASK)

#define PKSAV_GBA_POKEMON_BALL(field) \
    ((field) & PKSAV_GBA_POKEMON_BALL_MASK)

/*!
 * @brief Internal representation of Pokémon information that doesn't fit in
 *        other structs.
 */
struct pksav_gba_pokemon_misc_block
{
    /*!
     * @brief The Pokémon's Pokérus strain and duration.
     *
     * See <pksav/common/pokerus.h> for more details.
     */
    uint8_t pokerus;
    //! Where the Pokémon was originally met.
    uint8_t met_location;
    /*!
     * @brief Bitfield containing information about how the Pokémon was caught.
     *
     * The bitfield is arranged as follows:
     *  * 0-6: Level met (0-100)
     *  * 7-10: Origin game
     *  * 11-14: Ball
     *  * 15: OT gender (0 if male, 1 if female)
     */
    uint16_t origin_info;
    /*!
     * @brief Bitfield containing the Pokémon's IVs, egg status, and ability.
     *
     * The bitfield is arranged as follows:
     *  * 0-4: HP IV (0-31)
     *  * 5-9: Attack IV (0-31)
     *  * 10-14: Defense IV (0-31)
     *  * 15-19: Speed IV (0-31)
     *  * 20-24: Special Attack IV (0-31)
     *  * 25-29: Special Defense IV (0-31)
     *  * 30: 0 if this Pokémon is hatched, 1 if it is an egg
     *  * 31: 0 if the Pokémon has its first ability, 1 if it has its second
     */
    uint32_t iv_egg_ability;
    /*!
     * @brief Bitfield containing the Pokémon's ribbons and obedience information.
     *
     * The enums in <pksav/common/gen3_ribbons.h> can be used to set or unset
     * these values.
     *
     * The bitfield is arranged as follows:
     *  * 0-2: Cool contest level
     *  * 3-5: Beauty contest level
     *  * 6-8: Cute contest level
     *  * 9-11: Smart contest level
     *  * 12-14: Tough contest level
     *  * 15: Champion ribbon
     *  * 16: Winning ribbon
     *  * 17: Victory ribbon
     *  * 18: Artist ribbon
     *  * 19: Effort ribbon
     *  * 20: Marine ribbon
     *  * 21: Land ribbon
     *  * 22: Sky ribbon
     *  * 23: Country ribbon
     *  * 24: National ribbon
     *  * 25: Earth ribbon
     *  * 26: World ribbon
     *  * 31: needs to be set to 1 for a Mew or Deoxys to be obedient
     */
    uint32_t ribbons_obedience;
};

/*!
 * @brief The grouping of all Game Boy Advance Pokémon blocks.
 */
struct pksav_gba_pokemon_blocks
{
    //! Growth-related information.
    struct pksav_gba_pokemon_growth_block growth;
    //! Attacks and PP.
    struct pksav_gba_pokemon_attacks_block attacks;
    //! EVs.
    struct pksav_gba_pokemon_effort_block effort;
    //! Misc information.
    struct pksav_gba_pokemon_misc_block misc;
};

/*!
 * @brief The internal representation of Pokémon information that's shown in both
 *        the party and PC.
 */
struct pksav_gba_pc_pokemon
{
    /*!
     * @brief The Pokémon's personality value, used to determine other values.
     *
     * This value is stored in little-endian, so use the function ::pksav_littleendian32
     * to access or modify it.
     */
    uint32_t personality;
    //! This Pokémon's original trainer's ID.
    union pksav_trainer_id ot_id;
    /*!
     * @brief This Pokémon's nickname.
     *
     * This value should be accessed with ::pksav_text_from_gba and modified with
     * ::pksav_text_to_gba.
     *
     * In all cases, the num_chars parameter should be 10.
     */
    uint8_t nickname[PKSAV_GBA_POKEMON_NICKNAME_LENGTH];
    /*!
     * @brief The language of this Pokémon's original game.
     *
     * This value is stored in little-endian, so use the function ::pksav_littleendian16 to
     * access or modify it.
     */
    uint16_t language;
    /*!
     * @brief This Pokémon's nickname.
     *
     * This value should be accessed with ::pksav_text_from_gba and modified with
     * ::pksav_text_to_gba.
     *
     * In all cases, the num_chars parameter should be 7.
     */
    uint8_t otname[PKSAV_GBA_POKEMON_OTNAME_LENGTH];
    /*!
     * @brief This Pokémon's trainer-set markings.
     *
     * These markings do not mean anything in particular and are primarily used to sort
     * Pokémon in the PC.
     */
    uint8_t markings;
    /*!
     * @brief The checksum of the Pokémon blocks.
     *
     * Users should never need to access or modify this value, as ::pksav_gba_save_save
     * automatically sets all checksums.
     */
    uint16_t checksum;
    //! Unknown.
    uint16_t unknown_0x1E;
    //! Pokémon blocks.
    struct pksav_gba_pokemon_blocks blocks;
};

struct pksav_gba_pokemon_party_data
{
    uint32_t condition;
    uint8_t level;
    uint8_t pokerus_time;
    uint16_t current_hp;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spatk;
    uint16_t spdef;
};

struct pksav_gba_party_pokemon
{
    struct pksav_gba_pc_pokemon pc_data;
    struct pksav_gba_pokemon_party_data party_data;
};

struct pksav_gba_pokemon_party
{
    uint32_t count;
    struct pksav_gba_party_pokemon party[PKSAV_GBA_PARTY_NUM_POKEMON];
};

struct pksav_gba_pokemon_box
{
    struct pksav_gba_pc_pokemon entries[PKSAV_GBA_BOX_NUM_POKEMON];
};

struct pksav_gba_pokemon_pc
{
    uint32_t current_box;
    struct pksav_gba_pokemon_box boxes[PKSAV_GBA_NUM_POKEMON_BOXES];
    uint8_t box_names[PKSAV_GBA_NUM_POKEMON_BOXES][PKSAV_GBA_POKEMON_BOX_NAME_LENGTH + 1];
    uint8_t wallpapers[PKSAV_GBA_NUM_POKEMON_BOXES];
};

#pragma pack(pop)

#endif /* PKSAV_GBA_POKEMON_H */
