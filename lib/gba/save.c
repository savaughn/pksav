/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "checksum.h"
#include "crypt.h"
#include "save_internal.h"
#include "shuffle.h"

#include "util/fs.h"

#include <pksav/config.h>

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <pksav/math/endian.h>

#include <string.h>

union pksav_gba_save_slot* pksav_gba_get_active_save_slot_ptr(
    uint8_t* p_buffer,
    size_t buffer_len
)
{
    assert(p_buffer != NULL);

    union pksav_gba_save_slot* p_active_save_slot = NULL;

    if(buffer_len >= PKSAV_GBA_SAVE_SLOT_SIZE*2)
    {
        union pksav_gba_save_slot* save_slots = (union pksav_gba_save_slot*)p_buffer;
        uint32_t save_index1 = pksav_littleendian32(
                                   save_slots[0].section0.footer.save_index
                               );
        uint32_t save_index2 = pksav_littleendian32(
                                   save_slots[1].section0.footer.save_index
                               );

        if(save_index1 > save_index2)
        {
            p_active_save_slot = &save_slots[0];
        }
        else
        {
            p_active_save_slot = &save_slots[1];
        }
    }
    else if(buffer_len >= PKSAV_GBA_SAVE_SLOT_SIZE)
    {
        p_active_save_slot = (union pksav_gba_save_slot*)p_buffer;
    }

    return p_active_save_slot;
}

enum pksav_error pksav_gba_get_buffer_save_type(
    const uint8_t* buffer,
    size_t buffer_len,
    enum pksav_gba_save_type* p_save_type_out
)
{
    if(!buffer || !p_save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *p_save_type_out = PKSAV_GBA_SAVE_TYPE_NONE;
    const union pksav_gba_save_slot* p_save_slot = pksav_gba_get_active_save_slot_ptr(
                                                       (uint8_t*)buffer,
                                                       buffer_len
                                                   );
    if(p_save_slot != NULL)
    {
        // At this point, we know the buffer is large enough to be a save, so
        // now to validate the sections.

        // Make sure the section IDs are valid before using them as array
        // indices to avoid a crash.
        bool is_save_valid = true;

        for(size_t section_index = 0;
            (section_index < PKSAV_GBA_NUM_SAVE_SECTIONS) && is_save_valid;
            ++section_index)
        {
            const struct pksav_gba_section_footer* p_section_footer =
                &p_save_slot->sections_arr[section_index].footer;

            if((p_section_footer->section_id > (PKSAV_GBA_NUM_SAVE_SECTIONS-1)) ||
               (pksav_littleendian32(p_section_footer->validation) != PKSAV_GBA_VALIDATION_MAGIC))
            {
                is_save_valid = false;
            }
        }

        if(is_save_valid)
        {
            union pksav_gba_save_slot unshuffled_save_slots;
            uint8_t section_nums[14] = {0}; // Unused
            pksav_gba_save_unshuffle_sections(
                p_save_slot,
                &unshuffled_save_slots,
                section_nums
            );

            const uint32_t rs_game_code = 0;
            const uint32_t frlg_game_code = 1;

            /*
             * For Ruby/Sapphire and FireRed/LeafGreen, check for validation by
             * checking for a known game code and comparing the two security
             * keys. For Emerald, there is no game code, so just compare the
             * security keys. To avoid false positives, search in a specific
             * order.
             */
            static const enum pksav_gba_save_type save_types_to_search[3] =
            {
                PKSAV_GBA_SAVE_TYPE_RS,
                PKSAV_GBA_SAVE_TYPE_FRLG,
                PKSAV_GBA_SAVE_TYPE_EMERALD
            };
            static const size_t num_save_types =
                sizeof(save_types_to_search)/sizeof(save_types_to_search[0]);

            is_save_valid = false;
            for(size_t save_type_index = 0;
                (save_type_index < num_save_types) && !is_save_valid;
                ++save_type_index)
            {
                enum pksav_gba_save_type save_type = save_types_to_search[save_type_index];

                const size_t security_key1_offset =
                    PKSAV_GBA_SAVE_SECTION0_OFFSETS[save_type-1][PKSAV_GBA_SECURITY_KEY1];
                const size_t security_key2_offset =
                    PKSAV_GBA_SAVE_SECTION0_OFFSETS[save_type-1][PKSAV_GBA_SECURITY_KEY2];

                // Ignore endianness for the security keys since we're not actually
                // using the value, just checking equality.
                const uint32_t security_key1 = unshuffled_save_slots.section0.data32[
                                                   security_key1_offset/4
                                               ];
                const uint32_t security_key2 = unshuffled_save_slots.section0.data32[
                                                   security_key2_offset/4
                                               ];

                is_save_valid = (security_key1 == security_key2);
                if(save_type != PKSAV_GBA_SAVE_TYPE_EMERALD)
                {
                    const size_t game_code_offset =
                        PKSAV_GBA_SAVE_SECTION0_OFFSETS[save_type-1][PKSAV_GBA_GAME_CODE];

                    const uint32_t game_code = pksav_littleendian32(
                                                   unshuffled_save_slots.section0.data32[
                                                       game_code_offset/4
                                                   ]
                                               );

                    const uint32_t expected_game_code = (save_type == PKSAV_GBA_SAVE_TYPE_RS)
                        ? rs_game_code : frlg_game_code;

                    is_save_valid &= (game_code == expected_game_code);
                }

                if(is_save_valid)
                {
                    *p_save_type_out = save_type;
                }
            }
        }
    }

    return error;
}

enum pksav_error pksav_gba_get_file_save_type(
    const char* p_filepath,
    enum pksav_gba_save_type* p_save_type_out
)
{
    if(!p_filepath || !p_save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* p_file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                p_filepath,
                &p_file_buffer,
                &buffer_len
            );
    if(!error)
    {
        assert(p_file_buffer != NULL);

        enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
        error = pksav_gba_get_buffer_save_type(
                    p_file_buffer,
                    buffer_len,
                    &save_type
                );
        free(p_file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *p_save_type_out = save_type;
        }
    }

    return error;
}

static void _pksav_gba_set_save_pointers(
    struct pksav_gba_save* p_gba_save,
    uint8_t* buffer,
    size_t buffer_len,
    bool should_alloc_internal
)
{
    assert(p_gba_save != NULL);
    assert(p_gba_save->save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    assert(p_gba_save->save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);
    assert(buffer != NULL);

    // Offsets
    const size_t* section0_offsets_ptr = PKSAV_GBA_SAVE_SECTION0_OFFSETS[p_gba_save->save_type-1];
    const size_t* section1_offsets_ptr = PKSAV_GBA_SAVE_SECTION1_OFFSETS[p_gba_save->save_type-1];
    const size_t* section2_offsets_ptr = PKSAV_GBA_SAVE_SECTION2_OFFSETS[p_gba_save->save_type-1];
    const size_t* section4_offsets_ptr = PKSAV_GBA_SAVE_SECTION4_OFFSETS[p_gba_save->save_type-1];

    // At this point, we should know the save is valid, so we should be able
    // to get valid sections.
    union pksav_gba_save_slot* p_save_slot = pksav_gba_get_active_save_slot_ptr(
                                                    buffer,
                                                    buffer_len
                                               );
    assert(p_save_slot != NULL);

    // Internal
    if(should_alloc_internal)
    {
        p_gba_save->p_internal = calloc(sizeof(struct pksav_gba_save_internal), 1);
    }

    struct pksav_gba_save_internal* p_internal = p_gba_save->p_internal;
    p_internal->p_raw_save = buffer;
    p_internal->save_len = buffer_len;
    p_internal->is_save_from_first_slot = ((uint8_t*)p_save_slot == buffer);

    pksav_gba_save_unshuffle_sections(
        p_save_slot,
        &p_internal->unshuffled_save_slot,
        p_internal->shuffled_section_nums
    );

    // Pointers to relevant sections
    struct pksav_gba_save_section* p_section0 = &p_internal->unshuffled_save_slot.section0;
    struct pksav_gba_player_info_internal* p_player_info_internal =
        (struct pksav_gba_player_info_internal*)p_section0;

    struct pksav_gba_save_section* p_section1 = &p_internal->unshuffled_save_slot.section1;
    struct pksav_gba_save_section* p_section2 = &p_internal->unshuffled_save_slot.section2;
    struct pksav_gba_save_section* p_section4 = &p_internal->unshuffled_save_slot.section4;

    // Security key, used in decryption
    p_internal->p_security_key = &p_section0->data32[
                                     section0_offsets_ptr[PKSAV_GBA_SECURITY_KEY1]/4
                                 ];

    // Time played
    p_gba_save->p_time_played = &p_player_info_internal->time_played;

    // Options
    struct pksav_gba_options* p_options = &p_gba_save->options;

    p_options->p_button_mode = &p_section0->data8[
                                   section0_offsets_ptr[PKSAV_GBA_OPTIONS_BUTTON_MODE]
                               ];
    p_options->p_text_options = &p_section0->data8[
                                    section0_offsets_ptr[PKSAV_GBA_OPTIONS_TEXT]
                                ];
    p_options->p_sound_battle_options = &p_section0->data8[
                                            section0_offsets_ptr[PKSAV_GBA_OPTIONS_SOUND_BATTLE]
                                        ];

    // Pokémon storage
    struct pksav_gba_pokemon_storage* p_pokemon_storage = &p_gba_save->pokemon_storage;

    p_pokemon_storage->p_party = (struct pksav_gba_pokemon_party*)(
                                     &p_section1->data8[section1_offsets_ptr[PKSAV_GBA_POKEMON_PARTY]]
                                 );
    for(size_t party_index = 0;
        party_index < PKSAV_GBA_PARTY_NUM_POKEMON;
        ++party_index)
    {
        pksav_gba_crypt_pokemon(
            &p_pokemon_storage->p_party->party[party_index].pc_data,
            false // should_encrypt
        );
    }

    pksav_gba_save_load_pokemon_pc(
        &p_internal->unshuffled_save_slot,
        &p_internal->consolidated_pokemon_pc
    );
    p_pokemon_storage->p_pc = &p_internal->consolidated_pokemon_pc;

    // TODO: confirm crypting happens in daycare
    p_pokemon_storage->p_daycare = (union pksav_gba_daycare*)(
                                       &p_section4->data8[section4_offsets_ptr[PKSAV_GBA_DAYCARE]]
                                   );
    for(size_t daycare_index = 0;
        daycare_index < PKSAV_GBA_DAYCARE_NUM_POKEMON;
        ++daycare_index)
    {
        if(p_gba_save->save_type == PKSAV_GBA_SAVE_TYPE_RS)
        {
            pksav_gba_crypt_pokemon(
                &p_pokemon_storage->p_daycare->rs.pokemon[daycare_index],
                false // should_encrypt
            );
        }
        else
        {
            pksav_gba_crypt_pokemon(
                &p_pokemon_storage->p_daycare->emerald_frlg.pokemon[daycare_index].pokemon,
                false // should_encrypt
            );
        }
    }

    // Item storage
    struct pksav_gba_item_storage* p_item_storage = &p_gba_save->item_storage;

    p_item_storage->p_bag = (union pksav_gba_item_bag*)(
                                &p_section1->data8[section1_offsets_ptr[PKSAV_GBA_ITEM_BAG]]
                            );
    pksav_gba_save_crypt_items(
        p_item_storage->p_bag,
        *p_internal->p_security_key,
        p_gba_save->save_type
    );

    p_item_storage->p_pc = (struct pksav_gba_item_pc*)(
                               &p_section1->data8[section1_offsets_ptr[PKSAV_GBA_ITEM_PC]]
                           );

    p_item_storage->p_registered_item = &p_section1->data16[
                                            section1_offsets_ptr[PKSAV_GBA_REGISTERED_ITEM]/2
                                        ];

    // Pokédex
    struct pksav_gba_pokedex* p_pokedex = &p_gba_save->pokedex;

    p_pokedex->p_seenA = &p_section0->data8[
                             section0_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_A]
                         ];
    p_pokedex->p_seenB = &p_section1->data8[
                             section1_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_B]
                         ];
    p_pokedex->p_seenC = &p_section4->data8[
                             section4_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_C]
                         ];
    p_pokedex->p_owned = &p_section0->data8[
                             section0_offsets_ptr[PKSAV_GBA_POKEDEX_OWNED]
                         ];

    if(p_gba_save->save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
    {
        p_pokedex->p_frlg_nat_pokedex_unlockedA =
            &p_section0->data8[section0_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A]];

        p_pokedex->p_rse_nat_pokedex_unlockedA = NULL;
    }
    else
    {
        p_pokedex->p_rse_nat_pokedex_unlockedA =
            &p_section0->data16[section0_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A]/2];

        p_pokedex->p_frlg_nat_pokedex_unlockedA = NULL;
    }

    p_pokedex->p_nat_pokedex_unlockedB =
        &p_section2->data8[section2_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B]];

    p_pokedex->p_nat_pokedex_unlockedC =
        &p_section2->data16[section2_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_C]/2];

    // Trainer Info
    struct pksav_gba_player_info* p_player_info = &p_gba_save->player_info;

    p_player_info->p_id     = &p_player_info_internal->id;
    p_player_info->p_name   = p_player_info_internal->name;
    p_player_info->p_gender = &p_player_info_internal->gender;

    p_player_info->p_money = &p_section1->data32[
                                  section1_offsets_ptr[PKSAV_GBA_MONEY]/4
                              ];
    *p_player_info->p_money ^= *p_internal->p_security_key;

    // Misc Fields
    struct pksav_gba_misc_fields* p_misc_fields = &p_gba_save->misc_fields;

    p_misc_fields->p_casino_coins = &p_section1->data16[
                                        section1_offsets_ptr[PKSAV_GBA_CASINO_COINS]/2
                                    ];
    *p_misc_fields->p_casino_coins ^= (*p_internal->p_security_key & 0xFFFF);

    // TODO: roamer, once offset determined

    // Ruby/Sapphire-specific fields
    //struct pksav_gba_rs_fields* p_rs_fields = &p_misc_fields->rs_fields;

    // Emerald-specific fields
    //struct pksav_gba_emerald_fields* p_emerald_fields = &p_misc_fields->emerald_fields;

    // FireRed/LeafGreen-specific fields
    struct pksav_gba_frlg_fields* p_frlg_fields = &p_misc_fields->frlg_fields;
    if(p_gba_save->save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
    {
        p_frlg_fields->p_rival_name = &p_section4->data8[
                                          section4_offsets_ptr[PKSAV_GBA_FRLG_RIVAL_NAME]
                                      ];
    }
    else
    {
        p_frlg_fields->p_rival_name = NULL;
    }
}

static enum pksav_error _pksav_gba_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    bool is_buffer_ours,
    struct pksav_gba_save* p_gba_save_out
)
{
    assert(p_buffer != NULL);
    assert(p_gba_save_out != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_buffer_save_type(
                p_buffer,
                buffer_len,
                &save_type
            );
    if(!error)
    {
        if(save_type != PKSAV_GBA_SAVE_TYPE_NONE)
        {
            p_gba_save_out->save_type = save_type;
            _pksav_gba_set_save_pointers(
                p_gba_save_out,
                p_buffer,
                buffer_len,
                true // should_alloc_internal
            );

            // Internal
            struct pksav_gba_save_internal* p_internal = p_gba_save_out->p_internal;
            p_internal->is_buffer_ours = is_buffer_ours;
        }
        else
        {
            error = PKSAV_ERROR_INVALID_SAVE;
        }
    }

    return error;
}

enum pksav_error pksav_gba_load_save_from_buffer(
    uint8_t* p_buffer,
    size_t buffer_len,
    struct pksav_gba_save* p_gba_save_out
)
{
    if(!p_buffer || !p_gba_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    return _pksav_gba_load_save_from_buffer(
               p_buffer,
               buffer_len,
               false, // is_buffer_ours
               p_gba_save_out
           );
}

enum pksav_error pksav_gba_load_save_from_file(
    const char* p_filepath,
    struct pksav_gba_save* p_gba_save_out
)
{
    if(!p_filepath || !p_gba_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* p_file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                p_filepath,
                &p_file_buffer,
                &buffer_len
            );

    if(!error)
    {
        error = _pksav_gba_load_save_from_buffer(
                    p_file_buffer,
                    buffer_len,
                    true, // is_buffer_ours
                    p_gba_save_out
                );
        if(error)
        {
            // We made this buffer, so it's on us to free it if there's
            // an error.
            free(p_file_buffer);
        }
    }

    return error;
}

enum pksav_error pksav_gba_save_save(
    const char* p_filepath,
    struct pksav_gba_save* p_gba_save
)
{
    if(!p_filepath || !p_gba_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gba_save_internal* p_internal = p_gba_save->p_internal;

    // Item Storage
    pksav_gba_save_crypt_items(
        p_gba_save->item_storage.p_bag,
        *p_internal->p_security_key,
        p_gba_save->save_type
    );

    // Pokémon Storage
    for(size_t party_index = 0;
        party_index < PKSAV_GBA_PARTY_NUM_POKEMON;
        ++party_index)
    {
        pksav_gba_crypt_pokemon(
            &p_gba_save->pokemon_storage.p_party->party[party_index].pc_data,
            true // should_encrypt
        );
    }

    pksav_gba_save_save_pokemon_pc(
        &p_internal->consolidated_pokemon_pc,
        &p_internal->unshuffled_save_slot
    );

    // TODO: confirm crypting happens in daycare
    for(size_t daycare_index = 0;
        daycare_index < PKSAV_GBA_DAYCARE_NUM_POKEMON;
        ++daycare_index)
    {
        if(p_gba_save->save_type == PKSAV_GBA_SAVE_TYPE_RS)
        {
            pksav_gba_crypt_pokemon(
                &p_gba_save->pokemon_storage.p_daycare->rs.pokemon[daycare_index],
                true // should_encrypt
            );
        }
        else
        {
            pksav_gba_crypt_pokemon(
                &p_gba_save->pokemon_storage.p_daycare->emerald_frlg.pokemon[daycare_index].pokemon,
                true // should_encrypt
            );
        }
    }

    // Trainer Info
    *p_gba_save->player_info.p_money ^= *p_internal->p_security_key;

    // Misc Fields
    *p_gba_save->misc_fields.p_casino_coins ^= (*p_internal->p_security_key & 0xFFFF);

    // Save into the less recent save slot if the save file is large enough
    // for two slots.
    union pksav_gba_save_slot* p_raw_sections = (union pksav_gba_save_slot*)(
                                                      p_internal->p_raw_save
                                                  );
    union pksav_gba_save_slot* p_output_save_slot = NULL;
    if(p_internal->save_len >= PKSAV_GBA_SAVE_SLOT_SIZE*2)
    {
        p_output_save_slot = p_internal->is_save_from_first_slot ? &p_raw_sections[1]
                                                                 : &p_raw_sections[0];
    }
    else
    {
        p_output_save_slot = &p_raw_sections[0];
    }


    pksav_gba_set_section_checksums(&p_internal->unshuffled_save_slot);
    pksav_gba_save_shuffle_sections(
        &p_internal->unshuffled_save_slot,
        p_output_save_slot,
        p_internal->shuffled_section_nums
    );

    // Increment the save index of the new save index.
    uint32_t save_index = pksav_littleendian32(
                              p_output_save_slot->section0.footer.save_index
                          );
    ++save_index;

    for(size_t section_index = 0;
        section_index < PKSAV_GBA_NUM_SAVE_SECTIONS;
        ++section_index)
    {
        p_output_save_slot->sections_arr[section_index].footer.save_index =
            pksav_littleendian32(save_index);
    }

    error = pksav_fs_write_buffer_to_file(
                p_filepath,
                p_internal->p_raw_save,
                p_internal->save_len
            );

    if(!error)
    {
        // With everything saved to the new slot, reset the pointers.
        _pksav_gba_set_save_pointers(
            p_gba_save,
            p_internal->p_raw_save,
            p_internal->save_len,
            false // should_alloc_internal
        );
    }

    return error;
}

enum pksav_error pksav_gba_free_save(
    struct pksav_gba_save* p_gba_save
)
{
    if(!p_gba_save)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct pksav_gba_save_internal* p_internal = p_gba_save->p_internal;
    if(p_internal->is_buffer_ours)
    {
        free(p_internal->p_raw_save);
    }
    free(p_internal);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(p_gba_save, 0, sizeof(*p_gba_save));

    return PKSAV_ERROR_NONE;
}
