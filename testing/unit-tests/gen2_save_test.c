/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gen2/save.h>

#include <stdio.h>

// TODO: replace when size is moved to header
#define GEN2_SAVE_SIZE 0x8000

static void pksav_buffer_is_gen2_save_test(
    const char* subdir,
    const char* save_name,
    bool crystal
) {
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    static uint8_t save_buffer[GEN2_SAVE_SIZE];
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    if(read_file_into_buffer(filepath, save_buffer, GEN2_SAVE_SIZE)) {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    bool is_buffer_gen2_save = false;
    error = pksav_buffer_is_gen2_save(
                save_buffer,
                GEN2_SAVE_SIZE,
                crystal,
                &is_buffer_gen2_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_buffer_gen2_save);
}

static void pksav_file_is_gen2_save_test(
    const char* subdir,
    const char* save_name,
    bool crystal
) {
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%s%s%s%s",
        pksav_test_saves, FS_SEPARATOR, subdir, FS_SEPARATOR, save_name
    );

    bool is_file_gen2_save = false;
    error = pksav_file_is_gen2_save(
                filepath,
                crystal,
                &is_file_gen2_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gen2_save);
}

static void gen2_save_load_and_save_match_test(
    const char* subdir,
    const char* save_name,
    bool crystal
) {
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gen2_save_t gen2_save;
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
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

    error = pksav_gen2_save_load(
                original_filepath,
                &gen2_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL((crystal ? PKSAV_GEN2_CRYSTAL : PKSAV_GEN2_GS), gen2_save.gen2_game);

    error = pksav_gen2_save_save(
                tmp_save_filepath,
                &gen2_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    error = pksav_gen2_save_free(&gen2_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    bool files_differ = false;
    if(do_files_differ(original_filepath, tmp_save_filepath, &files_differ)) {
        if(delete_file(tmp_save_filepath)) {
            TEST_FAIL_MESSAGE("Comparing the two saves failed. Failed to clean up temp file.");
        }
        TEST_FAIL_MESSAGE("Comparing the two saves failed.");
    }

    if(delete_file(tmp_save_filepath)) {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
    TEST_ASSERT_FALSE(files_differ);
}

static void pksav_buffer_is_gold_save_test() {
    pksav_buffer_is_gen2_save_test("gold_silver", "pokemon_gold.sav", false);
}

static void pksav_file_is_gold_save_test() {
    pksav_file_is_gen2_save_test("gold_silver", "pokemon_gold.sav", false);
}

static void gold_save_load_and_save_match_test() {
    gen2_save_load_and_save_match_test("gold_silver", "pokemon_gold.sav", false);
}

static void pksav_buffer_is_crystal_save_test() {
    pksav_buffer_is_gen2_save_test("crystal", "pokemon_crystal.sav", true);
}

static void pksav_file_is_crystal_save_test() {
    pksav_file_is_gen2_save_test("crystal", "pokemon_crystal.sav", true);
}

static void crystal_save_load_and_save_match_test() {
    gen2_save_load_and_save_match_test("crystal", "pokemon_crystal.sav", true);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gold_save_test)
    PKSAV_TEST(pksav_file_is_gold_save_test)
    PKSAV_TEST(gold_save_load_and_save_match_test)

    PKSAV_TEST(pksav_buffer_is_crystal_save_test)
    PKSAV_TEST(pksav_file_is_crystal_save_test)
    PKSAV_TEST(crystal_save_load_and_save_match_test)
)
