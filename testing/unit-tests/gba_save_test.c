/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include <pksav/config.h>
#include <pksav/gba/save.h>

#include <stdio.h>
#include <string.h>

// TODO: replace when size is moved to header
#define GBA_SAVE_SLOT_SIZE 0x10000
#define GBA_SAVE_SIZE      0x20000

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_buffer_is_gba_save_on_random_buffer_test()
{
    uint8_t slot_buffer[GBA_SAVE_SLOT_SIZE] = {0};
    uint8_t save_buffer[GBA_SAVE_SIZE] = {0};

    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(slot_buffer, sizeof(slot_buffer));
        randomize_buffer(save_buffer, sizeof(save_buffer));

        bool is_buffer_gba_save = false;
        for(pksav_gba_game_t gba_game = PKSAV_GBA_RS;
            gba_game <= PKSAV_GBA_FRLG;
            ++gba_game)
        {
            // Test on a save with only one slot.
            (void)pksav_buffer_is_gba_save(
                      slot_buffer,
                      sizeof(slot_buffer),
                      gba_game,
                      &is_buffer_gba_save
                  );

            // Test on a save with two slots.
            (void)pksav_buffer_is_gba_save(
                      save_buffer,
                      sizeof(save_buffer),
                      gba_game,
                      &is_buffer_gba_save
                  );
        }
    }
}

static void pksav_buffer_is_gba_save_test(
    const char* subdir,
    const char* save_name,
    pksav_gba_game_t game
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char filepath[256];
    static uint8_t save_buffer[GBA_SAVE_SIZE];
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

    if(read_file_into_buffer(filepath, save_buffer, GBA_SAVE_SIZE))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }

    bool is_buffer_gba_save = false;
    error = pksav_buffer_is_gba_save(
                save_buffer,
                GBA_SAVE_SIZE,
                game,
                &is_buffer_gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_buffer_gba_save);
}

static void pksav_file_is_gba_save_test(
    const char* subdir,
    const char* save_name,
    pksav_gba_game_t game
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

    bool is_file_gba_save = false;
    error = pksav_file_is_gba_save(
                filepath,
                game,
                &is_file_gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_TRUE(is_file_gba_save);
}

static void gba_save_load_and_save_match_test(
    const char* subdir,
    const char* save_name,
    pksav_gba_game_t game
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gba_save_t gba_save;
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

    error = pksav_gba_save_load(
                original_filepath,
                &gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(game, gba_save.gba_game);

    error = pksav_gba_save_save(
                tmp_save_filepath,
                &gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    pksav_gba_save_t tmp_save;
    error = pksav_gba_save_load(
                tmp_save_filepath,
                &tmp_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(gba_save.gba_game, tmp_save.gba_game);

    TEST_ASSERT_EQUAL(0,
        memcmp(gba_save.trainer_info, tmp_save.trainer_info, sizeof(*tmp_save.trainer_info))
    );
    if(tmp_save.gba_game == PKSAV_GBA_FRLG)
    {
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
    if(tmp_save.gba_game == PKSAV_GBA_FRLG)
    {
        TEST_ASSERT_EQUAL(*gba_save.frlg_nat_pokedex_unlockedA, *tmp_save.frlg_nat_pokedex_unlockedA);
    }
    else
    {
        TEST_ASSERT_EQUAL(*gba_save.rse_nat_pokedex_unlockedA, *tmp_save.rse_nat_pokedex_unlockedA);
    }
    TEST_ASSERT_EQUAL(*gba_save.nat_pokedex_unlockedB, *tmp_save.nat_pokedex_unlockedB);
    TEST_ASSERT_EQUAL(*gba_save.nat_pokedex_unlockedC, *tmp_save.nat_pokedex_unlockedC);

    error = pksav_gba_save_free(&tmp_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    error = pksav_gba_save_free(&gba_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    if(delete_file(tmp_save_filepath))
    {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
}

static void pksav_buffer_is_ruby_save_test()
{
    pksav_buffer_is_gba_save_test("ruby_sapphire", "pokemon_ruby.sav", PKSAV_GBA_RS);
}

static void pksav_file_is_ruby_save_test()
{
    pksav_file_is_gba_save_test("ruby_sapphire", "pokemon_ruby.sav", PKSAV_GBA_RS);
}

static void ruby_save_load_and_save_match_test()
{
    gba_save_load_and_save_match_test("ruby_sapphire", "pokemon_ruby.sav", PKSAV_GBA_RS);
}

static void pksav_buffer_is_emerald_save_test()
{
    pksav_buffer_is_gba_save_test("emerald", "pokemon_emerald.sav", PKSAV_GBA_EMERALD);
}

static void pksav_file_is_emerald_save_test()
{
    pksav_file_is_gba_save_test("emerald", "pokemon_emerald.sav", PKSAV_GBA_EMERALD);
}

static void emerald_save_load_and_save_match_test()
{
    gba_save_load_and_save_match_test("emerald", "pokemon_emerald.sav", PKSAV_GBA_EMERALD);
}

static void pksav_buffer_is_firered_save_test()
{
    pksav_buffer_is_gba_save_test("firered_leafgreen", "pokemon_firered.sav", PKSAV_GBA_FRLG);
}

static void pksav_file_is_firered_save_test()
{
    pksav_file_is_gba_save_test("firered_leafgreen", "pokemon_firered.sav", PKSAV_GBA_FRLG);
}

static void firered_save_load_and_save_match_test()
{
    gba_save_load_and_save_match_test("firered_leafgreen", "pokemon_firered.sav", PKSAV_GBA_FRLG);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gba_save_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_ruby_save_test)
    PKSAV_TEST(pksav_file_is_ruby_save_test)
    PKSAV_TEST(ruby_save_load_and_save_match_test)

    PKSAV_TEST(pksav_buffer_is_emerald_save_test)
    PKSAV_TEST(pksav_file_is_emerald_save_test)
    PKSAV_TEST(emerald_save_load_and_save_match_test)

    PKSAV_TEST(pksav_buffer_is_firered_save_test)
    PKSAV_TEST(pksav_file_is_firered_save_test)
    PKSAV_TEST(firered_save_load_and_save_match_test)
)
