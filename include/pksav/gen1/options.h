/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_OPTIONS_H
#define PKSAV_GEN1_OPTIONS_H

// Common

#define PKSAV_GEN1_OPTIONS_BATTLE_STYLE_SET_MASK ((uint8_t)(1 << 6))
#define PKSAV_GEN1_OPTIONS_BATTLE_EFFECTS_DISABLE_MASK ((uint8_t)(1 << 7))

// Red/Blue

enum pksav_gen1_rb_text_speed
{
    PKSAV_GEN1_RB_TEXT_SPEED_FAST = 1,
    PKSAV_GEN1_RB_TEXT_SPEED_NORMAL = 3,
    PKSAV_GEN1_RB_TEXT_SPEED_SLOW = 5
};

#define PKSAV_GEN1_RB_OPTIONS_TEXT_SPEED_MASK ((uint8_t)7)
#define PKSAV_GEN1_RB_OPTIONS_SOUND_STEREO_MASK ((uint8_t)(1 << 4))

// Yellow

enum pksav_gen1_yellow_sound_option
{
    PKSAV_GEN1_YELLOW_SOUND_MONO = 0,
    PKSAV_GEN1_YELLOW_SOUND_EARPHONE1 = 1,
    PKSAV_GEN1_YELLOW_SOUND_EARPHONE2 = 2,
    PKSAV_GEN1_YELLOW_SOUND_EARPHONE3 = 3,
};

#define PKSAV_GEN1_YELLOW_OPTIONS_SOUND_OFFSET 4
#define PKSAV_GEN1_YELLOW_OPTIONS_SOUND_MASK ((uint8_t)(3 << PKSAV_GEN1_YELLOW_OPTIONS_SOUND_OFFSET))

#define PKSAV_GEN1_YELLOW_SOUND_OPTION(options_byte) \
    (((options_byte) >> PKSAV_GEN1_YELLOW_OPTIONS_SOUND_OFFSET) & \
     PKSAV_GEN1_YELLOW_OPTIONS_SOUND_MASK)

#endif /* PKSAV_GEN1_OPTIONS_H */
