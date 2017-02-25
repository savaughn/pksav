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

    char* pksav_test_files = getenv("PKSAV_TEST_FILES");
    snprintf(
        filepath, sizeof(filepath),
        "%s%sred_blue%spokemon_red.sav",
        pksav_test_files, FS_SEPARATOR, FS_SEPARATOR
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

    char* pksav_test_files = getenv("PKSAV_TEST_FILES");
    snprintf(
        filepath, sizeof(filepath),
        "%s%sred_blue%spokemon_red.sav",
        pksav_test_files, FS_SEPARATOR, FS_SEPARATOR
    );

    bool is_file_gen1_save = false;
    error = pksav_file_is_gen1_save(
                filepath,
                &is_file_gen1_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gen1_save);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gen1_save_test)
    PKSAV_TEST(pksav_file_is_gen1_save_test)
)
