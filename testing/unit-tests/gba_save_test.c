/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../../lib/gba/save_internal.h"

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gba/save.h>

#include <stdio.h>
#include <string.h>

#define STRBUFFER_LEN 64
/*
static const struct pksav_gen2_save EMPTY_GEN2_SAVE =
{
    .save_type = PKSAV_GEN2_SAVE_TYPE_NONE,

    .save_time =
    {
        .time_played_ptr      = NULL,
        .daylight_savings_ptr = NULL
    },

    .item_storage =
    {
        .item_bag_ptr = NULL,
        .item_pc_ptr  = NULL
    },

    .pokemon_storage =
    {
        .party_ptr           = NULL,
        .box_ptrs            = {NULL},
        .current_box_num_ptr = NULL,
        .current_box_ptr     = NULL
    },

    .pokedex_lists =
    {
        .seen_ptr  = NULL,
        .owned_ptr = NULL
    },

    .trainer_info =
    {
        .id_ptr           = NULL,
        .name_ptr         = NULL,
        .gender_ptr       = NULL,
        .palette_ptr      = NULL,
        .money_ptr        = NULL,
        .johto_badges_ptr = NULL,
        .kanto_badges_ptr = NULL
    },

    .misc_fields =
    {
        .rival_name_ptr = NULL
    },

    .internal_ptr = NULL
};*/

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_gba_get_buffer_save_type_on_random_buffer_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t buffer[PKSAV_GBA_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
        error = pksav_gba_get_buffer_save_type(
                    buffer,
                    sizeof(buffer),
                    &save_type
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    }
}

static void pksav_gba_get_buffer_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);

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

    size_t filesize;
    if(get_filesize(filepath, &filesize))
    {
        TEST_FAIL_MESSAGE("Failed to get save file size.");
    }
    TEST_ASSERT_TRUE(filesize >= PKSAV_GBA_SAVE_SIZE);

    uint8_t* save_buffer = calloc(filesize, 1);
    if(read_file_into_buffer(filepath, save_buffer, filesize))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_buffer_save_type(
                save_buffer,
                filesize,
                &save_type
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);

    free(save_buffer);
}

static void pksav_gba_get_file_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);

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

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_file_save_type(
                filepath,
                &save_type
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
}

static void pksav_buffer_is_ruby_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

static void pksav_file_is_ruby_save_test()
{
    pksav_gba_get_file_save_type_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

/*static void gold_save_from_buffer_test()
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
}*/

static void pksav_buffer_is_emerald_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

static void pksav_file_is_emerald_save_test()
{
    pksav_gba_get_file_save_type_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

/*static void crystal_save_from_buffer_test()
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
}*/

static void pksav_buffer_is_firered_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

static void pksav_file_is_firered_save_test()
{
    pksav_gba_get_file_save_type_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

/*static void crystal_save_from_buffer_test()
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
}*/

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gba_get_buffer_save_type_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_ruby_save_test)
    PKSAV_TEST(pksav_file_is_ruby_save_test)
    //PKSAV_TEST(ruby_save_from_buffer_test)
    //PKSAV_TEST(ruby_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_emerald_save_test)
    PKSAV_TEST(pksav_file_is_emerald_save_test)
    //PKSAV_TEST(emerald_save_from_buffer_test)
    //PKSAV_TEST(emerald_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_firered_save_test)
    PKSAV_TEST(pksav_file_is_firered_save_test)
    //PKSAV_TEST(firered_save_from_buffer_test)
    //PKSAV_TEST(firered_save_from_file_test)
)
