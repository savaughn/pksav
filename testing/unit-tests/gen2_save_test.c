/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include "util/fs.h"

#include <pksav/config.h>
#include <pksav/gen2/save.h>
#include <pksav/gen2/text.h>
#include <pksav/math/bcd.h>

#include <stdio.h>
#include <string.h>

#define STRBUFFER_LEN 64

static const struct pksav_gen2_save EMPTY_GEN2_SAVE =
{
    .save_type = PKSAV_GEN2_SAVE_TYPE_NONE,

    .save_time =
    {
        .p_time_played      = NULL,
        .p_daylight_savings = NULL
    },

    .item_storage =
    {
        .p_item_bag = NULL,
        .p_item_pc  = NULL
    },

    .pokemon_storage =
    {
        .p_party           = NULL,
        .pp_boxes            = {NULL},
        .p_current_box_num = NULL,
        .p_current_box     = NULL
    },

    .pokedex_lists =
    {
        .p_seen  = NULL,
        .p_owned = NULL
    },

    .trainer_info =
    {
        .p_id           = NULL,
        .p_name         = NULL,
        .p_gender       = NULL,
        .p_palette      = NULL,
        .p_money        = NULL,
        .p_johto_badges = NULL,
        .p_kanto_badges = NULL
    },

    .misc_fields =
    {
        .p_rival_name = NULL
    },

    .p_internal = NULL
};

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_gen2_get_buffer_save_type_on_random_buffer_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t buffer[PKSAV_GEN2_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        error = pksav_gen2_get_buffer_save_type(
                    buffer,
                    sizeof(buffer),
                    &save_type
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);
    }
}

static void pksav_gen2_get_buffer_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen2_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GEN2_SAVE_TYPE_GS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GEN2_SAVE_TYPE_CRYSTAL);

    char filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves)
    {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    uint8_t* save_buffer = NULL;
    size_t save_size = 0;
    if(pksav_fs_read_file_to_buffer(filepath, &save_buffer, &save_size))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }
    TEST_ASSERT_TRUE(save_size >= PKSAV_GEN2_SAVE_SIZE);

    enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    error = pksav_gen2_get_buffer_save_type(
                save_buffer,
                PKSAV_GEN2_SAVE_SIZE,
                &save_type
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);

    free(save_buffer);
}

static void pksav_gen2_get_file_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen2_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GEN2_SAVE_TYPE_GS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GEN2_SAVE_TYPE_CRYSTAL);

    char filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves)
    {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    error = pksav_gen2_get_file_save_type(
                filepath,
                &save_type
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
}

static void validate_gen2_string(
    const uint8_t* buffer,
    size_t buffer_len
)
{
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_TRUE(buffer_len > 0);

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pksav_error error = pksav_gen2_import_text(
                                 buffer,
                                 strbuffer,
                                 buffer_len
                             );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(strlen(strbuffer) > 0ULL);
}

static void validate_gen2_item_bag(
    const struct pksav_gen2_item_bag* p_item_bag
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);

    // TODO: validate pockets
}

static void validate_bcd(
    const uint8_t* buffer,
    size_t num_bytes,
    size_t max_value
)
{
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_TRUE(num_bytes > 0);
    TEST_ASSERT_TRUE(max_value > 0);

    size_t bcd_value = 0;
    enum pksav_error error = pksav_import_bcd(
                                 buffer,
                                 num_bytes,
                                 &bcd_value
                             );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(bcd_value <= max_value);
}

static void validate_gen2_item_pc(
    const struct pksav_gen2_item_pc* p_item_pc
)
{
    TEST_ASSERT_NOT_NULL(p_item_pc);
    TEST_ASSERT_TRUE(p_item_pc->count <= PKSAV_GEN2_ITEM_PC_SIZE);

    // Don't validate item contents. Generation I games are buggy enough
    // to make this unreliable.
}

static void validate_gen2_pokemon_party(
    const struct pksav_gen2_pokemon_party* p_party
)
{
    TEST_ASSERT_NOT_NULL(p_party);
    TEST_ASSERT_TRUE(p_party->count <= PKSAV_GEN2_PARTY_NUM_POKEMON);

    if(p_party->count < PKSAV_GEN2_PARTY_NUM_POKEMON)
    {
        TEST_ASSERT_EQUAL(0xFF, p_party->species[p_party->count]);
    }

    for(size_t party_index = 0; party_index < p_party->count; ++party_index)
    {
        if(p_party->species[party_index] != PKSAV_GEN2_POKEMON_EGG_INDEX)
        {
            validate_gen2_string(
                p_party->otnames[party_index],
                10
            );
            validate_gen2_string(
                p_party->nicknames[party_index],
                10
            );
        }
    }
}

static void validate_gen2_pokemon_box(
    const struct pksav_gen2_pokemon_box* p_pokemon_box
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon_box);
    TEST_ASSERT_TRUE(p_pokemon_box->count <= PKSAV_GEN2_BOX_NUM_POKEMON);

    if(p_pokemon_box->count < PKSAV_GEN2_BOX_NUM_POKEMON)
    {
        TEST_ASSERT_EQUAL(0xFF, p_pokemon_box->species[p_pokemon_box->count]);

        for(size_t box_index = 0; box_index < p_pokemon_box->count; ++box_index)
        {
            if(p_pokemon_box->species[box_index] != PKSAV_GEN2_POKEMON_EGG_INDEX)
            {
                validate_gen2_string(
                    p_pokemon_box->nicknames[box_index],
                    10
                );
                validate_gen2_string(
                    p_pokemon_box->otnames[box_index],
                    10
                );
            }
        }
    }
}

static void compare_gen2_saves(
    struct pksav_gen2_save* gen2_save1p_,
    struct pksav_gen2_save* gen2_save2p_
)
{
    TEST_ASSERT_NOT_NULL(gen2_save1p_);
    TEST_ASSERT_NOT_NULL(gen2_save2p_);

    TEST_ASSERT_EQUAL(
        gen2_save1p_->save_type,
        gen2_save2p_->save_type
    );

    // Save time
    struct pksav_gen2_save_time* save_time1p_ = &gen2_save1p_->save_time;
    struct pksav_gen2_save_time* save_time2p_ = &gen2_save2p_->save_time;

    TEST_ASSERT_EQUAL_MEMORY(
        save_time1p_->p_time_played,
        save_time2p_->p_time_played,
        sizeof(*save_time1p_->p_time_played)
    );
    TEST_ASSERT_EQUAL(
        *save_time1p_->p_daylight_savings,
        *save_time2p_->p_daylight_savings
    );

    // Item storage
    struct pksav_gen2_item_storage* item_storage1p_ = &gen2_save1p_->item_storage;
    struct pksav_gen2_item_storage* item_storage2p_ = &gen2_save2p_->item_storage;

    TEST_ASSERT_EQUAL_MEMORY(
        item_storage1p_->p_item_bag,
        item_storage2p_->p_item_bag,
        sizeof(*item_storage1p_->p_item_bag)
    );
    TEST_ASSERT_EQUAL_MEMORY(
        item_storage1p_->p_item_pc,
        item_storage2p_->p_item_pc,
        sizeof(*item_storage1p_->p_item_pc)
    );

    // Pokémon storage
    struct pksav_gen2_pokemon_storage* pokemon_storage1p_ = &gen2_save1p_->pokemon_storage;
    struct pksav_gen2_pokemon_storage* pokemon_storage2p_ = &gen2_save2p_->pokemon_storage;

    TEST_ASSERT_EQUAL_MEMORY(
        pokemon_storage1p_->p_party,
        pokemon_storage2p_->p_party,
        sizeof(*pokemon_storage1p_->p_party)
    );
    for(size_t box_index = 0; box_index < PKSAV_GEN2_NUM_POKEMON_BOXES; ++box_index)
    {
        TEST_ASSERT_EQUAL_MEMORY(
            pokemon_storage1p_->pp_boxes[box_index],
            pokemon_storage2p_->pp_boxes[box_index],
            sizeof(*pokemon_storage1p_->pp_boxes[box_index])
        );
    }
    TEST_ASSERT_EQUAL_MEMORY(
        pokemon_storage1p_->p_box_names,
        pokemon_storage2p_->p_box_names,
        sizeof(*pokemon_storage1p_->p_box_names)
    );
    TEST_ASSERT_EQUAL(
        *pokemon_storage1p_->p_current_box_num,
        *pokemon_storage2p_->p_current_box_num
    );
    TEST_ASSERT_EQUAL_MEMORY(
        pokemon_storage1p_->p_current_box,
        pokemon_storage2p_->p_current_box,
        sizeof(*pokemon_storage1p_->p_current_box)
    );

    // Pokédex lists
    struct pksav_gen2_pokedex_lists* pokedex_lists1p_ = &gen2_save1p_->pokedex_lists;
    struct pksav_gen2_pokedex_lists* pokedex_lists2p_ = &gen2_save2p_->pokedex_lists;
    const size_t pokedex_buffer_size = (251 / 8) + 1;

    TEST_ASSERT_EQUAL_MEMORY(
        pokedex_lists1p_->p_seen,
        pokedex_lists2p_->p_seen,
        pokedex_buffer_size
    );
    TEST_ASSERT_EQUAL_MEMORY(
        pokedex_lists1p_->p_owned,
        pokedex_lists2p_->p_owned,
        pokedex_buffer_size
    );

    // Trainer info
    struct pksav_gen2_trainer_info* trainer_info1p_ = &gen2_save1p_->trainer_info;
    struct pksav_gen2_trainer_info* trainer_info2p_ = &gen2_save2p_->trainer_info;

    TEST_ASSERT_EQUAL(
        *trainer_info1p_->p_id,
        *trainer_info2p_->p_id
    );
    TEST_ASSERT_EQUAL_MEMORY(
        trainer_info1p_->p_name,
        trainer_info2p_->p_name,
        7
    );
    if(gen2_save1p_->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        TEST_ASSERT_EQUAL(
            *trainer_info1p_->p_gender,
            *trainer_info2p_->p_gender
        );
    }
    TEST_ASSERT_EQUAL(
        *trainer_info1p_->p_palette,
        *trainer_info2p_->p_palette
    );
    TEST_ASSERT_EQUAL_MEMORY(
        trainer_info1p_->p_money,
        trainer_info2p_->p_money,
        3
    );
    TEST_ASSERT_EQUAL(
        *trainer_info1p_->p_johto_badges,
        *trainer_info2p_->p_johto_badges
    );
    TEST_ASSERT_EQUAL(
        *trainer_info1p_->p_kanto_badges,
        *trainer_info2p_->p_kanto_badges
    );

    // Trainer info
    struct pksav_gen2_misc_fields* misc_fields1p_ = &gen2_save1p_->misc_fields;
    struct pksav_gen2_misc_fields* misc_fields2p_ = &gen2_save2p_->misc_fields;

    TEST_ASSERT_EQUAL_MEMORY(
        misc_fields1p_->p_rival_name,
        misc_fields2p_->p_rival_name,
        7
    );
}

static void gen2_save_test(
    struct pksav_gen2_save* p_gen2_save,
    enum pksav_gen2_save_type expected_save_type,
    const char* original_filepath,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(p_gen2_save);
    TEST_ASSERT_NOT_NULL(original_filepath);
    TEST_ASSERT_NOT_NULL(save_name);

    char tmp_save_filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

    snprintf(
        tmp_save_filepath, sizeof(tmp_save_filepath),
        "%s%spksav_%d_%s",
        get_tmp_dir(), FS_SEPARATOR, get_pid(), save_name
    );

    // Validate fields. No pointers should be NULL (except the trainer gender
    // pointer for Gold/Silver), and some fields have a specific set of valid values.
    TEST_ASSERT_NOT_NULL(p_gen2_save->p_internal);

    TEST_ASSERT_EQUAL(expected_save_type, p_gen2_save->save_type);
    TEST_ASSERT_NOT_NULL(p_gen2_save->save_time.p_time_played);
    TEST_ASSERT_NOT_NULL(p_gen2_save->save_time.p_daylight_savings);

    validate_gen2_item_bag(p_gen2_save->item_storage.p_item_bag);
    validate_gen2_item_pc(p_gen2_save->item_storage.p_item_pc);
    validate_gen2_pokemon_party(p_gen2_save->pokemon_storage.p_party);

    for(size_t box_index = 0; box_index < PKSAV_GEN2_NUM_POKEMON_BOXES; ++box_index)
    {
        validate_gen2_string(
            p_gen2_save->pokemon_storage.p_box_names->names[box_index],
            9
        );
        validate_gen2_pokemon_box(
            p_gen2_save->pokemon_storage.pp_boxes[box_index]
        );
    }

    TEST_ASSERT_NOT_NULL(p_gen2_save->pokemon_storage.p_current_box_num);
    TEST_ASSERT_TRUE(*p_gen2_save->pokemon_storage.p_current_box_num <=
                     PKSAV_GEN2_NUM_POKEMON_BOXES);

    validate_gen2_pokemon_box(p_gen2_save->pokemon_storage.p_current_box);

    TEST_ASSERT_NOT_NULL(p_gen2_save->pokedex_lists.p_seen);
    TEST_ASSERT_NOT_NULL(p_gen2_save->pokedex_lists.p_owned);

    TEST_ASSERT_NOT_NULL(p_gen2_save->trainer_info.p_id);
    validate_gen2_string(
        p_gen2_save->trainer_info.p_name,
        7
    );

    if(p_gen2_save->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        TEST_ASSERT_NOT_NULL(p_gen2_save->trainer_info.p_gender);
    }
    else
    {
        TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_gender);
    }

    TEST_ASSERT_NOT_NULL(p_gen2_save->trainer_info.p_palette);
    validate_bcd(
        p_gen2_save->trainer_info.p_money,
        3,
        999999
    );
    TEST_ASSERT_NOT_NULL(p_gen2_save->trainer_info.p_johto_badges);
    TEST_ASSERT_NOT_NULL(p_gen2_save->trainer_info.p_kanto_badges);

    validate_gen2_string(
        p_gen2_save->misc_fields.p_rival_name,
        7
    );

    // Make sure loading and saving are perfectly symmetrical. As the checksum
    // is not guaranteed to be set for some reason, compare each part.
    error = pksav_gen2_save_save(
                tmp_save_filepath,
                p_gen2_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    struct pksav_gen2_save tmp_gen2_save = EMPTY_GEN2_SAVE;
    error = pksav_gen2_load_save_from_file(
                tmp_save_filepath,
                &tmp_gen2_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    compare_gen2_saves(
        p_gen2_save,
        &tmp_gen2_save
    );

    error = pksav_gen2_free_save(&tmp_gen2_save);
    PKSAV_TEST_ASSERT_SUCCESS(error);

    if(delete_file(tmp_save_filepath))
    {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }

    // Make sure setting the current box works as expected.
    for(uint8_t box_index = 0; box_index < PKSAV_GEN2_NUM_POKEMON_BOXES; ++box_index)
    {
        error = pksav_gen2_pokemon_storage_set_current_box(
                    &p_gen2_save->pokemon_storage,
                    box_index
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL(
            box_index,
            *p_gen2_save->pokemon_storage.p_current_box_num
        );
        TEST_ASSERT_EQUAL_MEMORY(
            p_gen2_save->pokemon_storage.pp_boxes[box_index],
            p_gen2_save->pokemon_storage.p_current_box,
            sizeof(struct pksav_gen2_pokemon_box)
        );
    }

    // Free the save and make sure all fields are set to NULL or default.
    error = pksav_gen2_free_save(p_gen2_save);
    PKSAV_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL(PKSAV_GEN2_SAVE_TYPE_NONE, p_gen2_save->save_type);

    TEST_ASSERT_NULL(p_gen2_save->save_time.p_time_played);
    TEST_ASSERT_NULL(p_gen2_save->save_time.p_daylight_savings);

    TEST_ASSERT_NULL(p_gen2_save->item_storage.p_item_bag);
    TEST_ASSERT_NULL(p_gen2_save->item_storage.p_item_pc);

    TEST_ASSERT_NULL(p_gen2_save->pokemon_storage.p_party);
    for(size_t box_index = 0; box_index < PKSAV_GEN2_NUM_POKEMON_BOXES; ++box_index)
    {
        TEST_ASSERT_NULL(p_gen2_save->pokemon_storage.pp_boxes[box_index]);
    }
    TEST_ASSERT_NULL(p_gen2_save->pokemon_storage.p_current_box_num);
    TEST_ASSERT_NULL(p_gen2_save->pokemon_storage.p_current_box);

    TEST_ASSERT_NULL(p_gen2_save->pokedex_lists.p_seen);
    TEST_ASSERT_NULL(p_gen2_save->pokedex_lists.p_owned);

    TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_id);
    TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_name);
    TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_money);
    TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_johto_badges);
    TEST_ASSERT_NULL(p_gen2_save->trainer_info.p_kanto_badges);

    TEST_ASSERT_NULL(p_gen2_save->misc_fields.p_rival_name);

    TEST_ASSERT_NULL(p_gen2_save->p_internal);
}

static void gen2_save_from_buffer_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen2_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gen2_save gen2_save = EMPTY_GEN2_SAVE;
    enum pksav_error error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves)
    {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        original_filepath, sizeof(original_filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    uint8_t* save_buffer = NULL;
    size_t save_size = 0;
    if(pksav_fs_read_file_to_buffer(original_filepath, &save_buffer, &save_size))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }
    TEST_ASSERT_TRUE(save_size >= PKSAV_GEN2_SAVE_SIZE);

    error = pksav_gen2_load_save_from_buffer(
                save_buffer,
                save_size,
                &gen2_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    // This test will free the save.
    gen2_save_test(
        &gen2_save,
        expected_save_type,
        original_filepath,
        save_name
    );

    free(save_buffer);
}

static void gen2_save_from_file_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen2_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gen2_save gen2_save = EMPTY_GEN2_SAVE;
    enum pksav_error error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves)
    {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        original_filepath, sizeof(original_filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    error = pksav_gen2_load_save_from_file(
                original_filepath,
                &gen2_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    // This test will free the save.
    gen2_save_test(
        &gen2_save,
        expected_save_type,
        original_filepath,
        save_name
    );
}

static void pksav_buffer_is_gold_save_test()
{
    pksav_gen2_get_buffer_save_type_test(
        "gold_silver",
        "pokemon_gold.sav",
        PKSAV_GEN2_SAVE_TYPE_GS
    );
}

static void pksav_file_is_gold_save_test()
{
    pksav_gen2_get_file_save_type_test(
        "gold_silver",
        "pokemon_gold.sav",
        PKSAV_GEN2_SAVE_TYPE_GS
    );
}

static void gold_save_from_buffer_test()
{
    gen2_save_from_buffer_test(
        "gold_silver",
        "pokemon_gold.sav",
        PKSAV_GEN2_SAVE_TYPE_GS
    );
}

static void gold_save_from_file_test()
{
    gen2_save_from_file_test(
        "gold_silver",
        "pokemon_gold.sav",
        PKSAV_GEN2_SAVE_TYPE_GS
    );
}

static void pksav_buffer_is_crystal_save_test()
{
    pksav_gen2_get_buffer_save_type_test(
        "crystal",
        "pokemon_crystal.sav",
        PKSAV_GEN2_SAVE_TYPE_CRYSTAL
    );
}

static void pksav_file_is_crystal_save_test()
{
    pksav_gen2_get_file_save_type_test(
        "crystal",
        "pokemon_crystal.sav",
        PKSAV_GEN2_SAVE_TYPE_CRYSTAL
    );
}

static void crystal_save_from_buffer_test()
{
    gen2_save_from_buffer_test(
        "crystal",
        "pokemon_crystal.sav",
        PKSAV_GEN2_SAVE_TYPE_CRYSTAL
    );
}

static void crystal_save_from_file_test()
{
    gen2_save_from_file_test(
        "crystal",
        "pokemon_crystal.sav",
        PKSAV_GEN2_SAVE_TYPE_CRYSTAL
    );
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen2_get_buffer_save_type_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_gold_save_test)
    PKSAV_TEST(pksav_file_is_gold_save_test)
    PKSAV_TEST(gold_save_from_buffer_test)
    PKSAV_TEST(gold_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_crystal_save_test)
    PKSAV_TEST(pksav_file_is_crystal_save_test)
    PKSAV_TEST(crystal_save_from_buffer_test)
    PKSAV_TEST(crystal_save_from_file_test)
)
