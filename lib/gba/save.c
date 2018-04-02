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

struct pksav_gba_save_internal
{
    uint8_t* raw_save_ptr;

    // There are multiple storage formats, so we can't just use our
    // "minimum size" #define.
    size_t save_len;

    bool is_save_from_first_slot;
    union pksav_gba_save_slot unshuffled_save_slot;
    uint8_t shuffled_section_nums[PKSAV_GBA_NUM_SAVE_SECTIONS];

    struct pksav_gba_pokemon_pc consolidated_pokemon_pc;
    uint32_t* security_key_ptr;

    bool is_buffer_ours;
};

// Each footer has a field that must equal this value to be considered valid.
#define PKSAV_GBA_VALIDATION_MAGIC 0x08012025

/*
 * Offsets
 */

enum pksav_gba_section0_field
{
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A = 0,
    PKSAV_GBA_POKEDEX_OWNED,
    PKSAV_GBA_POKEDEX_SEEN_A,
    PKSAV_GBA_GAME_CODE,
    PKSAV_GBA_SECURITY_KEY1,
    PKSAV_GBA_SECURITY_KEY2
};

static const size_t SECTION0_OFFSETS[][6] =
{
    {0x0019,0x0028,0x005C,0x00AC,0x00AC,0x00AC},
    {0x0019,0x0028,0x005C,0x00AC,0x00AC,0x01F4},
    {0x001B,0x0028,0x005C,0x00AC,0x0A48,0x0F28}
};

enum pksav_gba_section1_field
{
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_CASINO_COINS,
    PKSAV_GBA_ITEM_PC,
    PKSAV_GBA_ITEM_BAG,
    PKSAV_GBA_POKEDEX_SEEN_B
};

static const size_t SECTION1_OFFSETS[][6] =
{
    {0x0234,0x0490,0x0494,0x0498,0x0560,0x0938},
    {0x0234,0x0490,0x0494,0x0498,0x0560,0x0988},
    {0x0034,0x0290,0x0294,0x0298,0x0310,0x0788}
};

enum pksav_gba_section2_field
{
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B = 0,
    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_C
};

static const size_t SECTION2_OFFSETS[][2] =
{
    {0x03A6,0x044C},
    {0x0402,0x04A8},
    {0x0068,0x011C}
};

enum pksav_gba_section4_field
{
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
};

static const size_t SECTION4_OFFSETS[][2] =
{
    {0x0C0C,0x0000},
    {0x0CA4,0x0000},
    {0x0B98,0x0Bcc}
};

static union pksav_gba_save_slot* _pksav_gba_get_active_save_slot_ptr(
    uint8_t* buffer,
    size_t buffer_len
)
{
    assert(buffer != NULL);

    union pksav_gba_save_slot* active_save_slot_ptr = NULL;

    if(buffer_len >= PKSAV_GBA_SAVE_SIZE)
    {
        union pksav_gba_save_slot* save_slots = (union pksav_gba_save_slot*)buffer;
        uint32_t save_index1 = pksav_littleendian32(
                                   save_slots[0].section0.footer.save_index
                               );
        uint32_t save_index2 = pksav_littleendian32(
                                   save_slots[1].section0.footer.save_index
                               );

        if(save_index1 > save_index2)
        {
            active_save_slot_ptr = &save_slots[0];
        }
        else
        {
            active_save_slot_ptr = &save_slots[1];
        }
    }

    return active_save_slot_ptr;
}

enum pksav_error pksav_gba_get_buffer_save_type(
    const uint8_t* buffer,
    size_t buffer_len,
    enum pksav_gba_save_type* save_type_out
)
{
    if(!buffer || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    *save_type_out = PKSAV_GBA_SAVE_TYPE_NONE;
    const union pksav_gba_save_slot* save_slot_ptr = _pksav_gba_get_active_save_slot_ptr(
                                                          (uint8_t*)buffer,
                                                          buffer_len
                                                     );
    if(save_slot_ptr != NULL)
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
            const struct pksav_gba_section_footer* section1_footer_ptr =
                &save_slot_ptr->sections_arr[section_index].footer;

            if((section1_footer_ptr->section_id > (PKSAV_GBA_NUM_SAVE_SECTIONS-1)) ||
               (pksav_littleendian32(section1_footer_ptr->validation) != PKSAV_GBA_VALIDATION_MAGIC))
            {
                is_save_valid = false;
            }
        }

        if(is_save_valid)
        {
            union pksav_gba_save_slot unshuffled_save_slots;
            uint8_t section_nums[14]; // Unused
            pksav_gba_save_unshuffle_sections(
                save_slot_ptr,
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
                    SECTION0_OFFSETS[save_type-1][PKSAV_GBA_SECURITY_KEY1];
                const size_t security_key2_offset =
                    SECTION0_OFFSETS[save_type-1][PKSAV_GBA_SECURITY_KEY2];

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
                        SECTION0_OFFSETS[save_type-1][PKSAV_GBA_GAME_CODE];

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
                    *save_type_out = save_type;
                }
            }
        }
    }

    return error;
}

enum pksav_error pksav_gba_get_file_save_type(
    const char* filepath,
    enum pksav_gba_save_type* save_type_out
)
{
    if(!filepath || !save_type_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &file_buffer,
                &buffer_len
            );
    if(!error)
    {
        assert(file_buffer != NULL);

        enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
        error = pksav_gba_get_buffer_save_type(
                    file_buffer,
                    buffer_len,
                    &save_type
                );
        free(file_buffer);

        // Only return a result upon success.
        if(!error)
        {
            *save_type_out = save_type;
        }
    }

    return error;
}

static void _pksav_gba_set_save_pointers(
    struct pksav_gba_save* gba_save_ptr,
    uint8_t* buffer,
    size_t buffer_len,
    bool should_alloc_internal
)
{
    assert(gba_save_ptr != NULL);
    assert(gba_save_ptr->save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    assert(gba_save_ptr->save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);
    assert(buffer != NULL);

    // Offsets
    const size_t* section0_offsets_ptr = SECTION0_OFFSETS[gba_save_ptr->save_type-1];
    const size_t* section1_offsets_ptr = SECTION1_OFFSETS[gba_save_ptr->save_type-1];
    const size_t* section2_offsets_ptr = SECTION2_OFFSETS[gba_save_ptr->save_type-1];
    const size_t* section4_offsets_ptr = SECTION4_OFFSETS[gba_save_ptr->save_type-1];

    // At this point, we should know the save is valid, so we should be able
    // to get valid sections.
    union pksav_gba_save_slot* save_slot_ptr = _pksav_gba_get_active_save_slot_ptr(
                                                    buffer,
                                                    buffer_len
                                               );
    assert(save_slot_ptr != NULL);

    // Internal
    if(should_alloc_internal)
    {
        gba_save_ptr->internal_ptr = calloc(sizeof(struct pksav_gba_save_internal), 1);
    }

    struct pksav_gba_save_internal* internal_ptr = gba_save_ptr->internal_ptr;
    internal_ptr->raw_save_ptr = buffer;
    internal_ptr->save_len = buffer_len;
    internal_ptr->is_save_from_first_slot = ((uint8_t*)save_slot_ptr == buffer);

    pksav_gba_save_unshuffle_sections(
        save_slot_ptr,
        &internal_ptr->unshuffled_save_slot,
        internal_ptr->shuffled_section_nums
    );

    // Pointers to relevant sections
    struct pksav_gba_save_section* section0_ptr = &internal_ptr->unshuffled_save_slot.section0;
    struct pksav_gba_trainer_info_internal* trainer_info_internal_ptr =
        (struct pksav_gba_trainer_info_internal*)section0_ptr;

    struct pksav_gba_save_section* section1_ptr = &internal_ptr->unshuffled_save_slot.section1;
    struct pksav_gba_save_section* section2_ptr = &internal_ptr->unshuffled_save_slot.section2;
    struct pksav_gba_save_section* section4_ptr = &internal_ptr->unshuffled_save_slot.section4;

    // Security key, used in decryption
    internal_ptr->security_key_ptr = &section0_ptr->data32[
                                         section0_offsets_ptr[PKSAV_GBA_SECURITY_KEY1]/4
                                     ];

    // Time played
    gba_save_ptr->time_played_ptr = &trainer_info_internal_ptr->time_played;

    // Item storage
    struct pksav_gba_item_storage* item_storage_ptr = &gba_save_ptr->item_storage;

    item_storage_ptr->bag_ptr = (union pksav_gba_item_bag*)(
                                    &section0_ptr->data8[section0_offsets_ptr[PKSAV_GBA_ITEM_BAG]]
                                );
    pksav_gba_save_crypt_items(
        item_storage_ptr->bag_ptr,
        *internal_ptr->security_key_ptr,
        gba_save_ptr->save_type
    );

    item_storage_ptr->pc_ptr = (struct pksav_gba_item_pc*)(
                                    &section0_ptr->data8[section0_offsets_ptr[PKSAV_GBA_ITEM_PC]]
                                );

    // Pokémon storage
    struct pksav_gba_pokemon_storage* pokemon_storage_ptr = &gba_save_ptr->pokemon_storage;

    pokemon_storage_ptr->party_ptr = (struct pksav_gba_pokemon_party*)(
                                         &section1_ptr->data8[section1_offsets_ptr[PKSAV_GBA_POKEMON_PARTY]]
                                     );
    for(size_t party_index = 0;
        party_index < PKSAV_GBA_PARTY_NUM_POKEMON;
        ++party_index)
    {
        pksav_gba_crypt_pokemon(
            &pokemon_storage_ptr->party_ptr->party[party_index].pc,
            false // should_encrypt
        );
    }

    pksav_gba_save_load_pokemon_pc(
        &internal_ptr->unshuffled_save_slot,
        &internal_ptr->consolidated_pokemon_pc
    );
    pokemon_storage_ptr->pc_ptr = &internal_ptr->consolidated_pokemon_pc;

    // Pokédex
    struct pksav_gba_pokedex* pokedex_ptr = &gba_save_ptr->pokedex;

    pokedex_ptr->seen_ptrA = &section0_ptr->data8[
                                 section0_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_A]
                             ];
    pokedex_ptr->seen_ptrB = &section1_ptr->data8[
                                 section1_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_B]
                             ];
    pokedex_ptr->seen_ptrC = &section4_ptr->data8[
                                 section4_offsets_ptr[PKSAV_GBA_POKEDEX_SEEN_C]
                             ];
    pokedex_ptr->owned_ptr = &section0_ptr->data8[
                                 section0_offsets_ptr[PKSAV_GBA_POKEDEX_OWNED]
                             ];

    if(gba_save_ptr->save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
    {
        pokedex_ptr->frlg_nat_pokedex_unlocked_ptrA =
            &section0_ptr->data8[section0_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A]];

        pokedex_ptr->rse_nat_pokedex_unlocked_ptrA = NULL;
    }
    else
    {
        pokedex_ptr->rse_nat_pokedex_unlocked_ptrA =
            &section0_ptr->data16[section0_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_A]/2];

        pokedex_ptr->frlg_nat_pokedex_unlocked_ptrA = NULL;
    }

    pokedex_ptr->nat_pokedex_unlocked_ptrB =
        &section2_ptr->data8[section2_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B]];

    pokedex_ptr->nat_pokedex_unlocked_ptrC =
        &section2_ptr->data16[section2_offsets_ptr[PKSAV_GBA_NAT_POKEDEX_UNLOCKED_C]/2];

    // Trainer Info
    struct pksav_gba_trainer_info* trainer_info_ptr = &gba_save_ptr->trainer_info;

    trainer_info_ptr->id_ptr = &trainer_info_internal_ptr->id;
    trainer_info_ptr->name_ptr = trainer_info_internal_ptr->name;
    trainer_info_ptr->gender_ptr = &trainer_info_internal_ptr->gender;

    trainer_info_ptr->money_ptr = &section1_ptr->data32[
                                      section1_offsets_ptr[PKSAV_GBA_MONEY]
                                  ];
    *trainer_info_ptr->money_ptr ^= *internal_ptr->security_key_ptr;

    // Misc Fields
    struct pksav_gba_misc_fields* misc_fields_ptr = &gba_save_ptr->misc_fields;

    if(gba_save_ptr->save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
    {
        misc_fields_ptr->rival_name_ptr = &section4_ptr->data8[
                                              section4_offsets_ptr[PKSAV_GBA_FRLG_RIVAL_NAME]
                                          ];
    }
    else
    {
        misc_fields_ptr->rival_name_ptr = NULL;
    }

    misc_fields_ptr->casino_coins_ptr = &section1_ptr->data16[
                                            section1_offsets_ptr[PKSAV_GBA_CASINO_COINS]/2
                                        ];
    *misc_fields_ptr->casino_coins_ptr ^= (*internal_ptr->security_key_ptr & 0xFFFF);
}

static enum pksav_error _pksav_gba_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    bool is_buffer_ours,
    struct pksav_gba_save* gba_save_out
)
{
    assert(gba_save_out != NULL);
    assert(buffer != NULL);

    enum pksav_error error = PKSAV_ERROR_NONE;

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_buffer_save_type(
                buffer,
                buffer_len,
                &save_type
            );
    if(!error)
    {
        if(save_type != PKSAV_GBA_SAVE_TYPE_NONE)
        {
            gba_save_out->save_type = save_type;
            _pksav_gba_set_save_pointers(
                gba_save_out,
                buffer,
                buffer_len,
                true // should_alloc_internal
            );

            // Internal
            struct pksav_gba_save_internal* internal_ptr = gba_save_out->internal_ptr;
            internal_ptr->is_buffer_ours = is_buffer_ours;
        }
        else
        {
            error = PKSAV_ERROR_INVALID_SAVE;
        }
    }

    return error;
}

enum pksav_error pksav_gba_load_save_from_buffer(
    uint8_t* buffer,
    size_t buffer_len,
    struct pksav_gba_save* gba_save_out
)
{
    if(!buffer || !gba_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    return _pksav_gba_load_save_from_buffer(
               buffer,
               buffer_len,
               false, // is_buffer_ours
               gba_save_out
           );
}

enum pksav_error pksav_gba_load_save_from_file(
    const char* filepath,
    struct pksav_gba_save* gba_save_out
)
{
    if(!filepath || !gba_save_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t* file_buffer = NULL;
    size_t buffer_len = 0;
    error = pksav_fs_read_file_to_buffer(
                filepath,
                &file_buffer,
                &buffer_len
            );

    if(!error)
    {
        error = _pksav_gba_load_save_from_buffer(
                    file_buffer,
                    buffer_len,
                    true, // is_buffer_ours
                    gba_save_out
                );
        if(error)
        {
            // We made this buffer, so it's on us to free it if there's
            // an error.
            free(file_buffer);
        }
    }

    return error;
}

enum pksav_error pksav_gba_save_save(
    const char* filepath,
    struct pksav_gba_save* gba_save_ptr
)
{
    if(!filepath || !gba_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    enum pksav_error error = PKSAV_ERROR_NONE;

    struct pksav_gba_save_internal* internal_ptr = gba_save_ptr->internal_ptr;

    // Item Storage
    pksav_gba_save_crypt_items(
        gba_save_ptr->item_storage.bag_ptr,
        *internal_ptr->security_key_ptr,
        gba_save_ptr->save_type
    );

    // Pokémon Storage
    for(size_t party_index = 0;
        party_index < PKSAV_GBA_PARTY_NUM_POKEMON;
        ++party_index)
    {
        pksav_gba_crypt_pokemon(
            &gba_save_ptr->pokemon_storage.party_ptr->party[party_index].pc,
            true // should_encrypt
        );
    }

    pksav_gba_save_save_pokemon_pc(
        &internal_ptr->consolidated_pokemon_pc,
        &internal_ptr->unshuffled_save_slot
    );

    // Trainer Info
    *gba_save_ptr->trainer_info.money_ptr ^= *internal_ptr->security_key_ptr;

    // Misc Fields
    *gba_save_ptr->misc_fields.casino_coins_ptr ^= (*internal_ptr->security_key_ptr & 0xFFFF);

    union pksav_gba_save_slot* raw_sections_ptr = (union pksav_gba_save_slot*)(
                                                      internal_ptr->raw_save_ptr
                                                  );
    union pksav_gba_save_slot* output_save_slot_ptr =
        internal_ptr->is_save_from_first_slot ? &raw_sections_ptr[1]
                                              : &raw_sections_ptr[0];

    pksav_gba_set_section_checksums(&internal_ptr->unshuffled_save_slot);
    pksav_gba_save_shuffle_sections(
        &internal_ptr->unshuffled_save_slot,
        output_save_slot_ptr,
        internal_ptr->shuffled_section_nums
    );

    error = pksav_fs_write_buffer_to_file(
                filepath,
                internal_ptr->raw_save_ptr,
                internal_ptr->save_len
            );

    // With everything saved to the new slot, reset the pointers.
    _pksav_gba_set_save_pointers(
        gba_save_ptr,
        internal_ptr->raw_save_ptr,
        internal_ptr->save_len,
        false // should_alloc_internal
    );

    return error;
}

enum pksav_error pksav_gba_free_save(
    struct pksav_gba_save* gba_save_ptr
)
{
    if(!gba_save_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    struct pksav_gba_save_internal* internal_ptr = gba_save_ptr->internal_ptr;
    if(internal_ptr->is_buffer_ours)
    {
        free(internal_ptr->raw_save_ptr);
    }
    free(internal_ptr);

    // Everything else is a pointer or an enum with a default value of 0,
    // so this one memset should be fine.
    memset(
        gba_save_ptr,
        0,
        sizeof(*gba_save_ptr)
    );

    return PKSAV_ERROR_NONE;
}
