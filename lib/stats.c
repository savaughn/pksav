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

// TODO: as fast as possible, no floats?
pksav_error_t pksav_get_gb_stat(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    uint16_t EV,
    uint8_t IV,
    uint16_t* stat_out
) {
    // Common to both calculations
    uint16_t tmp1 = (base_stat + IV) << 1;
    uint16_t tmp2 = ceil((uint16_t)sqrt(EV) >> 2) * level;
    uint16_t tmp3 = (tmp1 + tmp2) / 100;

    switch(stat) {
        case PKSAV_STAT_HP:
            *stat_out = tmp3 + level + 10;
            break;

        case PKSAV_STAT_ATTACK:
        case PKSAV_STAT_DEFENSE:
        case PKSAV_STAT_SPEED:
        case PKSAV_STAT_SPATK:
        case PKSAV_STAT_SPDEF:
        case PKSAV_STAT_SPECIAL:
            *stat_out = tmp3 + 5;
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_get_gb_stat_range(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    pksav_stat_pair_t* stat_pair_out
) {
    pksav_stat_pair_t tmp;
    pksav_error_t return_code = pksav_get_gb_stat(
                                    stat, level, base_stat,
                                    0, 0, &tmp.first
                                );
    if(return_code) {
        return return_code;
    }

    return_code = pksav_get_gb_stat(
                      stat, level, base_stat,
                      65535, 15, &tmp.second
                  );
    if(return_code) {
        return return_code;
    }

    *stat_pair_out = tmp;
    return PKSAV_ERROR_NONE;
}

#define PKSAV_FLOATS_CLOSE(a,b) (fabs(a-b) < 0.00001f)

// TODO: as fast as possible
pksav_error_t pksav_get_stat(
    pksav_battle_stat_t stat,
    uint8_t level,
    float nature_modifier,
    uint8_t base_stat,
    uint16_t EV,
    uint8_t IV,
    uint16_t* stat_out
) {
    if(EV > 255) {
        return PKSAV_ERROR_INVALID_EV;
    }
    if(IV > 31) {
        return PKSAV_ERROR_INVALID_IV;
    }
    if(!PKSAV_FLOATS_CLOSE(nature_modifier,0.9f) && !PKSAV_FLOATS_CLOSE(nature_modifier,1.1f)) {
        return PKSAV_ERROR_INVALID_PARAM;
    }

    // Common to both calculations
    uint16_t tmp1 = (base_stat + IV) << 1;
    uint16_t tmp2 = (EV >> 2) * level;
    uint16_t tmp3 = (tmp1 + tmp2) / 100;

    switch(stat) {
        case PKSAV_STAT_HP:
            *stat_out = tmp3 + level + 10;
            break;

        case PKSAV_STAT_ATTACK:
        case PKSAV_STAT_DEFENSE:
        case PKSAV_STAT_SPEED:
        case PKSAV_STAT_SPATK:
        case PKSAV_STAT_SPDEF:
            *stat_out = (uint16_t)((float)(tmp3 + 5) * nature_modifier);
            break;

        default:
            return PKSAV_ERROR_INVALID_STAT;
    }

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_get_stat_range(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    pksav_stat_pair_t* stat_pair_out
) {
    pksav_stat_pair_t tmp;
    pksav_error_t return_code = pksav_get_stat(
                                    stat, level, 0.9f, base_stat,
                                    0, 0, &tmp.first
                                );
    if(return_code) {
        return return_code;
    }

    return_code = pksav_get_stat(
                      stat, level, 1.1f, base_stat,
                      255, 31, &tmp.second
                  );
    if(return_code) {
        return return_code;
    }

    *stat_pair_out = tmp;
    return PKSAV_ERROR_NONE;
}
