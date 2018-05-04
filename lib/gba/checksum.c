/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"

#include "save_internal.h"

#include <assert.h>

uint16_t pksav_gba_get_pokemon_checksum(
    const struct pksav_gba_pc_pokemon* p_gba_pokemon
)
{
    assert(p_gba_pokemon != NULL);

    union pksav_gba_pokemon_blocks_internal* p_gba_pokemon_internal_blocks =
        (union pksav_gba_pokemon_blocks_internal*)&p_gba_pokemon->blocks;

    uint16_t ret = 0;
    for(size_t index = 0;
        index < (sizeof(union pksav_gba_pokemon_blocks_internal)/2);
        ++index)
    {
        ret += p_gba_pokemon_internal_blocks->blocks16[index];
    }

    return ret;
}

uint16_t pksav_gba_get_section_checksum(
    const struct pksav_gba_save_section* p_section,
    size_t section_num
)
{
    assert(p_section != NULL);
    assert(section_num < PKSAV_GBA_NUM_SAVE_SECTIONS);

    uint32_t checksum = 0;
    uint16_t* p_checksum = (uint16_t*)&checksum;

    for(size_t index = 0;
        index < (pksav_gba_section_sizes[section_num]/4);
        ++index)
    {
        checksum += p_section->data32[index];
    }

    return (p_checksum[0] + p_checksum[1]);
}

void pksav_gba_set_section_checksums(
    union pksav_gba_save_slot* p_sections
)
{
    assert(p_sections != NULL);

    for(size_t section_index = 0;
        section_index < PKSAV_GBA_NUM_SAVE_SECTIONS;
        ++section_index)
    {
        p_sections->sections_arr[section_index].footer.checksum =
            pksav_gba_get_section_checksum(
                &p_sections->sections_arr[section_index],
                section_index
            );
    }
}
