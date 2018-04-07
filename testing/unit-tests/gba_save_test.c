/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "test-utils.h"

#include "gba/save_internal.h"
#include "util/fs.h"

#include <pksav/config.h>
#include <pksav/gba.h>

#include <stdio.h>
#include <string.h>

/*
 * Testing TODO: make sure saving alternates between slots
 */

#define STRBUFFER_LEN 64

static const struct pksav_gba_save EMPTY_GBA_SAVE =
{
    .save_type = PKSAV_GBA_SAVE_TYPE_NONE,

    .time_played_ptr = NULL,

    .item_storage =
    {
        .bag_ptr = NULL,
        .pc_ptr  = NULL
    },

    .pokemon_storage =
    {
        .party_ptr = NULL,
        .pc_ptr    = NULL
    },

    .pokedex =
    {
        .seen_ptrA  = NULL,
        .seen_ptrB  = NULL,
        .seen_ptrC  = NULL,
        .owned_ptr  = NULL,
        .rse_nat_pokedex_unlocked_ptrA  = NULL,
        .frlg_nat_pokedex_unlocked_ptrA = NULL,
        .nat_pokedex_unlocked_ptrB      = NULL,
        .nat_pokedex_unlocked_ptrC      = NULL
    },

    .trainer_info =
    {
        .id_ptr     = NULL,
        .name_ptr   = NULL,
        .gender_ptr = NULL,
        .money_ptr  = NULL,
    },

    .misc_fields =
    {
        .rival_name_ptr   = NULL,
        .casino_coins_ptr = NULL
    },

    .internal_ptr = NULL
};

/*
 * We don't care about the result of the function itself. As the buffer
 * is randomized, it will likely be false. This function is to make sure
 * running it on invalid input doesn't crash.
 */
static void pksav_gba_get_buffer_save_type_on_random_buffer_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t buffer[PKSAV_GBA_SAVE_SIZE] = {0};
    for(size_t run_index = 0; run_index < 1000; ++run_index)
    {
        randomize_buffer(buffer, sizeof(buffer));

        enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
        error = pksav_gba_get_buffer_save_type(
                    buffer,
                    sizeof(buffer),
                    &save_type
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    }
}

static void pksav_gba_get_buffer_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);

    char filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

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

    size_t filesize = 0;
    if(pksav_fs_filesize(filepath, &filesize))
    {
        TEST_FAIL_MESSAGE("Failed to get save file size.");
    }
    TEST_ASSERT_TRUE(filesize >= PKSAV_GBA_SAVE_SIZE);

    uint8_t* save_buffer = NULL;
    if(pksav_fs_read_file_to_buffer(filepath, &save_buffer, &filesize))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }
    TEST_ASSERT_TRUE(filesize >= PKSAV_GBA_SAVE_SIZE);

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_buffer_save_type(
                save_buffer,
                filesize,
                &save_type
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);

    free(save_buffer);
}

static void pksav_gba_get_file_save_type_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);
    TEST_ASSERT_TRUE(expected_save_type >= PKSAV_GBA_SAVE_TYPE_RS);
    TEST_ASSERT_TRUE(expected_save_type <= PKSAV_GBA_SAVE_TYPE_FRLG);

    char filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

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

    enum pksav_gba_save_type save_type = PKSAV_GBA_SAVE_TYPE_NONE;
    error = pksav_gba_get_file_save_type(
                filepath,
                &save_type
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_save_type, save_type);
}

static void gba_save_test(
    struct pksav_gba_save* gba_save_ptr,
    enum pksav_gba_save_type expected_save_type,
    const char* original_filepath,
    const char* save_name
)
{
    TEST_ASSERT_NOT_NULL(gba_save_ptr);
    TEST_ASSERT_NOT_NULL(original_filepath);
    TEST_ASSERT_NOT_NULL(save_name);

    char tmp_save_filepath[256] = {0};
    enum pksav_error error = PKSAV_ERROR_NONE;

    snprintf(
        tmp_save_filepath, sizeof(tmp_save_filepath),
        "%s%spksav_%d_%s",
        get_tmp_dir(), FS_SEPARATOR, get_pid(), save_name
    );

    // Validate fields. Most pointers should not be NULL, and some fields have
    // a specific set of valid values.
    TEST_ASSERT_NOT_NULL(gba_save_ptr->internal_ptr);

    TEST_ASSERT_EQUAL(expected_save_type, gba_save_ptr->save_type);

    // Free the save and make sure all fields are set to NULL or default.
    error = pksav_gba_free_save(gba_save_ptr);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(PKSAV_GBA_SAVE_TYPE_NONE, gba_save_ptr->save_type);

    TEST_ASSERT_NULL(gba_save_ptr->time_played_ptr);

    TEST_ASSERT_NULL(gba_save_ptr->item_storage.bag_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->item_storage.pc_ptr);

    TEST_ASSERT_NULL(gba_save_ptr->pokemon_storage.party_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->pokemon_storage.pc_ptr);

    TEST_ASSERT_NULL(gba_save_ptr->pokedex.seen_ptrA);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.seen_ptrB);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.seen_ptrC);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.owned_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.rse_nat_pokedex_unlocked_ptrA);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.frlg_nat_pokedex_unlocked_ptrA);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.nat_pokedex_unlocked_ptrB);
    TEST_ASSERT_NULL(gba_save_ptr->pokedex.nat_pokedex_unlocked_ptrC);

    TEST_ASSERT_NULL(gba_save_ptr->trainer_info.id_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->trainer_info.name_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->trainer_info.money_ptr);

    TEST_ASSERT_NULL(gba_save_ptr->misc_fields.rival_name_ptr);
    TEST_ASSERT_NULL(gba_save_ptr->misc_fields.casino_coins_ptr);

    TEST_ASSERT_NULL(gba_save_ptr->internal_ptr);
}

static void gba_save_from_buffer_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gba_save gba_save = EMPTY_GBA_SAVE;
    enum pksav_error error = PKSAV_ERROR_NONE;

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

    size_t filesize = 0;
    if(pksav_fs_filesize(original_filepath, &filesize))
    {
        TEST_FAIL_MESSAGE("Failed to get save file size.");
    }
    TEST_ASSERT_TRUE(filesize >= PKSAV_GBA_SAVE_SIZE);

    uint8_t* save_buffer = NULL;
    if(pksav_fs_read_file_to_buffer(original_filepath, &save_buffer, &filesize))
    {
        TEST_FAIL_MESSAGE("Failed to read save into buffer.");
    }
    TEST_ASSERT_TRUE(filesize >= PKSAV_GBA_SAVE_SIZE);

    error = pksav_gba_load_save_from_buffer(
                save_buffer,
                filesize,
                &gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    // This test will free the save.
    gba_save_test(
        &gba_save,
        expected_save_type,
        original_filepath,
        save_name
    );

    free(save_buffer);
}

static void gba_save_from_file_test(
    const char* subdir,
    const char* save_name,
    enum pksav_gba_save_type expected_save_type
)
{
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(save_name);

    char original_filepath[256] = {0};
    struct pksav_gba_save gba_save = EMPTY_GBA_SAVE;
    enum pksav_error error = PKSAV_ERROR_NONE;

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

    error = pksav_gba_load_save_from_file(
                original_filepath,
                &gba_save
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

    // This test will free the save.
    gba_save_test(
        &gba_save,
        expected_save_type,
        original_filepath,
        save_name
    );
}

static void convenience_macro_test()
{
    struct pksav_gba_pc_pokemon pc_pokemon;
    memset(&pc_pokemon, 0, sizeof(pc_pokemon));

    struct pksav_gba_pokemon_blocks* blocks_ptr = &pc_pokemon.blocks;

    //uint16_t* origin_info_ptr = &blocks_ptr->misc.origin_info;
    uint32_t* ribbons_obedience_ptr = &blocks_ptr->misc.ribbons_obedience;

    *ribbons_obedience_ptr |= PKSAV_GBA_CONTEST_RIBBON_SUPER; // Cool
    // None for Beauty
    *ribbons_obedience_ptr |= (PKSAV_GBA_CONTEST_RIBBON_HYPER << PKSAV_GBA_CUTE_RIBBONS_OFFSET);
    *ribbons_obedience_ptr |= (PKSAV_GBA_CONTEST_RIBBON_NORMAL << PKSAV_GBA_SMART_RIBBONS_OFFSET);
    *ribbons_obedience_ptr |= (PKSAV_GBA_CONTEST_RIBBON_SUPER << PKSAV_GBA_TOUGH_RIBBONS_OFFSET);

    // Origin info (TODO: add enums, set, and test

    uint16_t ball = \
        PKSAV_GBA_POKEMON_BALL(blocks_ptr->misc.origin_info);
    uint16_t origin_game = \
        PKSAV_GBA_POKEMON_ORIGIN_GAME(blocks_ptr->misc.origin_info);

    (void)ball;
    (void)origin_game;

    // Ribbons

    uint32_t cool_contest_level   = PKSAV_GBA_COOL_CONTEST_LEVEL(*ribbons_obedience_ptr);
    uint32_t beauty_contest_level = PKSAV_GBA_BEAUTY_CONTEST_LEVEL(*ribbons_obedience_ptr);
    uint32_t cute_contest_level   = PKSAV_GBA_CUTE_CONTEST_LEVEL(*ribbons_obedience_ptr);
    uint32_t smart_contest_level  = PKSAV_GBA_SMART_CONTEST_LEVEL(*ribbons_obedience_ptr);
    uint32_t tough_contest_level  = PKSAV_GBA_TOUGH_CONTEST_LEVEL(*ribbons_obedience_ptr);

    TEST_ASSERT_EQUAL(PKSAV_GBA_CONTEST_RIBBON_SUPER,  cool_contest_level);
    TEST_ASSERT_EQUAL(PKSAV_GBA_CONTEST_RIBBON_NONE,   beauty_contest_level);
    TEST_ASSERT_EQUAL(PKSAV_GBA_CONTEST_RIBBON_HYPER,  cute_contest_level);
    TEST_ASSERT_EQUAL(PKSAV_GBA_CONTEST_RIBBON_NORMAL, smart_contest_level);
    TEST_ASSERT_EQUAL(PKSAV_GBA_CONTEST_RIBBON_SUPER,  tough_contest_level);
}

static void pksav_buffer_is_ruby_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

static void pksav_file_is_ruby_save_test()
{
    pksav_gba_get_file_save_type_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

static void ruby_save_from_buffer_test()
{
    gba_save_from_buffer_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

static void ruby_save_from_file_test()
{
    gba_save_from_file_test(
        "ruby_sapphire",
        "pokemon_ruby.sav",
        PKSAV_GBA_SAVE_TYPE_RS
    );
}

static void pksav_buffer_is_emerald_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

static void pksav_file_is_emerald_save_test()
{
    pksav_gba_get_file_save_type_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

static void emerald_save_from_buffer_test()
{
    gba_save_from_buffer_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

static void emerald_save_from_file_test()
{
    gba_save_from_file_test(
        "emerald",
        "pokemon_emerald.sav",
        PKSAV_GBA_SAVE_TYPE_EMERALD
    );
}

static void pksav_buffer_is_firered_save_test()
{
    pksav_gba_get_buffer_save_type_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

static void pksav_file_is_firered_save_test()
{
    pksav_gba_get_file_save_type_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

static void firered_save_from_buffer_test()
{
    gba_save_from_buffer_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

static void firered_save_from_file_test()
{
    gba_save_from_file_test(
        "firered_leafgreen",
        "pokemon_firered.sav",
        PKSAV_GBA_SAVE_TYPE_FRLG
    );
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_gba_get_buffer_save_type_on_random_buffer_test)

    PKSAV_TEST(convenience_macro_test)

    PKSAV_TEST(pksav_buffer_is_ruby_save_test)
    PKSAV_TEST(pksav_file_is_ruby_save_test)
    PKSAV_TEST(ruby_save_from_buffer_test)
    PKSAV_TEST(ruby_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_emerald_save_test)
    PKSAV_TEST(pksav_file_is_emerald_save_test)
    PKSAV_TEST(emerald_save_from_buffer_test)
    PKSAV_TEST(emerald_save_from_file_test)

    PKSAV_TEST(pksav_buffer_is_firered_save_test)
    PKSAV_TEST(pksav_file_is_firered_save_test)
    PKSAV_TEST(firered_save_from_buffer_test)
    PKSAV_TEST(firered_save_from_file_test)
)
