/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/pokedex.h>

#include <pksav/gba/pokedex.h>

enum pksav_error pksav_gba_pokedex_set_has_seen(
    struct pksav_gba_pokedex* p_gba_pokedex,
    uint16_t pokedex_num,
    bool has_seen
)
{
    if(!p_gba_pokedex)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(pokedex_num > 386)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    error = pksav_set_pokedex_bit(
                p_gba_pokedex->p_seenA,
                pokedex_num,
                has_seen
            );
    if(!error)
    {
        error = pksav_set_pokedex_bit(
                    p_gba_pokedex->p_seenB,
                    pokedex_num,
                    has_seen
                );
        if(!error)
        {
            error = pksav_set_pokedex_bit(
                        p_gba_pokedex->p_seenC,
                        pokedex_num,
                        has_seen
                    );
        }
    }

    return error;
}

enum pksav_error pksav_gba_pokedex_set_national_pokedex_unlocked(
    struct pksav_gba_pokedex* p_gba_pokedex,
    enum pksav_gba_save_type save_type,
    bool is_unlocked
)
{
    if(!p_gba_pokedex)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if((save_type < PKSAV_GBA_SAVE_TYPE_NONE) || (save_type > PKSAV_GBA_SAVE_TYPE_FRLG))
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    if(is_unlocked)
    {
        *p_gba_pokedex->p_nat_pokedex_unlockedB |=
            PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B_FLAG;

        if(save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            *p_gba_pokedex->p_frlg_nat_pokedex_unlockedA |=
                PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_A_FLAG;
            *p_gba_pokedex->p_nat_pokedex_unlockedC |=
                PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_C_FLAG;
        }
        else
        {
            *p_gba_pokedex->p_rse_nat_pokedex_unlockedA |=
                PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_A_FLAG;
            *p_gba_pokedex->p_nat_pokedex_unlockedC |=
                PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_C_FLAG;
        }
    }
    else
    {
        *p_gba_pokedex->p_nat_pokedex_unlockedB &=
            ~PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B_FLAG;

        if(save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            *p_gba_pokedex->p_frlg_nat_pokedex_unlockedA &=
                ~PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_A_FLAG;
            *p_gba_pokedex->p_nat_pokedex_unlockedC &=
                ~PKSAV_GBA_FRLG_NAT_POKEDEX_UNLOCKED_C_FLAG;
        }
        else
        {
            *p_gba_pokedex->p_rse_nat_pokedex_unlockedA &=
                ~PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_A_FLAG;
            *p_gba_pokedex->p_nat_pokedex_unlockedC &=
                ~PKSAV_GBA_RSE_NAT_POKEDEX_UNLOCKED_C_FLAG;
        }
    }

    return PKSAV_ERROR_NONE;
}
