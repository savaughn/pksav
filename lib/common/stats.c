/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/error.h>

#include <pksav/common/stats.h>

#include <assert.h>
#include <math.h>

#define PKSAV_GB_ATK_IV_MASK  ((uint16_t)0xF000)
#define PKSAV_GB_ATK_IV_OFFSET 12

#define PKSAV_GB_ATK_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_GB_ATK_IV_MASK) >> PKSAV_GB_ATK_IV_OFFSET))

#define PKSAV_GB_DEF_IV_MASK  ((uint16_t)0x0F00)
#define PKSAV_GB_DEF_IV_OFFSET 8

#define PKSAV_GB_DEF_IV(raw) \
    ((uint8_t)((raw) & PKSAV_GB_DEF_IV_MASK) >> PKSAV_GB_DEF_IV_OFFSET)

#define PKSAV_GB_SPD_IV_MASK  ((uint16_t)0x00F0)
#define PKSAV_GB_SPD_IV_OFFSET 4

#define PKSAV_GB_SPD_IV(raw) \
    ((uint8_t)((raw) & PKSAV_GB_SPD_IV_MASK) >> PKSAV_GB_SPD_IV_OFFSET)

#define PKSAV_GB_SPCL_IV_MASK ((uint16_t)0x000F)

#define PKSAV_GB_SPCL_IV(raw) \
    ((uint8_t)((raw) & PKSAV_GB_SPCL_IV_MASK))

enum pksav_error pksav_get_gb_IVs(
    const uint16_t* raw_IV_ptr,
    uint8_t* IVs_out,
    size_t IV_buffer_size,
    size_t* actual_num_IVs_out
)
{
    if(!raw_IV_ptr || !IVs_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(IV_buffer_size > (size_t)PKSAV_GB_IV_ATTACK)
    {
        IVs_out[PKSAV_GB_IV_ATTACK] = PKSAV_GB_ATK_IV((*raw_IV_ptr));
    }
    if(IV_buffer_size > (size_t)PKSAV_GB_IV_DEFENSE)
    {
        IVs_out[PKSAV_GB_IV_DEFENSE] = PKSAV_GB_DEF_IV((*raw_IV_ptr));
    }
    if(IV_buffer_size > (size_t)PKSAV_GB_IV_SPEED)
    {
        IVs_out[PKSAV_GB_IV_SPEED] = PKSAV_GB_SPD_IV((*raw_IV_ptr));
    }
    if(IV_buffer_size > (size_t)PKSAV_GB_IV_SPECIAL)
    {
        IVs_out[PKSAV_GB_IV_SPECIAL] = PKSAV_GB_SPCL_IV((*raw_IV_ptr));
    }
    if(IV_buffer_size > (size_t)PKSAV_GB_IV_HP)
    {
        IVs_out[PKSAV_GB_IV_HP] = (IVs_out[PKSAV_GB_IV_SPECIAL] & 0x01)
                                | ((IVs_out[PKSAV_GB_IV_SPEED] & 0x01) << 1)
                                | ((IVs_out[PKSAV_GB_IV_DEFENSE] & 0x01) << 2)
                                | ((IVs_out[PKSAV_GB_IV_ATTACK] & 0x01) << 3);
    }

    if(actual_num_IVs_out)
    {
        *actual_num_IVs_out = (size_t)PKSAV_GB_IV_HP;
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_set_gb_IV(
    enum pksav_gb_IV stat,
    uint8_t IV_value,
    uint16_t* raw_IV_ptr
)
{
    if(!raw_IV_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if((stat < PKSAV_GB_IV_ATTACK) || (stat > PKSAV_GB_IV_HP))
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }
    if(IV_value > 15)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch(stat)
    {
        case PKSAV_GB_IV_ATTACK:
            (*raw_IV_ptr) &= ~PKSAV_GB_ATK_IV_MASK;
            (*raw_IV_ptr) |= (IV_value << PKSAV_GB_ATK_IV_OFFSET);
            break;

        case PKSAV_GB_IV_DEFENSE:
            (*raw_IV_ptr) &= ~PKSAV_GB_DEF_IV_MASK;
            (*raw_IV_ptr) |= (IV_value << PKSAV_GB_DEF_IV_OFFSET);
            break;

        case PKSAV_GB_IV_SPEED:
            (*raw_IV_ptr) &= ~PKSAV_GB_SPD_IV_MASK;
            (*raw_IV_ptr) |= (IV_value << PKSAV_GB_SPD_IV_OFFSET);
            break;

        case PKSAV_GB_IV_SPECIAL:
            (*raw_IV_ptr) &= ~PKSAV_GB_SPCL_IV_MASK;
            (*raw_IV_ptr) |= IV_value;
            break;

        case PKSAV_GB_IV_HP:
            (*raw_IV_ptr) &= 0xEEEE;
            (*raw_IV_ptr) = ((IV_value & 0x08) << 9)
                          | ((IV_value & 0x04) << 6)
                          | ((IV_value & 0x02) << 3)
                          |  (IV_value & 0x01);
            break;

        // This should have been caught by the input validation above.
        default:
            assert(0);
    }

    return PKSAV_ERROR_NONE;
}

/*
enum pksav_error pksav_set_gb_IV(
    uint16_t* raw,
    enum pksav_battle_stat stat,
    uint8_t new_IV
)
{
    if(!raw)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(new_IV > 15)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch(stat)
    {
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

enum pksav_error pksav_get_IV(
    const uint32_t* raw,
    enum pksav_battle_stat stat,
    uint8_t* IV_out
)
{
    if(!raw || !IV_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    switch(stat)
    {
        case PKSAV_STAT_HP:
            *IV_out = (*raw) & PKSAV_HP_IV_MASK;
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

enum pksav_error pksav_set_IV(
    uint32_t* raw,
    enum pksav_battle_stat stat,
    uint8_t new_IV
)
{
    if(!raw)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(new_IV > 31)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch(stat)
    {
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
*/
