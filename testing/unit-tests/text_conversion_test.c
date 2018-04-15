/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav.h>

#define BUFFER_LEN 256

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
        PKSAV_TEST_ASSERT_SUCCESS(error);

        error = pksav_gen1_import_text(
                    gen1_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }

    // Make sure processing invalid buffers doesn't crash.
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(gen1_buffer, sizeof(gen1_buffer));
        pksav_gen1_import_text(
            gen1_buffer,
            strbuffer,
            BUFFER_LEN
        );

        randomize_buffer((uint8_t*)strbuffer, sizeof(strbuffer));
        pksav_gen1_export_text(
            strbuffer,
            gen1_buffer,
            BUFFER_LEN
        );
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
        PKSAV_TEST_ASSERT_SUCCESS(error);

        error = pksav_gen2_import_text(
                    gen2_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }

    // Make sure processing invalid buffers doesn't crash.
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(gen2_buffer, sizeof(gen2_buffer));
        pksav_gen2_import_text(
            gen2_buffer,
            strbuffer,
            BUFFER_LEN
        );

        randomize_buffer((uint8_t*)strbuffer, sizeof(strbuffer));
        pksav_gen2_export_text(
            strbuffer,
            gen2_buffer,
            BUFFER_LEN
        );
    }
}

static void pksav_gba_text_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;
    uint8_t gba_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gba_export_text(
                    strings[string_index],
                    gba_buffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        error = pksav_gba_import_text(
                    gba_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }

    // Make sure processing invalid buffers doesn't crash.
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(gba_buffer, sizeof(gba_buffer));
        pksav_gba_import_text(
            gba_buffer,
            strbuffer,
            BUFFER_LEN
        );

        randomize_buffer((uint8_t*)strbuffer, sizeof(strbuffer));
        pksav_gba_export_text(
            strbuffer,
            gba_buffer,
            BUFFER_LEN
        );
    }
}

static void pksav_gen4_text_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;
    uint16_t gen4_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gen4_export_text(
                    strings[string_index],
                    gen4_buffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        error = pksav_gen4_import_text(
                    gen4_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }

    // Make sure processing invalid buffers doesn't crash.
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer((uint8_t*)gen4_buffer, sizeof(gen4_buffer));
        pksav_gen4_import_text(
            gen4_buffer,
            strbuffer,
            BUFFER_LEN
        );

        randomize_buffer((uint8_t*)strbuffer, sizeof(strbuffer));
        pksav_gen4_export_text(
            strbuffer,
            gen4_buffer,
            BUFFER_LEN
        );
    }
}

static void pksav_gen5_text_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;
    uint16_t gen5_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t string_index = 0; strings[string_index] != NULL; ++string_index)
    {
        error = pksav_gen5_export_text(
                    strings[string_index],
                    gen5_buffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        error = pksav_gen5_import_text(
                    gen5_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(strings[string_index], strbuffer);
    }

    // Make sure processing invalid buffers doesn't crash.
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer((uint8_t*)gen5_buffer, sizeof(gen5_buffer));
        pksav_gen5_import_text(
            gen5_buffer,
            strbuffer,
            BUFFER_LEN
        );

        randomize_buffer((uint8_t*)strbuffer, sizeof(strbuffer));
        pksav_gen5_export_text(
            strbuffer,
            gen5_buffer,
            BUFFER_LEN
        );
    }
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen1_text_test)
    PKSAV_TEST(pksav_gen2_text_test)
    PKSAV_TEST(pksav_gba_text_test)
    PKSAV_TEST(pksav_gen4_text_test)
    PKSAV_TEST(pksav_gen5_text_test)
)
