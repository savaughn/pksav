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

#define PKSAV_GB_ATK_IV_MASK   ((uint16_t)0xF000)
#define PKSAV_GB_ATK_IV_OFFSET (12)

#define PKSAV_GB_ATK_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_GB_ATK_IV_MASK) >> PKSAV_GB_ATK_IV_OFFSET))

#define PKSAV_GB_DEF_IV_MASK   ((uint16_t)0x0F00)
#define PKSAV_GB_DEF_IV_OFFSET (8)

#define PKSAV_GB_DEF_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_GB_DEF_IV_MASK) >> PKSAV_GB_DEF_IV_OFFSET))

#define PKSAV_GB_SPD_IV_MASK   ((uint16_t)0x00F0)
#define PKSAV_GB_SPD_IV_OFFSET (4)

#define PKSAV_GB_SPD_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_GB_SPD_IV_MASK) >> PKSAV_GB_SPD_IV_OFFSET))

#define PKSAV_GB_SPCL_IV_MASK ((uint16_t)0x000F)

#define PKSAV_GB_SPCL_IV(raw) \
    ((uint8_t)((raw) & PKSAV_GB_SPCL_IV_MASK))

enum pksav_error pksav_get_gb_IVs(
    const uint16_t* p_raw_IV,
    uint8_t* IVs_out,
    size_t IV_buffer_size
)
{
    if(!p_raw_IV || !IVs_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(IV_buffer_size > PKSAV_GB_IV_ATTACK)
    {
        IVs_out[PKSAV_GB_IV_ATTACK] = PKSAV_GB_ATK_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_GB_IV_DEFENSE)
    {
        IVs_out[PKSAV_GB_IV_DEFENSE] = PKSAV_GB_DEF_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_GB_IV_SPEED)
    {
        IVs_out[PKSAV_GB_IV_SPEED] = PKSAV_GB_SPD_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_GB_IV_SPECIAL)
    {
        IVs_out[PKSAV_GB_IV_SPECIAL] = PKSAV_GB_SPCL_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_GB_IV_HP)
    {
        IVs_out[PKSAV_GB_IV_HP] = (IVs_out[PKSAV_GB_IV_SPECIAL] & 0x01)
                                | ((IVs_out[PKSAV_GB_IV_SPEED] & 0x01) << 1)
                                | ((IVs_out[PKSAV_GB_IV_DEFENSE] & 0x01) << 2)
                                | ((IVs_out[PKSAV_GB_IV_ATTACK] & 0x01) << 3);
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_set_gb_IV(
    enum pksav_gb_IV stat,
    uint8_t IV_value,
    uint16_t* p_raw_IV
)
{
    if(!p_raw_IV)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if((stat < PKSAV_GB_IV_ATTACK) || (stat > PKSAV_GB_IV_HP))
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }
    if(IV_value > PKSAV_MAX_GB_IV)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch(stat)
    {
        case PKSAV_GB_IV_ATTACK:
            (*p_raw_IV) &= ~PKSAV_GB_ATK_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_GB_ATK_IV_OFFSET);
            break;

        case PKSAV_GB_IV_DEFENSE:
            (*p_raw_IV) &= ~PKSAV_GB_DEF_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_GB_DEF_IV_OFFSET);
            break;

        case PKSAV_GB_IV_SPEED:
            (*p_raw_IV) &= ~PKSAV_GB_SPD_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_GB_SPD_IV_OFFSET);
            break;

        case PKSAV_GB_IV_SPECIAL:
            (*p_raw_IV) &= ~PKSAV_GB_SPCL_IV_MASK;
            (*p_raw_IV) |= IV_value;
            break;

        case PKSAV_GB_IV_HP:
            (*p_raw_IV) = ((*p_raw_IV) & 0xEEEE)
                          | ((IV_value & 0x08) << 9)
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

#define PKSAV_HP_IV_MASK ((uint32_t)0x1F)

#define PKSAV_HP_IV(raw) \
    ((uint8_t)((raw) & PKSAV_HP_IV_MASK))

#define PKSAV_ATK_IV_MASK   ((uint32_t)0x3E0)
#define PKSAV_ATK_IV_OFFSET (5)

#define PKSAV_ATK_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_ATK_IV_MASK) >> PKSAV_ATK_IV_OFFSET))

#define PKSAV_DEF_IV_MASK   ((uint32_t)0x7C00)
#define PKSAV_DEF_IV_OFFSET (10)

#define PKSAV_DEF_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_DEF_IV_MASK) >> PKSAV_DEF_IV_OFFSET))

#define PKSAV_SPD_IV_MASK   ((uint32_t)0xF8000)
#define PKSAV_SPD_IV_OFFSET (15)

#define PKSAV_SPD_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_SPD_IV_MASK) >> PKSAV_SPD_IV_OFFSET))

#define PKSAV_SPATK_IV_MASK   ((uint32_t)0x1F00000)
#define PKSAV_SPATK_IV_OFFSET (20)

#define PKSAV_SPATK_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_SPATK_IV_MASK) >> PKSAV_SPATK_IV_OFFSET))

#define PKSAV_SPDEF_IV_MASK   ((uint32_t)0x3E000000)
#define PKSAV_SPDEF_IV_OFFSET (25)

#define PKSAV_SPDEF_IV(raw) \
    ((uint8_t)(((raw) & PKSAV_SPDEF_IV_MASK) >> PKSAV_SPDEF_IV_OFFSET))

enum pksav_error pksav_get_IVs(
    const uint32_t* p_raw_IV,
    uint8_t* IVs_out,
    size_t IV_buffer_size
)
{
    if(!p_raw_IV || !IVs_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    if(IV_buffer_size > PKSAV_IV_ATTACK)
    {
        IVs_out[PKSAV_IV_ATTACK] = PKSAV_ATK_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_IV_DEFENSE)
    {
        IVs_out[PKSAV_IV_DEFENSE] = PKSAV_DEF_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_IV_SPEED)
    {
        IVs_out[PKSAV_IV_SPEED] = PKSAV_SPD_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_IV_SPATK)
    {
        IVs_out[PKSAV_IV_SPATK] = PKSAV_SPATK_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_IV_SPDEF)
    {
        IVs_out[PKSAV_IV_SPDEF] = PKSAV_SPDEF_IV((*p_raw_IV));
    }
    if(IV_buffer_size > PKSAV_IV_HP)
    {
        IVs_out[PKSAV_IV_HP] = PKSAV_HP_IV((*p_raw_IV));
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_set_IV(
    enum pksav_IV stat,
    uint8_t IV_value,
    uint32_t* p_raw_IV
)
{
    if(!p_raw_IV)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if((stat < PKSAV_IV_ATTACK) || (stat > PKSAV_IV_HP))
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }
    if(IV_value > PKSAV_MAX_IV)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch(stat)
    {
        case PKSAV_IV_ATTACK:
            (*p_raw_IV) &= ~PKSAV_ATK_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_ATK_IV_OFFSET);
            break;

        case PKSAV_IV_DEFENSE:
            (*p_raw_IV) &= ~PKSAV_DEF_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_DEF_IV_OFFSET);
            break;

        case PKSAV_IV_SPEED:
            (*p_raw_IV) &= ~PKSAV_SPD_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_SPD_IV_OFFSET);
            break;

        case PKSAV_IV_SPATK:
            (*p_raw_IV) &= ~PKSAV_SPATK_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_SPATK_IV_OFFSET);
            break;

        case PKSAV_IV_SPDEF:
            (*p_raw_IV) &= ~PKSAV_SPDEF_IV_MASK;
            (*p_raw_IV) |= (IV_value << PKSAV_SPDEF_IV_OFFSET);
            break;

        case PKSAV_IV_HP:
            (*p_raw_IV) &= ~PKSAV_HP_IV_MASK;
            (*p_raw_IV) |= IV_value;
            break;

        // This should have been caught by the input validation above.
        default:
            assert(0);
    }

    return PKSAV_ERROR_NONE;
}
