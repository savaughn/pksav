/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gen1/save.h>

#include <stdio.h>

// TODO: replace when size is moved to header
#define GEN1_SAVE_SIZE 0x8000

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_buffer_is_gen1_save_on_random_buffer_test()
{
    uint8_t buffer[GEN1_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        bool is_buffer_gen1_save = false;
        (void)pksav_buffer_is_gen1_save(
                  buffer,
                  sizeof(buffer),
                  &is_buffer_gen1_save
              );
    }
}

static void pksav_buffer_is_gen1_save_test(
    const char* subdir,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    static uint8_t save_buffer[GEN1_SAVE_SIZE];
    pksav_error_t error = PKSAV_ERROR_NONE;

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

    if(read_file_into_buffer(filepath, save_buffer, GEN1_SAVE_SIZE))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    bool is_buffer_gen1_save = false;
    error = pksav_buffer_is_gen1_save(
                save_buffer,
                GEN1_SAVE_SIZE,
                &is_buffer_gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_buffer_gen1_save);
}

static void pksav_file_is_gen1_save_test(
    const char* subdir,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    pksav_error_t error = PKSAV_ERROR_NONE;

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

    bool is_file_gen1_save = false;
    error = pksav_file_is_gen1_save(
                filepath,
                &is_file_gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gen1_save);
}

static void gen1_save_load_and_save_match_test(
    const char* subdir,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gen1_save_t gen1_save;
    pksav_error_t error = PKSAV_ERROR_NONE;

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
    snprintf(
        tmp_save_filepath, sizeof(tmp_save_filepath),
        "%s%spksav_%d_%s",
        get_tmp_dir(), FS_SEPARATOR, get_pid(), save_name
    );

    error = pksav_gen1_save_load(
                original_filepath,
                &gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    error = pksav_gen1_save_save(
                tmp_save_filepath,
                &gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    error = pksav_gen1_save_free(&gen1_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    bool files_differ = false;
    if(do_files_differ(original_filepath, tmp_save_filepath, &files_differ))
    {
        if(delete_file(tmp_save_filepath))
        {
            TEST_FAIL_MESSAGE("Comparing the two saves failed. Failed to clean up temp file.");
        }
        TEST_FAIL_MESSAGE("Comparing the two saves failed.");
    }

    if(delete_file(tmp_save_filepath)) {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
    TEST_ASSERT_FALSE(files_differ);
}

static void pksav_buffer_is_red_save_test()
{
    pksav_buffer_is_gen1_save_test("red_blue", "pokemon_red.sav");
}

static void pksav_file_is_red_save_test()
{
    pksav_file_is_gen1_save_test("red_blue", "pokemon_red.sav");
}

static void red_save_load_and_save_match_test()
{
    gen1_save_load_and_save_match_test("red_blue", "pokemon_red.sav");
}

static void pksav_buffer_is_yellow_save_test()
{
    pksav_buffer_is_gen1_save_test("yellow", "pokemon_yellow.sav");
}

static void pksav_file_is_yellow_save_test()
{
    pksav_file_is_gen1_save_test("yellow", "pokemon_yellow.sav");
}

static void yellow_save_load_and_save_match_test()
{
    gen1_save_load_and_save_match_test("yellow", "pokemon_yellow.sav");
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gen1_save_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_red_save_test)
    PKSAV_TEST(pksav_file_is_red_save_test)
    PKSAV_TEST(red_save_load_and_save_match_test)

    PKSAV_TEST(pksav_buffer_is_yellow_save_test)
    PKSAV_TEST(pksav_file_is_yellow_save_test)
    PKSAV_TEST(yellow_save_load_and_save_match_test)
)
