/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include "util/fs.h"

#include <pksav/config.h>
#include <pksav/gen1.h>

#include <stdio.h>
#include <string.h>

#define STRBUFFER_LEN (64)

static const struct pksav_gen1_save EMPTY_GEN1_SAVE =
{
    .save_type       = PKSAV_GEN1_SAVE_TYPE_NONE,
    .p_time_played = NULL,
    .p_options     = NULL,

    .item_storage =
    {
        .p_item_bag = NULL,
        .p_item_pc  = NULL
    },

    .pokemon_storage =
    {
        .p_party           = NULL,
        .pp_boxes          = {NULL},
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
        .p_id     = NULL,
        .p_name   = NULL,
        .p_money  = NULL,
        .p_badges = NULL
    },

    .misc_fields =
    {
        .p_rival_name         = NULL,
        .p_casino_coins       = NULL,
        .p_pikachu_friendship = NULL
    },

    .p_internal = NULL
};

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_gen1_get_buffer_save_type_on_random_buffer_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t buffer[PKSAV_GEN1_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        enum pksav_gen1_save_type save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
        error = pksav_gen1_get_buffer_save_type(
                    buffer,
                    sizeof(buffer),
                    &save_type
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);
    }
}

static void pksav_gen1_get_buffer_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen1_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GEN1_SAVE_TYPE_RED_BLUE);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GEN1_SAVE_TYPE_YELLOW);

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
    TEST_ASSERT_TRUE(save_size >= PKSAV_GEN1_SAVE_SIZE);

    enum pksav_gen1_save_type save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
    error = pksav_gen1_get_buffer_save_type(
                save_buffer,
                PKSAV_GEN1_SAVE_SIZE,
                &save_type
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
}

static void pksav_gen1_get_file_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen1_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GEN1_SAVE_TYPE_RED_BLUE);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GEN1_SAVE_TYPE_YELLOW);

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

    enum pksav_gen1_save_type save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
    error = pksav_gen1_get_file_save_type(
                filepath,
                &save_type
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
}

static void validate_gen1_string(
    const uint8_t* buffer,
    size_t buffer_len
)
{
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_TRUE(buffer_len > 0);

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pksav_error error = pksav_gen1_import_text(
                                 buffer,
                                 strbuffer,
                                 buffer_len
                             );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(strlen(strbuffer) > 0ULL);
}

static void validate_gen1_item_bag(
    const struct pksav_gen1_item_bag* p_item_bag
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);
    TEST_ASSERT_TRUE(p_item_bag->count <= PKSAV_GEN1_ITEM_BAG_SIZE);

    // Don't validate item contents. Generation I games are buggy enough
    // to make this unreliable.
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

static void validate_gen1_item_pc(
    const struct pksav_gen1_item_pc* p_item_pc
)
{
    TEST_ASSERT_NOT_NULL(p_item_pc);
    TEST_ASSERT_TRUE(p_item_pc->count <= PKSAV_GEN1_ITEM_PC_SIZE);

    // Don't validate item contents. Generation I games are buggy enough
    // to make this unreliable.
}

static void validate_gen1_pokemon_party(
    const struct pksav_gen1_pokemon_party* p_party
)
{
    TEST_ASSERT_NOT_NULL(p_party);
    TEST_ASSERT_TRUE(p_party->count <= PKSAV_GEN1_PARTY_NUM_POKEMON);

    if(p_party->count < PKSAV_GEN1_PARTY_NUM_POKEMON)
    {
        TEST_ASSERT_EQUAL(0xFF, p_party->species[p_party->count]);
    }

    for(size_t party_index = 0; party_index < p_party->count; ++party_index)
    {
        validate_gen1_string(
            p_party->otnames[party_index],
            10
        );
        validate_gen1_string(
            p_party->nicknames[party_index],
            PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
        );
    }
}

static void validate_gen1_pokemon_box(
    const struct pksav_gen1_pokemon_box* p_pokemon_box
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon_box);
    TEST_ASSERT_TRUE(p_pokemon_box->count <= PKSAV_GEN1_BOX_NUM_POKEMON);

    if(p_pokemon_box->count < PKSAV_GEN1_BOX_NUM_POKEMON)
    {
        TEST_ASSERT_EQUAL(0xFF, p_pokemon_box->species[p_pokemon_box->count]);

        for(size_t box_index = 0; box_index < p_pokemon_box->count; ++box_index)
        {
            validate_gen1_string(
                p_pokemon_box->otnames[box_index],
                10
            );
            validate_gen1_string(
                p_pokemon_box->nicknames[box_index],
                PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
            );
        }
    }
}

static void gen1_save_test(
    struct pksav_gen1_save* p_gen1_save,
    enum pksav_gen1_save_type expected_save_type,
    const char* original_filepath,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(p_gen1_save);
    TEST_ASSERT_NOT_NULL(save_name);

    char tmp_save_filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

    snprintf(
        tmp_save_filepath, sizeof(tmp_save_filepath),
        "%s%spksav_%d_%s",
        get_tmp_dir(), FS_SEPARATOR, get_pid(), save_name
    );

    // Validate fields. No pointers should be NULL (except the Pikachu friendship
    // pointer for Red/Blue), and some fields have a specific set of valid values.
    TEST_ASSERT_NOT_NULL(p_gen1_save->p_internal);

    TEST_ASSERT_EQUAL(expected_save_type, p_gen1_save->save_type);
    TEST_ASSERT_NOT_NULL(p_gen1_save->p_time_played);
    TEST_ASSERT_NOT_NULL(p_gen1_save->p_options);

    validate_gen1_item_bag(p_gen1_save->item_storage.p_item_bag);
    validate_gen1_item_pc(p_gen1_save->item_storage.p_item_pc);
    validate_gen1_pokemon_party(p_gen1_save->pokemon_storage.p_party);

    for(size_t box_index = 0; box_index < PKSAV_GEN1_NUM_POKEMON_BOXES; ++box_index)
    {
        validate_gen1_pokemon_box(
            p_gen1_save->pokemon_storage.pp_boxes[box_index]
        );
    }

    TEST_ASSERT_NOT_NULL(p_gen1_save->pokemon_storage.p_current_box_num);
    uint8_t current_box_num = (*p_gen1_save->pokemon_storage.p_current_box_num
                            & PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK);
    TEST_ASSERT_TRUE(current_box_num <= PKSAV_GEN1_NUM_POKEMON_BOXES);

    validate_gen1_pokemon_box(p_gen1_save->pokemon_storage.p_current_box);

    TEST_ASSERT_NOT_NULL(p_gen1_save->pokedex_lists.p_seen);
    TEST_ASSERT_NOT_NULL(p_gen1_save->pokedex_lists.p_owned);

    TEST_ASSERT_NOT_NULL(p_gen1_save->trainer_info.p_id);
    validate_gen1_string(
        p_gen1_save->trainer_info.p_name,
        7
    );
    validate_bcd(
        p_gen1_save->trainer_info.p_money,
        3,
        999999
    );
    TEST_ASSERT_NOT_NULL(p_gen1_save->trainer_info.p_badges);

    validate_gen1_string(
        p_gen1_save->misc_fields.p_rival_name,
        7
    );
    validate_bcd(
        p_gen1_save->misc_fields.p_casino_coins,
        2,
        9999
    );
    if(p_gen1_save->save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE)
    {
        TEST_ASSERT_NULL(p_gen1_save->misc_fields.p_pikachu_friendship);
    }
    else
    {
        TEST_ASSERT_NOT_NULL(p_gen1_save->misc_fields.p_pikachu_friendship);
    }

    // Make sure loading and saving are perfectly symmetrical.
    error = pksav_gen1_save_save(
                tmp_save_filepath,
                p_gen1_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    bool files_differ = false;
    if(do_files_differ(original_filepath, tmp_save_filepath, &files_differ))
    {
        if(delete_file(tmp_save_filepath))
        {
            TEST_FAIL_MESSAGE("Comparing the two saves failed. Failed to clean up temp file.");
        }
        TEST_FAIL_MESSAGE("Comparing the two saves failed.");
    }

    if(delete_file(tmp_save_filepath))
    {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
    TEST_ASSERT_FALSE(files_differ);

    // Make sure setting the current box works as expected.
    for(uint8_t box_index = 0; box_index < PKSAV_GEN1_NUM_POKEMON_BOXES; ++box_index)
    {
        error = pksav_gen1_pokemon_storage_set_current_box(
                    &p_gen1_save->pokemon_storage,
                    box_index
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        uint8_t current_box_num = *p_gen1_save->pokemon_storage.p_current_box_num;
        current_box_num &= PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;
        TEST_ASSERT_EQUAL(box_index, current_box_num);
        TEST_ASSERT_EQUAL_MEMORY(
            p_gen1_save->pokemon_storage.pp_boxes[box_index],
            p_gen1_save->pokemon_storage.p_current_box,
            sizeof(struct pksav_gen1_pokemon_box)
        );
    }

    // Free the save and make sure all fields are set to NULL or default.
    error = pksav_gen1_free_save(p_gen1_save);
    PKSAV_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL(PKSAV_GEN1_SAVE_TYPE_NONE, p_gen1_save->save_type);
    TEST_ASSERT_NULL(p_gen1_save->p_time_played);
    TEST_ASSERT_NULL(p_gen1_save->p_options);

    TEST_ASSERT_NULL(p_gen1_save->item_storage.p_item_bag);
    TEST_ASSERT_NULL(p_gen1_save->item_storage.p_item_pc);

    TEST_ASSERT_NULL(p_gen1_save->pokemon_storage.p_party);
    for(size_t box_index = 0; box_index < PKSAV_GEN1_NUM_POKEMON_BOXES; ++box_index)
    {
        TEST_ASSERT_NULL(p_gen1_save->pokemon_storage.pp_boxes[box_index]);
    }
    TEST_ASSERT_NULL(p_gen1_save->pokemon_storage.p_current_box_num);
    TEST_ASSERT_NULL(p_gen1_save->pokemon_storage.p_current_box);

    TEST_ASSERT_NULL(p_gen1_save->pokedex_lists.p_seen);
    TEST_ASSERT_NULL(p_gen1_save->pokedex_lists.p_owned);

    TEST_ASSERT_NULL(p_gen1_save->trainer_info.p_id);
    TEST_ASSERT_NULL(p_gen1_save->trainer_info.p_name);
    TEST_ASSERT_NULL(p_gen1_save->trainer_info.p_money);
    TEST_ASSERT_NULL(p_gen1_save->trainer_info.p_badges);

    TEST_ASSERT_NULL(p_gen1_save->misc_fields.p_rival_name);
    TEST_ASSERT_NULL(p_gen1_save->misc_fields.p_casino_coins);
    TEST_ASSERT_NULL(p_gen1_save->misc_fields.p_pikachu_friendship);

    TEST_ASSERT_NULL(p_gen1_save->p_internal);
}

static void gen1_save_from_buffer_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen1_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gen1_save gen1_save = EMPTY_GEN1_SAVE;
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
    TEST_ASSERT_TRUE(save_size >= PKSAV_GEN1_SAVE_SIZE);

    error = pksav_gen1_load_save_from_buffer(
                save_buffer,
                save_size,
                &gen1_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    // This test will free the save.
    gen1_save_test(
        &gen1_save,
        expected_save_type,
        original_filepath,
        save_name
    );

    free(save_buffer);
}

static void gen1_save_from_file_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gen1_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gen1_save gen1_save = EMPTY_GEN1_SAVE;
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

    error = pksav_gen1_load_save_from_file(
                original_filepath,
                &gen1_save
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    // This test will free the save.
    gen1_save_test(
        &gen1_save,
        expected_save_type,
        original_filepath,
        save_name
    );
}

static void pksav_buffer_is_red_save_test()
{
    pksav_gen1_get_buffer_save_type_test(
        "red_blue",
        "pokemon_red.sav",
        PKSAV_GEN1_SAVE_TYPE_RED_BLUE
    );
}

static void pksav_file_is_red_save_test()
{
    pksav_gen1_get_file_save_type_test(
        "red_blue",
        "pokemon_red.sav",
        PKSAV_GEN1_SAVE_TYPE_RED_BLUE
    );
}

static void red_save_from_buffer_test()
{
    gen1_save_from_buffer_test(
        "red_blue",
        "pokemon_red.sav",
        PKSAV_GEN1_SAVE_TYPE_RED_BLUE
    );
}

static void red_save_from_file_test()
{
    gen1_save_from_file_test(
        "red_blue",
        "pokemon_red.sav",
        PKSAV_GEN1_SAVE_TYPE_RED_BLUE
    );
}

static void pksav_buffer_is_yellow_save_test()
{
    pksav_gen1_get_buffer_save_type_test(
        "yellow",
        "pokemon_yellow.sav",
        PKSAV_GEN1_SAVE_TYPE_YELLOW
    );
}

static void pksav_file_is_yellow_save_test()
{
    pksav_gen1_get_file_save_type_test(
        "yellow",
        "pokemon_yellow.sav",
        PKSAV_GEN1_SAVE_TYPE_YELLOW
    );
}

static void yellow_save_from_buffer_test()
{
    gen1_save_from_buffer_test(
        "yellow",
        "pokemon_yellow.sav",
        PKSAV_GEN1_SAVE_TYPE_YELLOW
    );
}

static void yellow_save_from_file_test()
{
    gen1_save_from_file_test(
        "yellow",
        "pokemon_yellow.sav",
        PKSAV_GEN1_SAVE_TYPE_YELLOW
    );
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen1_get_buffer_save_type_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_red_save_test)
    PKSAV_TEST(pksav_file_is_red_save_test)
    PKSAV_TEST(red_save_from_buffer_test)
    PKSAV_TEST(red_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_yellow_save_test)
    PKSAV_TEST(pksav_file_is_yellow_save_test)
    PKSAV_TEST(yellow_save_from_buffer_test)
    PKSAV_TEST(yellow_save_from_file_test)
)
