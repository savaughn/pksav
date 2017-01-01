/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

#define MAX_SAVE_SIZE 0x80000 // NDS save

static pksav_error_t error = PKSAV_ERROR_NONE;
static uint8_t buffer[MAX_SAVE_SIZE];

/*
 * Generation I
 */

static void pksav_gen1_save_detection_test(
    const char* filepath
) {
    bool is_valid = false;

    // Test with buffer
    FILE* sav_file = fopen(filepath, "r");
    TEST_ASSERT_NOT_NULL(sav_file);
    PKSAV_UNUSED(int fread_output) = fread(buffer, 1, sizeof(buffer), sav_file);
    fclose(sav_file);
    error = pksav_buffer_is_gen1_save(
                buffer,
                sizeof(buffer),
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);

    // Test with file
    error = pksav_file_is_gen1_save(
                filepath,
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);
}

static void pksav_red_save_detection_test() {
    pksav_gen1_save_detection_test("@POKEMON_RED_SAV@");
}

// TODO: Blue

static void pksav_yellow_save_detection_test() {
    pksav_gen1_save_detection_test("@POKEMON_YELLOW_SAV@");
}

/*
 * Generation II
 */

static void pksav_gen2_save_detection_test(
    const char* filepath,
    bool crystal
) {
    bool is_valid = false;

    // Test with buffer
    FILE* sav_file = fopen(filepath, "r");
    TEST_ASSERT_NOT_NULL(sav_file);
    PKSAV_UNUSED(int fread_output) = fread(buffer, 1, sizeof(buffer), sav_file);
    fclose(sav_file);
    error = pksav_buffer_is_gen2_save(
                buffer,
                sizeof(buffer),
                crystal,
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);

    // Test with file
    error = pksav_file_is_gen2_save(
                filepath,
                crystal,
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);
}

static void pksav_gold_save_detection_test() {
    pksav_gen2_save_detection_test(
        "@POKEMON_GOLD_SAV@",
        false
    );
}

// TODO: Silver

static void pksav_crystal_save_detection_test() {
    pksav_gen2_save_detection_test(
        "@POKEMON_CRYSTAL_SAV@",
        true
    );
}

/*
 * Game Boy Advance
 */

static void pksav_gba_save_detection_test(
    const char* filepath,
    pksav_gba_game_t gba_game
) {
    bool is_valid = false;

    // Test with buffer
    FILE* sav_file = fopen(filepath, "r");
    TEST_ASSERT_NOT_NULL(sav_file);
    PKSAV_UNUSED(int fread_output) = fread(buffer, 1, sizeof(buffer), sav_file);
    fclose(sav_file);
    error = pksav_buffer_is_gba_save(
                buffer,
                sizeof(buffer),
                gba_game,
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);

    // Test with file
    error = pksav_file_is_gba_save(
                filepath,
                gba_game,
                &is_valid
            );
    TEST_ASSERT_EQUAL(error, PKSAV_ERROR_NONE);
    TEST_ASSERT_TRUE(is_valid);
}

static void pksav_ruby_save_detection_test() {
    pksav_gba_save_detection_test(
        "@POKEMON_RUBY_SAV@",
        PKSAV_GBA_RS
    );
}

// TODO: Sapphire

static void pksav_emerald_save_detection_test() {
    pksav_gba_save_detection_test(
        "@POKEMON_EMERALD_SAV@",
        PKSAV_GBA_EMERALD
    );
}

static void pksav_firered_save_detection_test() {
    pksav_gba_save_detection_test(
        "@POKEMON_FIRERED_SAV@",
        PKSAV_GBA_FRLG
    );
}

// TODO: LeafGreen

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_red_save_detection_test)
    PKSAV_TEST(pksav_yellow_save_detection_test)
    PKSAV_TEST(pksav_gold_save_detection_test)
    PKSAV_TEST(pksav_crystal_save_detection_test)
    PKSAV_TEST(pksav_ruby_save_detection_test)
    PKSAV_TEST(pksav_emerald_save_detection_test)
    PKSAV_TEST(pksav_firered_save_detection_test)
)
