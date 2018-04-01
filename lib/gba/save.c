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

struct pksav_gba_save_internal
{
    uint8_t* raw_save_ptr;

    union pksav_gba_save_slot unshuffled_save_slot;
    bool is_small_save;
    uint8_t shuffled_section_nums[PKSAV_GBA_NUM_SAVE_SECTIONS];

    bool is_buffer_ours;
};

/*
 * Each footer has a field that must equal this value to be considered valid.
 */
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

static const size_t pksav_gba_section0_offsets[][3] =
{
    {0x0019,0x0019,0x001B}, // National Pokédex Unlocked A
    {0x0028,0x0028,0x0028}, // Pokédex Owned
    {0x005C,0x005C,0x005C}, // Pokédex Seen A
    {0x00AC,0x00AC,0x00AC}, // Game Code
    {0x00AC,0x00AC,0x0AF8}, // Security Key 1
    {0x00AC,0x01F4,0x0F20}  // Security Key 2
};

enum pksav_gba_section1_field
{
    PKSAV_GBA_POKEMON_PARTY = 0,
    PKSAV_GBA_MONEY,
    PKSAV_GBA_CASINO_COINS,
    PKSAV_GBA_ITEM_PC,
    PKSAV_GBA_ITEM_BAG,
    PKSAV_GBA_POKEDEX_SEEN_B
} pksav_gba_section1_field;

static const size_t pksav_gba_section1_offsets[][3] =
{
    {0x0234,0x0234,0x0034}, // Pokémon Party
    {0x0490,0x0490,0x0290}, // Money
    {0x0494,0x0494,0x0294}, // Casino Coins
    {0x0498,0x0498,0x0298}, // Item PC
    {0x0560,0x0560,0x0310}, // Item Bag
    {0x0938,0x0988,0x0788}  // Pokédex Seen B
};

enum pksav_gba_section2_field
{
    PKSAV_GBA_NAT_POKEDEX_B = 0,
    PKSAV_GBA_NAT_POKEDEX_C
};

static const size_t pksav_gba_section2_offsets[][3] =
{
    {0x03A6,0x0402,0x0068}, // National Pokédex Unlocked B
    {0x044C,0x04A8,0x011C}  // National Pokédex Unlocked C
};

enum pksav_gba_section4_field
{
    PKSAV_GBA_POKEDEX_SEEN_C = 0,
    PKSAV_GBA_FRLG_RIVAL_NAME
};

static const size_t pksav_gba_section4_offsets[][3] =
{
    {0x0C0C,0x0CA4,0x0B98}, // Pokédex Seen C
    {0x0000,0x0000,0x0BCC}  // Rival Name (FR/LG only)
};

static union pksav_gba_save_slot* _pksav_gba_get_active_save_slot_ptr(
    uint8_t* buffer,
    size_t buffer_len
)
{
    assert(buffer != NULL);

    union pksav_gba_save_slot* active_save_slot_ptr = NULL;

    if(buffer_len >= PKSAV_GBA_SMALL_SAVE_SIZE)
    {
        union pksav_gba_save_slot* save_slots = (union pksav_gba_save_slot*)buffer;
        if(buffer_len < PKSAV_GBA_LARGE_SAVE_SIZE)
        {
            active_save_slot_ptr = &save_slots[0];
        }
        else
        {
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
                    pksav_gba_section0_offsets[PKSAV_GBA_SECURITY_KEY1][save_type-1];
                const size_t security_key2_offset =
                    pksav_gba_section0_offsets[PKSAV_GBA_SECURITY_KEY2][save_type-1];

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
                        pksav_gba_section0_offsets[PKSAV_GBA_GAME_CODE][save_type-1];

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
    size_t buffer_len
)
{
    assert(gba_save_ptr != NULL);
    assert(buffer != NULL);

    // At this point, we should know the save is valid, so we should be able
    // to get valid sections.
    union pksav_gba_save_slot* save_slot_ptr = _pksav_gba_get_active_save_slot_ptr(
                                                    buffer,
                                                    buffer_len
                                               );
    assert(save_slot_ptr != NULL);

    // Internal
    gba_save_ptr->internal_ptr = calloc(sizeof(struct pksav_gba_save_internal), 1);
    struct pksav_gba_save_internal* internal_ptr = gba_save_ptr->internal_ptr;
    internal_ptr->raw_save_ptr = buffer;

    pksav_gba_save_unshuffle_sections(
        save_slot_ptr,
        &internal_ptr->unshuffled_save_slot,
        internal_ptr->shuffled_section_nums
    );

    struct pksav_gba_save_section* section0_ptr = &internal_ptr->unshuffled_save_slot.section0;
    struct pksav_gba_trainer_info_internal* trainer_info_ptr =
        (struct pksav_gba_trainer_info_internal*)section0_ptr;

    struct pksav_gba_save_section* section1_ptr = &internal_ptr->unshuffled_save_slot.section1;
    struct pksav_gba_save_section* section2_ptr = &internal_ptr->unshuffled_save_slot.section2;
    struct pksav_gba_save_section* section4_ptr = &internal_ptr->unshuffled_save_slot.section4;

    // Time played

    // Pokémon storage
}

/*
// Assumes all dynamically allocated memory has already been allocated
static void _pksav_gba_save_set_pointers(
    pksav_gba_save_t* gba_save
) {
    // Find the most recent save slot
    const union pksav_gba_save_slot* sections_pair = (const union pksav_gba_save_slot*)gba_save->raw;
    const union pksav_gba_save_slot* most_recent;

    if(gba_save->small_save) {
        most_recent = sections_pair;
        gba_save->from_first_slot = true;
    } else {
        if(SAVE_INDEX(&sections_pair[0]) > SAVE_INDEX(&sections_pair[1])) {
            most_recent = &sections_pair[0];
            gba_save->from_first_slot = true;
        } else {
            most_recent = &sections_pair[1];
            gba_save->from_first_slot = false;
        }
    }

    // Set pointers
    pksav_gba_save_unshuffle_sections(
        most_recent,
        gba_save->unshuffled,
        gba_save->shuffled_section_nums
    );
    gba_save->trainer_info = &gba_save->unshuffled->trainer_info;
    if(gba_save->gba_game == PKSAV_GBA_FRLG) {
        gba_save->rival_name = &SECTION4_DATA8(
                                   gba_save->unshuffled,
                                   gba_save->gba_game,
                                   PKSAV_GBA_FRLG_RIVAL_NAME
                               );
    } else {
        gba_save->rival_name = NULL;
    }
    gba_save->pokemon_party = (struct pksav_gba_pokemon_party*)&SECTION1_DATA8(
                                                              gba_save->unshuffled,
                                                              gba_save->gba_game,
                                                              PKSAV_GBA_POKEMON_PARTY
                                                          );
    for(uint8_t i = 0; i < 6; ++i) {
        pksav_gba_crypt_pokemon(
            &gba_save->pokemon_party->party[i].pc,
            false
        );
    }

    pksav_gba_save_load_pokemon_pc(
        gba_save->unshuffled,
        gba_save->pokemon_pc
    );

    gba_save->item_storage = (union pksav_gba_item_bag*)&SECTION1_DATA8(
                                                            gba_save->unshuffled,
                                                            gba_save->gba_game,
                                                            PKSAV_GBA_ITEM_PC
                                                        );
    pksav_gba_save_crypt_items(
        gba_save->item_storage,
        SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game),
        gba_save->gba_game
    );

    gba_save->money = &SECTION1_DATA32(
                          gba_save->unshuffled,
                          gba_save->gba_game,
                          PKSAV_GBA_MONEY
                      );
    *gba_save->money ^= SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game);

    gba_save->casino_coins = &SECTION1_DATA16(
                                 gba_save->unshuffled,
                                 gba_save->gba_game,
                                 PKSAV_GBA_CASINO_COINS
                             );
    *gba_save->casino_coins ^= (uint16_t)(SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game) & 0xFFFF);

    gba_save->pokedex_owned = &SECTION0_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_OWNED
                              );

    gba_save->pokedex_seenA = &SECTION0_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_A
                              );

    gba_save->pokedex_seenB = &SECTION1_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_B
                              );

    gba_save->pokedex_seenC = &SECTION4_DATA8(
                                  gba_save->unshuffled,
                                  gba_save->gba_game,
                                  PKSAV_GBA_POKEDEX_SEEN_C
                              );

    if(gba_save->gba_game == PKSAV_GBA_FRLG) {
        gba_save->rse_nat_pokedex_unlockedA = NULL;

        gba_save->frlg_nat_pokedex_unlockedA = &SECTION0_DATA8(
                                                   gba_save->unshuffled,
                                                   gba_save->gba_game,
                                                   PKSAV_GBA_NAT_POKEDEX_A
                                               );
    } else {
        gba_save->rse_nat_pokedex_unlockedA = &SECTION0_DATA16(
                                                  gba_save->unshuffled,
                                                  gba_save->gba_game,
                                                  PKSAV_GBA_NAT_POKEDEX_A
                                              );
        gba_save->frlg_nat_pokedex_unlockedA = NULL;
    }

    gba_save->nat_pokedex_unlockedB = &SECTION2_DATA8(
                                          gba_save->unshuffled,
                                          gba_save->gba_game,
                                          PKSAV_GBA_NAT_POKEDEX_B
                                      );

    gba_save->nat_pokedex_unlockedC = &SECTION2_DATA16(
                                          gba_save->unshuffled,
                                          gba_save->gba_game,
                                          PKSAV_GBA_NAT_POKEDEX_C
                                      );
}

pksav_error_t pksav_gba_save_load(
    const char* filepath,
    pksav_gba_save_t* gba_save
) {
    if(!filepath || !gba_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Read the file and make sure it's valid
    FILE* gba_save_file = fopen(filepath, "rb");
    if(!gba_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    fseek(gba_save_file, 0, SEEK_END);
    size_t filesize = ftell(gba_save_file);

    if(filesize < PKSAV_GBA_SMALL_SAVE_SIZE) {
        fclose(gba_save_file);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    gba_save->raw = calloc(filesize, 1);
    fseek(gba_save_file, 0, SEEK_SET);
    size_t num_read = fread((void*)gba_save->raw, 1, filesize, gba_save_file);
    fclose(gba_save_file);
    if(num_read != filesize) {
        return PKSAV_ERROR_FILE_IO;
    }

    gba_save->small_save = (filesize < PKSAV_GBA_SAVE_SIZE);

    // Detect what kind of save this is
    bool found = false;
    for(pksav_gba_game_t i = PKSAV_GBA_RS; i <= PKSAV_GBA_FRLG; ++i) {
        pksav_buffer_is_gba_save(
            gba_save->raw,
            filesize,
            i,
            &found
        );
        if(found) {
            gba_save->gba_game = i;
            break;
        }
    }

    if(!found) {
        free(gba_save->raw);
        return PKSAV_ERROR_INVALID_SAVE;
    }

    // Allocate memory as needed and set pointers
    gba_save->unshuffled = calloc(sizeof(union pksav_gba_save_slot), 1);
    gba_save->pokemon_pc = calloc(sizeof(struct pksav_gba_pokemon_pc), 1);
    _pksav_gba_save_set_pointers(
        gba_save
    );

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gba_save_save(
    const char* filepath,
    pksav_gba_save_t* gba_save
) {
    if(!filepath || !gba_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    // Make sure we can write to this file
    FILE* gba_save_file = fopen(filepath, "wb");
    if(!gba_save_file) {
        return PKSAV_ERROR_FILE_IO;
    }

    *gba_save->money ^= SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game);
    *gba_save->casino_coins ^= (uint16_t)(SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game) & 0xFFFF);

    pksav_gba_save_crypt_items(
        gba_save->item_storage,
        SECURITY_KEY1(gba_save->unshuffled, gba_save->gba_game),
        gba_save->gba_game
    );

    pksav_gba_save_save_pokemon_pc(
        gba_save->pokemon_pc,
        gba_save->unshuffled
    );

    for(uint8_t i = 0; i < 6; ++i) {
        pksav_gba_crypt_pokemon(
            &gba_save->pokemon_party->party[i].pc,
            true
        );
    }

    // Find the least recent save slot, increment the save index, save into that
    uint32_t save_index = pksav_littleendian32(SAVE_INDEX(gba_save->unshuffled) + 1);
    for(uint8_t i = 0; i < 14; ++i) {
        gba_save->unshuffled->sections_arr[i].footer.save_index = save_index;
    }

    union pksav_gba_save_slot* save_into = NULL;
    if(!gba_save->small_save) {
        union pksav_gba_save_slot* sections_pair = (union pksav_gba_save_slot*)gba_save->raw;
        save_into = gba_save->from_first_slot ? &sections_pair[1] : &sections_pair[0];
        gba_save->from_first_slot = !gba_save->from_first_slot;
    } else {
        save_into = (union pksav_gba_save_slot*)gba_save->raw;
    }

    pksav_gba_set_section_checksums(
        gba_save->unshuffled
    );
    pksav_gba_save_shuffle_sections(
        gba_save->unshuffled,
        save_into,
        gba_save->shuffled_section_nums
    );

    // With everything saved to the new slot, reload it
    _pksav_gba_save_set_pointers(
        gba_save
    );

    // Write to file
    fwrite(
        (void*)gba_save->raw,
        1,
        (gba_save->small_save ? PKSAV_GBA_SMALL_SAVE_SIZE : PKSAV_GBA_SAVE_SIZE),
        gba_save_file
    );

    fclose(gba_save_file);

    return PKSAV_ERROR_NONE;
}

pksav_error_t pksav_gba_save_free(
    pksav_gba_save_t* gba_save
) {
    if(!gba_save) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    free(gba_save->pokemon_pc);
    free(gba_save->unshuffled);
    free(gba_save->raw);

    return PKSAV_ERROR_NONE;
}
*/
