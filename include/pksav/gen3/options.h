/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef pksav_gen3_OPTIONS_H
#define pksav_gen3_OPTIONS_H

// Button mode
// TODO: is this masked, or is the whole byte actually used for just this?

enum pksav_gen3_rse_button_mode
{
    pksav_gen3_RSE_BUTTON_MODE_NORMAL     = 0x01,
    pksav_gen3_RSE_BUTTON_MODE_LR         = 0x02,
    pksav_gen3_RSE_BUTTON_MODE_L_EQUALS_A = 0x03
};

enum pksav_gen3_frlg_button_mode
{
    pksav_gen3_FRLG_BUTTON_MODE_HELP       = 0x01,
    pksav_gen3_FRLG_BUTTON_MODE_LR         = 0x02,
    pksav_gen3_FRLG_BUTTON_MODE_L_EQUALS_A = 0x03
};

// Text speed, textbox frame

enum pksav_gen3_text_speed
{
    pksav_gen3_TEXT_SPEED_SLOW   = 0,
    pksav_gen3_TEXT_SPEED_MEDIUM = 1,
    pksav_gen3_TEXT_SPEED_FAST   = 2
};

#define pksav_gen3_OPTIONS_TEXT_SPEED_MASK ((uint8_t)7)

#define pksav_gen3_OPTIONS_TEXTBOX_FRAME_OFFSET (3)

#define pksav_gen3_OPTIONS_TEXTBOX_FRAME_MASK ((uint8_t)(31 << pksav_gen3_OPTIONS_TEXTBOX_FRAME_OFFSET))

#define pksav_gen3_OPTIONS_TEXTBOX_MIN_FRAME ((uint8_t)0)
#define pksav_gen3_OPTIONS_TEXTBOX_MAX_FRAME ((uint8_t)15)

#define pksav_gen3_OPTIONS_TEXTBOX_FRAME(options_byte) \
    ((uint8_t)(((options_byte) & pksav_gen3_OPTIONS_TEXTBOX_FRAME_MASK) >> pksav_gen3_OPTIONS_TEXTBOX_FRAME_OFFSET))

// Sound, battle style, battle scene

#define pksav_gen3_OPTIONS_SOUND_STEREO_MASK         ((uint8_t)1)
#define pksav_gen3_OPTIONS_BATTLE_STYLE_SET_MASK     ((uint8_t)(1 << 1))
#define pksav_gen3_OPTIONS_BATTLE_SCENE_DISABLE_MASK ((uint8_t)(1 << 2))

#endif /* pksav_gen3_OPTIONS_H */
