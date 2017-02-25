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

static void pksav_buffer_is_gen1_save_test() {
    static char filepath[256];
    static uint8_t save_buffer[GEN1_SAVE_SIZE];
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%sred_blue%spokemon_red.sav",
        pksav_test_saves, FS_SEPARATOR, FS_SEPARATOR
    );

    if(read_file_into_buffer(filepath, save_buffer, GEN1_SAVE_SIZE)) {
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

static void pksav_file_is_gen1_save_test() {
    static char filepath[256];
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        filepath, sizeof(filepath),
        "%s%sred_blue%spokemon_red.sav",
        pksav_test_saves, FS_SEPARATOR, FS_SEPARATOR
    );

    bool is_file_gen1_save = false;
    error = pksav_file_is_gen1_save(
                filepath,
                &is_file_gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gen1_save);
}

static void gen1_save_load_and_save_match_test() {
    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gen1_save_t gen1_save;
    pksav_error_t error = PKSAV_ERROR_NONE;

    char* pksav_test_saves = getenv("PKSAV_TEST_SAVES");
    if(!pksav_test_saves) {
        TEST_FAIL_MESSAGE("Failed to get test save directory.");
    }

    snprintf(
        original_filepath, sizeof(original_filepath),
        "%s%sred_blue%spokemon_red.sav",
        pksav_test_saves, FS_SEPARATOR, FS_SEPARATOR
    );
    snprintf(
        tmp_save_filepath, sizeof(tmp_save_filepath),
        "%s%spksav_red_%d.sav",
        get_tmp_dir(), FS_SEPARATOR, get_pid()
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

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gen1_save_test)
    PKSAV_TEST(pksav_file_is_gen1_save_test)
    PKSAV_TEST(gen1_save_load_and_save_match_test)
)
