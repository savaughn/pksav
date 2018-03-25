/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gen2/save.h>

#include <stdio.h>
#include <string.h>

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
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
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
    uint8_t save_buffer[PKSAV_GEN2_SAVE_SIZE];
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

    if(read_file_into_buffer(filepath, save_buffer, PKSAV_GEN2_SAVE_SIZE))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    enum pksav_gen2_save_type save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    error = pksav_gen2_get_buffer_save_type(
                save_buffer,
                PKSAV_GEN2_SAVE_SIZE,
                &save_type
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
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
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
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

// TODO: gold_save_test

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

// TODO: crystal_save_test

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen2_get_buffer_save_type_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_gold_save_test)
    PKSAV_TEST(pksav_file_is_gold_save_test)

    PKSAV_TEST(pksav_buffer_is_crystal_save_test)
    PKSAV_TEST(pksav_file_is_crystal_save_test)
)
