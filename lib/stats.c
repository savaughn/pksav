/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/error.h>
#include <pksav/stats.h>

#include <math.h>

#define PKSAV_GB_ATK_IV_MASK  ((uint16_t)0xF000)
#define PKSAV_GB_DEF_IV_MASK  ((uint16_t)0x0F00)
#define PKSAV_GB_SPD_IV_MASK  ((uint16_t)0x00F0)
#define PKSAV_GB_SPCL_IV_MASK ((uint16_t)0x000F)

pksav_error_t pksav_get_gb_IV(
    uint16_t* raw,
    pksav_battle_stat_t stat,
    uint8_t* IV_out
) {
    switch(stat) {
        case PKSAV_STAT_HP: {
            uint8_t atk, def, spd, spcl;
            (void)pksav_get_gb_IV(raw, PKSAV_STAT_ATTACK,  &atk);
            (void)pksav_get_gb_IV(raw, PKSAV_STAT_DEFENSE, &def);
            (void)pksav_get_gb_IV(raw, PKSAV_STAT_SPEED,   &spd);
            (void)pksav_get_gb_IV(raw, PKSAV_STAT_SPECIAL, &spcl);
            *IV_out = ((atk & 0x01) << 3) | ((def & 0x01) << 2)
                    | ((spd & 0x01) << 1) | (spcl & 0x01);
            break;
        }

        case PKSAV_STAT_ATTACK:
            *IV_out = ((*raw) & PKSAV_GB_ATK_IV_MASK) >> 12;
            break;

        case PKSAV_STAT_DEFENSE:
            *IV_out = ((*raw) & PKSAV_GB_DEF_IV_MASK) >> 8;
            break;

        case PKSAV_STAT_SPEED:
            *IV_out = ((*raw) & PKSAV_GB_SPD_IV_MASK) >> 4;
            break;

        case PKSAV_STAT_SPATK:
        case PKSAV_STAT_SPDEF:
        case PKSAV_STAT_SPECIAL:
            *IV_out = (*raw) & PKSAV_GB_SPCL_IV_MASK;
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_set_gb_IV(
    uint16_t* raw,
    pksav_battle_stat_t stat,
    uint8_t new_IV
) {
    if(new_IV > 15) {
        return PKSAV_ERROR_INVALID_IV;
    }

    switch(stat) {
        case PKSAV_STAT_HP:
            *raw = (((*raw) & 0xEEEE) | ((new_IV & 0x08) << 9)
                                      | ((new_IV & 0x04) << 6)
                                      | ((new_IV & 0x02) << 3)
                                      |  (new_IV & 0x01));
            break;

        case PKSAV_STAT_ATTACK:
            *raw = ((*raw) & ~PKSAV_GB_ATK_IV_MASK) | (new_IV << 12);
            break;

        case PKSAV_STAT_DEFENSE:
            *raw = ((*raw) & ~PKSAV_GB_DEF_IV_MASK) | (new_IV << 8);
            break;

        case PKSAV_STAT_SPEED:
            *raw = ((*raw) & ~PKSAV_GB_SPD_IV_MASK) | (new_IV << 4);
            break;

        case PKSAV_STAT_SPATK:
        case PKSAV_STAT_SPDEF:
        case PKSAV_STAT_SPECIAL:
            *raw = ((*raw) & ~PKSAV_GB_SPCL_IV_MASK) | new_IV;
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}

#define PKSAV_HP_IV_MASK    ((uint32_t)0x1F)
#define PKSAV_ATK_IV_MASK   ((uint32_t)0x3E0)
#define PKSAV_DEF_IV_MASK   ((uint32_t)0x7C00)
#define PKSAV_SPD_IV_MASK   ((uint32_t)0xF8000)
#define PKSAV_SPATK_IV_MASK ((uint32_t)0x1F00000)
#define PKSAV_SPDEF_IV_MASK ((uint32_t)0x3E000000)

pksav_error_t pksav_get_IV(
    uint32_t* raw,
    pksav_battle_stat_t stat,
    uint8_t* IV_out
) {
    switch(stat) {
        case PKSAV_STAT_HP:
            *IV_out = (*raw) | PKSAV_HP_IV_MASK;
            break;

        case PKSAV_STAT_ATTACK:
            *IV_out = ((*raw) & PKSAV_ATK_IV_MASK) >> 5;
            break;

        case PKSAV_STAT_DEFENSE:
            *IV_out = ((*raw) & PKSAV_DEF_IV_MASK) >> 10;
            break;

        case PKSAV_STAT_SPEED:
            *IV_out = ((*raw) & PKSAV_SPD_IV_MASK) >> 15;
            break;

        case PKSAV_STAT_SPATK:
            *IV_out = ((*raw) & PKSAV_SPATK_IV_MASK) >> 20;
            break;

        case PKSAV_STAT_SPDEF:
            *IV_out = ((*raw) & PKSAV_SPDEF_IV_MASK) >> 25;
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_set_IV(
    uint32_t* raw,
    pksav_battle_stat_t stat,
    uint8_t new_IV
) {
    if(new_IV > 31) {
        return PKSAV_ERROR_INVALID_IV;
    }

    switch(stat) {
        case PKSAV_STAT_HP:
            *raw = ((*raw) & ~PKSAV_HP_IV_MASK) | new_IV;
            break;

        case PKSAV_STAT_ATTACK:
            *raw = ((*raw) & ~PKSAV_ATK_IV_MASK) | (new_IV << 5);
            break;

        case PKSAV_STAT_DEFENSE:
            *raw = ((*raw) & ~PKSAV_DEF_IV_MASK) | (new_IV << 10);
            break;

        case PKSAV_STAT_SPEED:
            *raw = ((*raw) & ~PKSAV_SPD_IV_MASK) | (new_IV << 15);
            break;

        case PKSAV_STAT_SPATK:
            *raw = ((*raw) & ~PKSAV_SPATK_IV_MASK) | (new_IV << 20);
            break;

        case PKSAV_STAT_SPDEF:
            *raw = ((*raw) & ~PKSAV_SPDEF_IV_MASK) | (new_IV << 25);
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}
