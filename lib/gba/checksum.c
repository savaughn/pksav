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
    const struct pksav_gba_pc_pokemon* gba_pokemon_ptr
)
{
    assert(gba_pokemon_ptr != NULL);

    union pksav_gba_pokemon_blocks_internal* gba_pokemon_internal_blocks_ptr =
        (union pksav_gba_pokemon_blocks_internal*)&gba_pokemon_ptr->blocks;

    uint16_t ret = 0;
    for(size_t index = 0;
        index < (sizeof(union pksav_gba_pokemon_blocks_internal)/2);
        ++index)
    {
        ret += gba_pokemon_internal_blocks_ptr->blocks16[index];
    }

    return ret;
}

uint16_t pksav_gba_get_section_checksum(
    const struct pksav_gba_save_section* section_ptr,
    size_t section_num
)
{
    assert(section_ptr != NULL);

    uint32_t checksum = 0;
    uint16_t* checksum_ptr = (uint16_t*)&checksum;

    for(size_t index = 0;
        index < (pksav_gba_section_sizes[section_num]/4);
        ++index)
    {
        checksum += section_ptr->data32[index];
    }

    return (checksum_ptr[0] + checksum_ptr[1]);
}

void pksav_gba_set_section_checksums(
    union pksav_gba_save_slot* sections_ptr
)
{
    assert(sections_ptr != NULL);

    for(size_t section_index = 0;
        section_index < 14;
        ++section_index)
    {
        sections_ptr->sections_arr[section_index].footer.checksum =
            pksav_gba_get_section_checksum(
                &sections_ptr->sections_arr[section_index],
                section_index
            );
    }
}
