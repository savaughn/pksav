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
#include <string.h>

// TODO: replace when size is moved to header
#define GEN2_SAVE_SIZE 0x8000

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_buffer_is_gen2_save_on_random_buffer_test()
{
    uint8_t buffer[GEN2_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        bool is_buffer_gen2_save = false;
        (void)pksav_buffer_is_gen2_save(
                  buffer,
                  sizeof(buffer),
                  false, // crystal
                  &is_buffer_gen2_save
              );
        (void)pksav_buffer_is_gen2_save(
                  buffer,
                  sizeof(buffer),
                  true, // crystal
                  &is_buffer_gen2_save
              );
    }
}

static void pksav_buffer_is_gen2_save_test(
    const char* subdir,
    const char* save_name,
    bool crystal
)
{
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

    if(read_file_into_buffer(filepath, save_buffer, GEN2_SAVE_SIZE))
    {
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
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    static char original_filepath[256];
    static char tmp_save_filepath[256];
    pksav_gen2_save_t gen2_save;
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

    /*
     * Files may differ if one of the checksums was not set, which happen for some reason. So
     * just see if we can load the saved file and compare the saves by field.
     */
    pksav_gen2_save_t tmp_save;
    error = pksav_gen2_save_load(
                tmp_save_filepath,
                &tmp_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(gen2_save.gen2_game, tmp_save.gen2_game);

    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.pokemon_party, tmp_save.pokemon_party, sizeof(*gen2_save.pokemon_party))
    );
    TEST_ASSERT_EQUAL(*gen2_save.current_pokemon_box_num, *tmp_save.current_pokemon_box_num);
    for(int i = 0; i < 14; ++i)
    {
        TEST_ASSERT_EQUAL(0,
            memcmp(gen2_save.pokemon_boxes[i], tmp_save.pokemon_boxes[i], sizeof(*gen2_save.pokemon_boxes[i]))
        );
    }
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.pokemon_box_names, tmp_save.pokemon_box_names, sizeof(*gen2_save.pokemon_box_names))
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.item_bag, tmp_save.item_bag, sizeof(*gen2_save.item_bag))
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.item_pc, tmp_save.item_pc, sizeof(*gen2_save.item_pc))
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.trainer_name, tmp_save.trainer_name, 7)
    );
    TEST_ASSERT_EQUAL(*gen2_save.trainer_id, *tmp_save.trainer_id);
    if(crystal) {
        TEST_ASSERT_EQUAL(*gen2_save.trainer_gender, *tmp_save.trainer_gender);
    }
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.money, tmp_save.money, 3)
    );
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.rival_name, tmp_save.rival_name, 7)
    );
    TEST_ASSERT_EQUAL(*gen2_save.daylight_savings, *tmp_save.daylight_savings);
    TEST_ASSERT_EQUAL(0,
        memcmp(gen2_save.time_played, tmp_save.time_played, sizeof(*gen2_save.time_played))
    );

    error = pksav_gen2_save_free(&tmp_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    error = pksav_gen2_save_free(&gen2_save);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    if(delete_file(tmp_save_filepath))
    {
        TEST_FAIL_MESSAGE("Failed to clean up temp file.");
    }
}

static void pksav_buffer_is_gold_save_test()
{
    pksav_buffer_is_gen2_save_test("gold_silver", "pokemon_gold.sav", false);
}

static void pksav_file_is_gold_save_test()
{
    pksav_file_is_gen2_save_test("gold_silver", "pokemon_gold.sav", false);
}

static void gold_save_load_and_save_match_test()
{
    gen2_save_load_and_save_match_test("gold_silver", "pokemon_gold.sav", false);
}

static void pksav_buffer_is_crystal_save_test()
{
    pksav_buffer_is_gen2_save_test("crystal", "pokemon_crystal.sav", true);
}

static void pksav_file_is_crystal_save_test()
{
    pksav_file_is_gen2_save_test("crystal", "pokemon_crystal.sav", true);
}

static void crystal_save_load_and_save_match_test()
{
    gen2_save_load_and_save_match_test("crystal", "pokemon_crystal.sav", true);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_buffer_is_gen2_save_on_random_buffer_test)

    PKSAV_TEST(pksav_buffer_is_gold_save_test)
    PKSAV_TEST(pksav_file_is_gold_save_test)
    PKSAV_TEST(gold_save_load_and_save_match_test)

    PKSAV_TEST(pksav_buffer_is_crystal_save_test)
    PKSAV_TEST(pksav_file_is_crystal_save_test)
    PKSAV_TEST(crystal_save_load_and_save_match_test)
)
