/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

#define BUFFER_LEN 256

// Ugly strings to test
static const char* strings[] =
{
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "abcdefghijklmnopqrstuvwxyz",
    "1234567890",
    "Nidoran♀",
    "Nidoran♂",
    "Flabébé",
    NULL
};

static void pksav_gen1_text_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;
    uint8_t gen1_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gen1_export_text(
                    strings[string_index],
                    gen1_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_gen1_import_text(
                    gen1_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }
}

static void pksav_gen2_text_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;
    uint8_t gen2_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gen2_export_text(
                    strings[string_index],
                    gen2_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_gen2_import_text(
                    gen2_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }
}

static void pksav_gba_text_test()
{
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gba_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gba_export_text(
                    strings[string_index],
                    gba_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_gba_import_text(
                    gba_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen1_text_test)
    PKSAV_TEST(pksav_gen2_text_test)
    PKSAV_TEST(pksav_gba_text_test)
)
