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
static const char* strings[] = {
    "Nidoran♀",
    "Nidoran♂",
    "Flabébé",
    NULL
};

static void pksav_gen1_text_test() {
#ifdef PKSAV_ENABLE_GEN1
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gen1_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gen1(
                    strings[i],
                    gen1_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gen1(
                    gen1_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

static void pksav_gen2_text_test() {
#ifdef PKSAV_ENABLE_GEN2
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gen2_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gen2(
                    strings[i],
                    gen2_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gen2(
                    gen2_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

static void pksav_gba_text_test() {
#ifdef PKSAV_ENABLE_GBA
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gba_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gba(
                    strings[i],
                    gba_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gba(
                    gba_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

static void pksav_gen4_text_test() {
#ifdef PKSAV_ENABLE_GEN4
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint16_t gen4_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gen4(
                    strings[i],
                    gen4_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gen4(
                    gen4_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen1_text_test)
    PKSAV_TEST(pksav_gen2_text_test)
    PKSAV_TEST(pksav_gba_text_test)
    PKSAV_TEST(pksav_gen4_text_test)
)
