/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gen4/save.h>

#include <stdio.h>
#include <string.h>

// TODO: replace when size is moved to header
#define GEN4_SAVE_SIZE 0x80000

static void pksav_buffer_is_gen4_save_test(
    const char* subdir,
    const char* save_name,
    pksav_gen4_game_t game
) {
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    static uint8_t save_buffer[GEN4_SAVE_SIZE];
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

    if(read_file_into_buffer(filepath, save_buffer, GEN4_SAVE_SIZE)) {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    bool is_buffer_gen4_save = false;
    error = pksav_buffer_is_gen4_save(
                save_buffer,
                GEN4_SAVE_SIZE,
                game,
                &is_buffer_gen4_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_buffer_gen4_save);
}

static void pksav_file_is_gen4_save_test(
    const char* subdir,
    const char* save_name,
    pksav_gen4_game_t game
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

    bool is_file_gen4_save = false;
    error = pksav_file_is_gen4_save(
                filepath,
                game,
                &is_file_gen4_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gen4_save);
}

static void gen4_save_load_and_save_match_test(
    const char* subdir,
    const char* save_name,
    pksav_gen4_game_t game
) {
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gen4_save_t gen4_save;
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

    error = pksav_gen4_save_load(
                original_filepath,
                &gen4_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(game, gen4_save.gen4_game);

    error = pksav_gen4_save_save(
                tmp_save_filepath,
                &gen4_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    pksav_gen4_save_t tmp_save;
    error = pksav_gen4_save_load(
                tmp_save_filepath,
                &tmp_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(gen4_save.gen4_game, tmp_save.gen4_game);

    /*TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.trainer_info, tmp_save.trainer_info, sizeof(*tmp_save.trainer_info))
    );
    if(tmp_save.gba_game == PKSAV_GBA_FRLG) {
        TEST_ASSERT_EQUAL(0,
            memcmp(gba_save.rival_name, tmp_save.rival_name, 7)
        );
    }
    TEST_ASSERT_EQUAL(*gba_save.money, *tmp_save.money);
    TEST_ASSERT_EQUAL(*gba_save.casino_coins, *tmp_save.casino_coins);
    TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.pokedex_owned, tmp_save.pokedex_owned, 49)
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.pokedex_seenA, tmp_save.pokedex_seenA, 49)
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.pokedex_seenB, tmp_save.pokedex_seenB, 49)
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.pokedex_seenC, tmp_save.pokedex_seenC, 49)
    );
    if(tmp_save.gba_game == PKSAV_GBA_FRLG) {
        TEST_ASSERT_EQUAL(*gba_save.frlg_nat_pokedex_unlockedA, *tmp_save.frlg_nat_pokedex_unlockedA);
    } else {
        TEST_ASSERT_EQUAL(*gba_save.rse_nat_pokedex_unlockedA, *tmp_save.rse_nat_pokedex_unlockedA);
    }
    TEST_ASSERT_EQUAL(*gba_save.nat_pokedex_unlockedB, *tmp_save.nat_pokedex_unlockedB);
    TEST_ASSERT_EQUAL(*gba_save.nat_pokedex_unlockedC, *tmp_save.nat_pokedex_unlockedC);*/

    pksav_gen4_save_free(&tmp_save);
    //TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    pksav_gen4_save_free(&gen4_save);
    //TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    if(delete_file(tmp_save_filepath)) {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
}

static void pksav_buffer_is_diamond_save_test() {
    pksav_buffer_is_gen4_save_test("diamond_pearl", "pokemon_diamond.sav", PKSAV_GEN4_DP);
}

static void pksav_file_is_diamond_save_test() {
    pksav_file_is_gen4_save_test("diamond_pearl", "pokemon_diamond.sav", PKSAV_GEN4_DP);
}

static void diamond_save_load_and_save_match_test() {
    gen4_save_load_and_save_match_test("diamond_pearl", "pokemon_diamond.sav", PKSAV_GEN4_DP);
}

static void pksav_buffer_is_platinum_save_test() {
    pksav_buffer_is_gen4_save_test("platinum", "pokemon_platinum.sav", PKSAV_GEN4_PLATINUM);
}

static void pksav_file_is_platinum_save_test() {
    pksav_file_is_gen4_save_test("platinum", "pokemon_platinum.sav", PKSAV_GEN4_PLATINUM);
}

static void platinum_save_load_and_save_match_test() {
    gen4_save_load_and_save_match_test("platinum", "pokemon_platinum.sav", PKSAV_GEN4_PLATINUM);
}

static void pksav_buffer_is_soulsilver_save_test() {
    pksav_buffer_is_gen4_save_test("heartgold_soulsilver", "pokemon_soulsilver.sav", PKSAV_GEN4_HGSS);
}

static void pksav_file_is_soulsilver_save_test() {
    pksav_file_is_gen4_save_test("heartgold_soulsilver", "pokemon_soulsilver.sav", PKSAV_GEN4_HGSS);
}

static void soulsilver_save_load_and_save_match_test() {
    gen4_save_load_and_save_match_test("heartgold_soulsilver", "pokemon_soulsilver.sav", PKSAV_GEN4_HGSS);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_diamond_save_test)
    PKSAV_TEST(pksav_file_is_diamond_save_test)
    PKSAV_TEST(diamond_save_load_and_save_match_test)
    PKSAV_TEST(pksav_buffer_is_platinum_save_test)
    PKSAV_TEST(pksav_file_is_platinum_save_test)
    PKSAV_TEST(platinum_save_load_and_save_match_test)
    PKSAV_TEST(pksav_buffer_is_soulsilver_save_test)
    PKSAV_TEST(pksav_file_is_soulsilver_save_test)
    PKSAV_TEST(soulsilver_save_load_and_save_match_test)
)
