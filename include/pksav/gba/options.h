/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_OPTIONS_H
#define PKSAV_GBA_OPTIONS_H

// Button mode
// TODO: is this masked, or is the whole byte actually used for just this?

enum pksav_gba_rse_button_mode
{
    PKSAV_GBA_RSE_BUTTON_MODE_NORMAL     = 0x01,
    PKSAV_GBA_RSE_BUTTON_MODE_LR         = 0x02,
    PKSAV_GBA_RSE_BUTTON_MODE_L_EQUALS_A = 0x03
};

enum pksav_gba_frlg_button_mode
{
    PKSAV_GBA_FRLG_BUTTON_MODE_HELP       = 0x01,
    PKSAV_GBA_FRLG_BUTTON_MODE_LR         = 0x02,
    PKSAV_GBA_FRLG_BUTTON_MODE_L_EQUALS_A = 0x03
};

// Text speed, textbox frame

enum pksav_gba_text_speed
{
    PKSAV_GBA_TEXT_SPEED_SLOW   = 0,
    PKSAV_GBA_TEXT_SPEED_MEDIUM = 1,
    PKSAV_GBA_TEXT_SPEED_FAST   = 2
};

#define PKSAV_GBA_OPTIONS_TEXT_SPEED_MASK ((uint8_t)7)

#define PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_OFFSET (3)

#define PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_MASK ((uint8_t)(31 << PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_OFFSET))

#define PKSAV_GBA_OPTIONS_TEXTBOX_MIN_FRAME ((uint8_t)0)
#define PKSAV_GBA_OPTIONS_TEXTBOX_MAX_FRAME ((uint8_t)15)

#define PKSAV_GBA_OPTIONS_TEXTBOX_FRAME(options_byte) \
    ((uint8_t)(((options_byte) & PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_MASK) >> PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_OFFSET))

// Sound, battle style, battle scene

#define PKSAV_GBA_OPTIONS_SOUND_STEREO_MASK         ((uint8_t)1)
#define PKSAV_GBA_OPTIONS_BATTLE_STYLE_SET_MASK     ((uint8_t)(1 << 1))
#define PKSAV_GBA_OPTIONS_BATTLE_SCENE_DISABLE_MASK ((uint8_t)(1 << 2))

#endif /* PKSAV_GBA_OPTIONS_H */
