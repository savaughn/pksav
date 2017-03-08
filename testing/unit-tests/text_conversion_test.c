/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

#include <wchar.h>

#define BUFFER_LEN 256
#define TEST_ASSERT_EQUAL_WSTRING(wstr1,wstr2) TEST_ASSERT_EQUAL(0, wcscmp(wstr1,wstr2));

// Ugly strings to test
static const char* strings[] = {
    "Nidoran♀",
    "Nidoran♂",
    "Flabébé",
    NULL
};
static const wchar_t* wstrings[] = {
    L"Nidoran♀",
    L"Nidoran♂",
    L"Flabébé",
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

static void pksav_gen1_widetext_test() {
#ifdef PKSAV_ENABLE_GEN1
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gen1_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gen1(
                    wstrings[i],
                    gen1_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gen1(
                    gen1_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
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

static void pksav_gen2_widetext_test() {
#ifdef PKSAV_ENABLE_GEN2
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gen2_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gen2(
                    wstrings[i],
                    gen2_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gen2(
                    gen2_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
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

static void pksav_gba_widetext_test() {
#ifdef PKSAV_ENABLE_GBA
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint8_t gba_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gba(
                    wstrings[i],
                    gba_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gba(
                    gba_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
    }
#endif
}

static void pksav_gcn_text_test() {
#ifdef PKSAV_ENABLE_GCN
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint32_t gcn_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gcn(
                    strings[i],
                    gcn_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gcn(
                    gcn_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

static void pksav_gcn_widetext_test() {
#ifdef PKSAV_ENABLE_GCN
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint32_t gcn_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gcn(
                    wstrings[i],
                    gcn_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gcn(
                    gcn_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
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

static void pksav_gen4_widetext_test() {
#ifdef PKSAV_ENABLE_GEN4
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint16_t gen4_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gen4(
                    wstrings[i],
                    gen4_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gen4(
                    gen4_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
    }
#endif
}

static void pksav_gen5_text_test() {
#ifdef PKSAV_ENABLE_GEN5
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint16_t gen5_buffer[BUFFER_LEN] = {0};
    char strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; strings[i] != NULL; ++i) {
        error = pksav_text_to_gen5(
                    strings[i],
                    gen5_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_text_from_gen5(
                    gen5_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(strings[i], strbuffer);
    }
#endif
}

static void pksav_gen5_widetext_test() {
#ifdef PKSAV_ENABLE_GEN5
    pksav_error_t error = PKSAV_ERROR_NONE;
    uint16_t gen5_buffer[BUFFER_LEN] = {0};
    wchar_t strbuffer[BUFFER_LEN] = {0};

    for(size_t i = 0; wstrings[i] != NULL; ++i) {
        error = pksav_widetext_to_gen5(
                    wstrings[i],
                    gen5_buffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_widetext_from_gen5(
                    gen5_buffer,
                    strbuffer,
                    BUFFER_LEN
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_WSTRING(wstrings[i], strbuffer);
    }
#endif
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gen1_text_test)
    PKSAV_TEST(pksav_gen1_widetext_test)
    PKSAV_TEST(pksav_gen2_text_test)
    PKSAV_TEST(pksav_gen2_widetext_test)
    PKSAV_TEST(pksav_gba_text_test)
    PKSAV_TEST(pksav_gba_widetext_test)
    PKSAV_TEST(pksav_gcn_text_test)
    PKSAV_TEST(pksav_gcn_widetext_test)
    PKSAV_TEST(pksav_gen4_text_test)
    PKSAV_TEST(pksav_gen4_widetext_test)
    PKSAV_TEST(pksav_gen5_text_test)
    PKSAV_TEST(pksav_gen5_widetext_test)
)
