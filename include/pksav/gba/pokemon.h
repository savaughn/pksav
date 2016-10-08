/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_POKEMON_H
#define PKSAV_GBA_POKEMON_H

#include <pksav/config.h>

#include <pksav/common/contest_stats.h>
#include <pksav/common/trainer_id.h>

#include <stdint.h>

/*!
 * @brief The mask for determining if a Pokémon is an egg.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.iv_egg_ability
 * field to access or modify this bit.
 */
#define PKSAV_GBA_EGG_MASK (uint32_t)(1 << 30)

/*!
 * @brief The mask for determining which ability a Pokémon has.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.iv_egg_ability
 * field to access or modify this bit.
 */
#define PKSAV_GBA_ABILITY_MASK (uint32_t)(1 << 31)

/*!
 * @brief The mask for determining at which level a Pokémon was met.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.origin_info
 * field to access or modify this value.
 */
#define PKSAV_GBA_LEVEL_MET_MASK (uint16_t)(0x7F)

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
#define PKSAV_GBA_ORIGIN_GAME_MASK (uint16_t)(0x780)

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
#define PKSAV_GBA_BALL_MASK (uint16_t)(0x7800)

/*!
 * @brief The offset of the bits corresponding to a Pokémon's ball in its bitfield.
 *
 * See ::PKSAV_GBA_BALL_MASK for its usage.
 */
#define PKSAV_GBA_BALL_OFFSET 11

/*!
 * @brief The mask for determining the gender of a Pokémon's original trainer.
 *
 * Apply this mask to the pksav_gba_pokemon_misc_t.origin_info
 * field to access or modify this bit.
 */
#define PKSAV_GBA_OTGENDER_MASK (uint16_t)(1 << 15)

/*!
 * @brief The mask for determining which Coolness contests a Pokémon has won.
 *
 * The result is 3 bits that correspond to whether or not the Pokémon has won
 * various levels of the Coolness contests, as follows:
 *  * Bit 3 (MSB): Cool Hyper/Ultra
 *  * Bit 2: Cool Super/Great
 *  * Bit 1 (LSB): Cool
 *
 * To access this value, apply this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field.
 *
 * To modify this value, apply the inverse of this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, then logical-or the
 * result with the new value.
 */
#define PKSAV_GBA_COOL_MASK (uint32_t)(7)

/*!
 * @brief The mask for determining which Beauty contests a Pokémon has won.
 *
 * The result is 3 bits that correspond to whether or not the Pokémon has won
 * various levels of the Beauty contests, as follows:
 *  * Bit 3 (MSB): Beauty Hyper/Ultra
 *  * Bit 2: Beauty Super/Great
 *  * Bit 1 (LSB): Beauty
 *
 * To access this value, apply this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, and shift the result right
 * by ::PKSAV_GBA_BEAUTY_OFFSET.
 *
 * To modify this value, apply the inverse of this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, shift the new value left
 * by ::PKSAV_GBA_BEAUTY_OFFSET, then logical-or the result with the field.
 */
#define PKSAV_GBA_BEAUTY_MASK (uint32_t)(7 << PKSAV_GBA_BEAUTY_OFFSET)

/*!
 * @brief The offset of the bits corresponding to which Beauty contests a Pokémon has won.
 *
 * See ::PKSAV_GBA_BEAUTY_MASK for its usage.
 */
#define PKSAV_GBA_BEAUTY_OFFSET 3

/*!
 * @brief The mask for determining which Cuteness contests a Pokémon has won.
 *
 * The result is 3 bits that correspond to whether or not the Pokémon has won
 * various levels of the Cute contests, as follows:
 *  * Bit 3 (MSB): Cute Hyper/Ultra
 *  * Bit 2: Cute Super/Great
 *  * Bit 1 (LSB): Cute
 *
 * To access this value, apply this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, and shift the result right
 * by ::PKSAV_GBA_CUTE_OFFSET.
 *
 * To modify this value, apply the inverse of this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, shift the new value left
 * by ::PKSAV_GBA_CUTE_OFFSET, then logical-or the result with the field.
 */
#define PKSAV_GBA_CUTE_MASK (uint32_t)(7 << PKSAV_GBA_CUTE_OFFSET)

/*!
 * @brief The offset of the bits corresponding to which Cuteness contests a Pokémon has won.
 *
 * See ::PKSAV_GBA_CUTE_MASK for its usage.
 */
#define PKSAV_GBA_CUTE_OFFSET 6

/*!
 * @brief The mask for determining which Smartness contests a Pokémon has won.
 *
 * The result is 3 bits that correspond to whether or not the Pokémon has won
 * various levels of the Smart contests, as follows:
 *  * Bit 3 (MSB): Smart Hyper/Ultra
 *  * Bit 2: Smart Super/Great
 *  * Bit 1 (LSB): Smart
 *
 * To access this value, apply this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, and shift the result right
 * by ::PKSAV_GBA_SMART_OFFSET.
 *
 * To modify this value, apply the inverse of this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, shift the new value left
 * by ::PKSAV_GBA_SMART_OFFSET, then logical-or the result with the field.
 */
#define PKSAV_GBA_SMART_MASK (uint32_t)(7 << PKSAV_GBA_SMART_OFFSET)

/*!
 * @brief The offset of the bits corresponding to which Smartness contests a Pokémon has won.
 *
 * See ::PKSAV_GBA_SMART_MASK for its usage.
 */
#define PKSAV_GBA_SMART_OFFSET 9

/*!
 * @brief The mask for determining which Toughness contests a Pokémon has won.
 *
 * The result is 3 bits that correspond to whether or not the Pokémon has won
 * various levels of the Tough contests, as follows:
 *  * Bit 3 (MSB): Tough Hyper/Ultra
 *  * Bit 2: Tough Super/Great
 *  * Bit 1 (LSB): Tough
 *
 * To access this value, apply this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, and shift the result right
 * by ::PKSAV_GBA_TOUGH_OFFSET.
 *
 * To modify this value, apply the inverse of this mask to the
 * pksav_gba_pokemon_misc_t.ribbons_obedience field, shift the new value left
 * by ::PKSAV_GBA_TOUGH_OFFSET, then logical-or the result with the field.
 */
#define PKSAV_GBA_TOUGH_MASK (uint32_t)(7 << PKSAV_GBA_TOUGH_OFFSET)

/*!
 * @brief The offset of the bits corresponding to which Toughness contests a Pokémon has won.
 *
 * See ::PKSAV_GBA_TOUGH_MASK for its usage.
 */
#define PKSAV_GBA_TOUGH_OFFSET 12

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
#define PKSAV_GBA_OBEDIENCE_MASK (uint32_t)(1 << 31)

#pragma pack(push,1)

/*!
 * @brief Internal representation of Pokémon statistics that increase
 *        with growth in a Game Boy Advance game.
 */
typedef struct {
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
} pksav_gba_pokemon_growth_t;

/*!
 * @brief Internal representation of a Pokémon's moves and current PP in a
 *        Game Boy Advance game.
 */
typedef struct {
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
} pksav_gba_pokemon_attacks_t;

/*!
 * @brief Internal representation of a Pokémon's EVs and contest stats
 *        in a Game Boy Advance game.
 */
typedef struct {
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
    pksav_contest_stats_t contest_stats;
} pksav_gba_pokemon_effort_t;

/*!
 * @brief Internal representation of Pokémon information that doesn't fit in
 *        other structs.
 */
typedef struct {
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
} pksav_gba_pokemon_misc_t;

/*!
 * @brief The grouping of all Game Boy Advance Pokémon blocks.
 *
 * This union allows the data to be parsed in multiple ways, which is useful for
 * unshuffling and decryption.
 */
typedef union {
    //! Parse the blocks byte-by-byte.
    uint8_t blocks8[48];
    //! Parse the blocks in two-byte chunks.
    uint16_t blocks16[24];
    //! Parse the blocks in four-byte chunks.
    uint32_t blocks32[12];
    //! Parse individual blocks byte-by-byte.
    uint8_t blocks[4][12];
    /*!
     * @brief Parse the blocks in a consistent order once unshuffled.
     *
     * This order is completely arbitrary and is only laid out this way for
     * a convenient interface.
     */
    struct {
        //! Growth-related information.
        pksav_gba_pokemon_growth_t growth;
        //! Attacks and PP.
        pksav_gba_pokemon_attacks_t attacks;
        //! EVs.
        pksav_gba_pokemon_effort_t effort;
        //! Misc information.
        pksav_gba_pokemon_misc_t misc;
    };
} pksav_gba_pokemon_blocks_t;

/*!
 * @brief The internal representation of Pokémon information that's shown in both
 *        the party and PC.
 */
typedef struct {
    /*!
     * @brief The Pokémon's personality value, used to determine other values.
     *
     * This value is stored in little-endian, so use the function ::pksav_littleendian32
     * to access or modify it.
     */
    uint32_t personality;
    //! This Pokémon's original trainer's ID.
    pksav_trainer_id_t ot_id;
    /*!
     * @brief This Pokémon's nickname.
     *
     * This value should be accessed with ::pksav_text_from_gba or ::pksav_widetext_from_gba
     * and should be modified with ::pksav_text_to_gba or ::pksav_widetext_to_gba.
     *
     * In all cases, the num_chars parameter should be 10.
     */
    uint8_t nickname[10];
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
     * This value should be accessed with ::pksav_text_from_gba or ::pksav_widetext_from_gba
     * and should be modified with ::pksav_text_to_gba or ::pksav_widetext_to_gba.
     *
     * In all cases, the num_chars parameter should be 7.
     */
    uint8_t otname[7];
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
     * automatically sets all checksum.
     */
    uint16_t checksum;
    //! Unknown.
    uint16_t unknown_0x1E;
    //! Pokémon blocks.
    pksav_gba_pokemon_blocks_t blocks;
} pksav_gba_pc_pokemon_t;

typedef struct {
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
} pksav_gba_pokemon_party_data_t;

typedef struct {
    pksav_gba_pc_pokemon_t pc;
    pksav_gba_pokemon_party_data_t party_data;
} pksav_gba_party_pokemon_t;

typedef struct {
    uint32_t count;
    pksav_gba_party_pokemon_t party[6];
} pksav_gba_pokemon_party_t;

typedef struct {
    pksav_gba_pc_pokemon_t entries[30];
} pksav_gba_pokemon_box_t;

typedef struct {
    uint32_t current_box;
    pksav_gba_pokemon_box_t boxes[14];
    uint8_t box_names[14][9];
    uint8_t wallpapers[14];
} pksav_gba_pokemon_pc_t;

#pragma pack(pop)

#endif /* PKSAV_GBA_POKEMON_H */
