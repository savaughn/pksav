/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_OPTIONS_H
#define PKSAV_GEN2_OPTIONS_H

// Byte 1

enum pksav_gen2_text_speed
{
    PKSAV_GEN2_TEXT_SPEED_FAST = 1,
    PKSAV_GEN2_TEXT_SPEED_NORMAL = 3,
    PKSAV_GEN2_TEXT_SPEED_SLOW = 5
};

#define PKSAV_GEN2_OPTIONS_TEXT_SPEED_MASK       ((uint8_t)7)
#define PKSAV_GEN2_OPTIONS_NO_TEXT_DELAY_MASK    ((uint8_t)(1 << 4))
#define PKSAV_GEN2_OPTIONS_SOUND_STEREO_MASK     ((uint8_t)(1 << 4))
#define PKSAV_GEN2_OPTIONS_BATTLE_STYLE_SET_MASK ((uint8_t)(1 << 6))
#define PKSAV_GEN2_OPTIONS_BATTLE_SCENE_MASK     ((uint8_t)(1 << 7))

// Textbox frame

#define PKSAV_GEN2_OPTIONS_TEXTBOX_FRAME_MASK ((uint8_t)7)

#define PKSAV_GEN2_OPTIONS_TEXTBOX_MIN_FRAME ((uint8_t)0)
#define PKSAV_GEN2_OPTIONS_TEXTBOX_MAX_FRAME ((uint8_t)7)

// Game Boy Printer

enum pksav_gen2_gbprinter_brightness
{
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS_LIGHTEST = 0x00,
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS_LIGHTER  = 0x20,
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS_NORMAL   = 0x40,
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS_DARKER   = 0x60,
    PKSAV_GEN2_GBPRINTER_BRIGHTNESS_DARKEST  = 0x7F,
};

#define PKSAV_GEN2_GBPRINTER_BRIGHTNESS_MASK ((uint8_t)0x7F)

// Menu account

#define PKSAV_GEN2_OPTIONS_MENU_ACCOUNT_MASK ((uint8_t)2)

#endif /* PKSAV_GEN2_OPTIONS_H */
