/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"
#include "crypt.h"
#include "save_internal.h"
#include "shuffle.h"

#include <pksav/gba/save.h>

#include <assert.h>
#include <string.h>

void pksav_gba_save_unshuffle_sections(
    const union pksav_gba_save_slot* save_slot_in,
    union pksav_gba_save_slot* save_slot_out,
    uint8_t* section_nums_out
)
{
    assert(save_slot_in != NULL);
    assert(save_slot_out != NULL);
    assert(section_nums_out != NULL);

    for(size_t section_index = 0;
        section_index < PKSAV_GBA_NUM_SAVE_SECTIONS;
        ++section_index)
    {
        uint8_t section_id = save_slot_in->sections_arr[section_index].footer.section_id;
        save_slot_out->sections_arr[section_id] = save_slot_in->sections_arr[section_index];

        // Cache the original positions.
        section_nums_out[section_index] = section_id;
    }
}

void pksav_gba_save_shuffle_sections(
    const union pksav_gba_save_slot* save_slot_in,
    union pksav_gba_save_slot* save_slot_out,
    const uint8_t* section_nums_ptr
)
{
    assert(save_slot_in != NULL);
    assert(save_slot_out != NULL);
    assert(section_nums_ptr != NULL);

    for(size_t section_index = 0;
        section_index < PKSAV_GBA_NUM_SAVE_SECTIONS;
        ++section_index)
    {
        assert(section_nums_ptr[section_index] < PKSAV_GBA_NUM_SAVE_SECTIONS);

        save_slot_out->sections_arr[section_index] =
            save_slot_in->sections_arr[section_nums_ptr[section_index]];
    }
}

void pksav_gba_save_load_pokemon_pc(
    const union pksav_gba_save_slot* gba_save_slot,
    struct pksav_gba_pokemon_pc* pokemon_pc_out
)
{
    assert(gba_save_slot != NULL);
    assert(pokemon_pc_out != NULL);

    memset(
        pokemon_pc_out,
        0,
        sizeof(*pokemon_pc_out)
    );

    // Copy data from sections into contiguous data structure.
    uint8_t* dst_ptr = (uint8_t*)pokemon_pc_out;
    for(size_t section_index = 5;
        section_index <= 13;
        ++section_index)
    {
        memcpy(
            dst_ptr,
            gba_save_slot->sections_arr[section_index].data8,
            pksav_gba_section_sizes[section_index]
        );

        dst_ptr += pksav_gba_section_sizes[section_index];
    }

    // Decrypt Pokémon.
    for(size_t box_index = 0;
        box_index < PKSAV_GBA_NUM_POKEMON_BOXES;
        ++box_index)
    {
        for(size_t pokemon_index = 0;
            pokemon_index < PKSAV_GBA_BOX_NUM_POKEMON;
            ++pokemon_index)
        {
            pksav_gba_crypt_pokemon(
                &pokemon_pc_out->boxes[box_index].entries[pokemon_index],
                false
            );
        }
    }
}

void pksav_gba_save_save_pokemon_pc(
    struct pksav_gba_pokemon_pc* pokemon_pc_ptr,
    union pksav_gba_save_slot* gba_save_slot_out
)
{
    assert(pokemon_pc_ptr != NULL);
    assert(gba_save_slot_out != NULL);

    // Set Pokémon checksum and encrypt.
    for(size_t box_index = 0;
        box_index < PKSAV_GBA_NUM_POKEMON_BOXES;
        ++box_index)
    {
        for(uint8_t pokemon_index = 0;
            pokemon_index < PKSAV_GBA_BOX_NUM_POKEMON;
            ++pokemon_index)
        {
            pksav_gba_set_pokemon_checksum(
                &pokemon_pc_ptr->boxes[box_index].entries[pokemon_index]
            );
            pksav_gba_crypt_pokemon(
                &pokemon_pc_ptr->boxes[box_index].entries[pokemon_index],
                true
            );
        }
    }

    // Copy contiguous data structure back into sections.
    uint8_t* src_ptr = (uint8_t*)pokemon_pc_ptr;
    for(size_t section_index = 5;
        section_index <= 13;
        ++section_index)
    {
        memcpy(
            gba_save_slot_out->sections_arr[section_index].data8,
            src_ptr,
            pksav_gba_section_sizes[section_index]
        );
        src_ptr += pksav_gba_section_sizes[section_index];
    }
}
