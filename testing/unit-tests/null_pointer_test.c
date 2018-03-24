/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

/*
 * pksav/common/datetime.h
 */
static void pksav_common_datetime_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    time_t dummy_time_t = 0;
    pksav_date_t dummy_pksav_date_t;
    uint32_t dummy_uint32_t = 0;

    /*
     * pksav_time_t_to_date_t
     */

    status = pksav_time_t_to_date_t(
        NULL,
        &dummy_pksav_date_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_time_t_to_date_t(
        &dummy_time_t,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_time_t_to_date_t(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_time_to_uint32_t
     */

    status = pksav_time_t_to_uint32_t(
        NULL,
        &dummy_uint32_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_time_t_to_uint32_t(
        &dummy_time_t,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_time_t_to_uint32_t(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/common/pokedex.h
 */
static void pksav_common_pokedex_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    bool dummy_bool = false;

    /*
     * pksav_get_pokedex_bit
     */

    status = pksav_get_pokedex_bit(
        NULL,
        0,
        &dummy_bool
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_pokedex_bit(
        &dummy_uint8_t,
        0,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_pokedex_bit(
        NULL,
        0,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_set_pokedex_bit
     */

    status = pksav_set_pokedex_bit(
        NULL,
        0,
        false
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/common/pokerus.h
 */
static void pksav_common_pokerus_h_test()
{
    pksav_error_t status = PKSAV_ERROR_NONE;
    uint8_t dummy_uint8_t = 0;
    pksav_pokerus_strain_t dummy_strain = PKSAV_POKERUS_STRAIN_A;

    /*
     * pksav_pokerus_get_strain
     */

    status = pksav_pokerus_get_strain(
                 NULL, // pokerus_ptr
                 &dummy_strain
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_pokerus_get_strain(
                 &dummy_uint8_t,
                 NULL // strain_out
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_pokerus_set_strain
     */

    status = pksav_pokerus_set_strain(
                 NULL, // pokerus_ptr
                 dummy_strain
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_pokerus_get_duration
     */

    status = pksav_pokerus_get_duration(
                 NULL, // pokerus_ptr
                 &dummy_uint8_t
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_pokerus_get_duration(
                 &dummy_uint8_t,
                 NULL // duration_out
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_pokerus_set_duration
     */

    status = pksav_pokerus_set_duration(
                  NULL, // pokerus_ptr
                  dummy_uint8_t
             );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/common/prng.h
 *
 * TODO: arng, lcrng functions when gen4 update merged
 */
static void pksav_common_prng_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    pksav_mtrng_t dummy_pksav_mtrng_t;
    uint32_t dummy_uint32_t = 0;

    /*
     * pksav_mtrng_populate
     */

    status = pksav_mtrng_populate(
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_mtrng_next
     */

    status = pksav_mtrng_next(
        NULL,
        &dummy_uint32_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_mtrng_next(
        &dummy_pksav_mtrng_t,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_mtrng_next(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/common/stats.h
 */
static void pksav_common_stats_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    uint16_t dummy_uint16_t = 0;
    uint32_t dummy_uint32_t = 0;

    /*
     * pksav_get_gb_IV
     */

    status = pksav_get_gb_IV(
        NULL,
        PKSAV_STAT_NONE,
        &dummy_uint8_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_gb_IV(
        &dummy_uint16_t,
        PKSAV_STAT_NONE,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_gb_IV(
        NULL,
        PKSAV_STAT_NONE,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_set_gb_IV
     */

    status = pksav_set_gb_IV(
        NULL,
        PKSAV_STAT_NONE,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_get_IV
     */

    status = pksav_get_IV(
        NULL,
        PKSAV_STAT_NONE,
        &dummy_uint8_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_IV(
        &dummy_uint32_t,
        PKSAV_STAT_NONE,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_get_IV(
        NULL,
        PKSAV_STAT_NONE,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/gen1/save.h
 */
static void pksav_gen1_save_h_test() {
}

/*
 * pksav/gen1/text.h
 */
static void pksav_gen1_text_h_test()
{
}

/*
 * pksav/gen2/save.h
 */
static void pksav_gen2_save_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    bool dummy_bool = false;
    char dummy_char = 0;
    pksav_gen2_save_t dummy_pksav_gen2_save_t;
    dummy_pksav_gen2_save_t.raw = NULL;

    /*
     * pksav_buffer_is_gen2_save
     */

    status = pksav_buffer_is_gen2_save(
        NULL,
        0,
        false,
        &dummy_bool
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_buffer_is_gen2_save(
        &dummy_uint8_t,
        0,
        false,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_buffer_is_gen2_save(
        NULL,
        0,
        false,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_file_is_gen2_save
     */

    status = pksav_file_is_gen2_save(
        NULL,
        false,
        &dummy_bool
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_file_is_gen2_save(
        &dummy_char,
        false,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_file_is_gen2_save(
        NULL,
        false,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gen2_save_load
     */

    status = pksav_gen2_save_load(
        NULL,
        &dummy_pksav_gen2_save_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_save_load(
        &dummy_char,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_save_load(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gen2_save_save
     */

    status = pksav_gen2_save_save(
        NULL,
        &dummy_pksav_gen2_save_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_save_save(
        &dummy_char,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_save_save(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gen2_save_free
     */

    status = pksav_gen2_save_free(
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_save_free(
        &dummy_pksav_gen2_save_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/gen2/text.h
 */
static void pksav_gen2_text_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    char dummy_char = 0;

    /*
     * pksav_text_from_gen2
     */

    status = pksav_text_from_gen2(
        NULL,
        &dummy_char,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_from_gen2(
        &dummy_uint8_t,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_from_gen2(
        NULL,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_text_to_gen2
     */

    status = pksav_text_to_gen2(
        NULL,
        &dummy_uint8_t,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_to_gen2(
        &dummy_char,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_to_gen2(
        NULL,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/gen2/time.h
 */
static void pksav_gen2_time_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    time_t dummy_time_t = 0;
    uint16_t dummy_uint16_t = 0;

    /*
     * pksav_gen2_set_caught_data_time_field
     */

    status = pksav_gen2_set_caught_data_time_field(
        NULL,
        &dummy_uint16_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_set_caught_data_time_field(
        &dummy_time_t,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gen2_set_caught_data_time_field(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/gba/save.h
 */
static void pksav_gba_save_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    bool dummy_bool = false;
    pksav_gba_save_t dummy_pksav_gba_save_t;
    char dummy_char = 0;

    /*
     * pksav_buffer_is_gba_save
     */

    status = pksav_buffer_is_gba_save(
        NULL,
        0,
        PKSAV_GBA_RS,
        &dummy_bool
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_buffer_is_gba_save(
        &dummy_uint8_t,
        0,
        PKSAV_GBA_RS,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_buffer_is_gba_save(
        NULL,
        0,
        PKSAV_GBA_RS,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_file_is_gba_save
     */

    status = pksav_file_is_gba_save(
        NULL,
        PKSAV_GBA_RS,
        &dummy_bool
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_file_is_gba_save(
        &dummy_char,
        PKSAV_GBA_RS,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_file_is_gba_save(
        NULL,
        PKSAV_GBA_RS,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gba_save_load
     */

    status = pksav_gba_save_load(
        NULL,
        &dummy_pksav_gba_save_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gba_save_load(
        &dummy_char,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gba_save_load(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gba_save_save
     */

    status = pksav_gba_save_save(
        NULL,
        &dummy_pksav_gba_save_t
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gba_save_save(
        &dummy_char,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_gba_save_save(
        NULL,
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_gba_save_free
     */

    status = pksav_gba_save_free(
        NULL
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

/*
 * pksav/gba/text.h
 */
static void pksav_gba_text_h_test() {
    pksav_error_t status = PKSAV_ERROR_NONE;

    uint8_t dummy_uint8_t = 0;
    char dummy_char = 0;

    /*
     * pksav_text_from_gba
     */

    status = pksav_text_from_gba(
        NULL,
        &dummy_char,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_from_gba(
        &dummy_uint8_t,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_from_gba(
        NULL,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    /*
     * pksav_text_to_gba
     */

    status = pksav_text_to_gba(
        NULL,
        &dummy_uint8_t,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_to_gba(
        &dummy_char,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);

    status = pksav_text_to_gba(
        NULL,
        NULL,
        0
    );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NULL_POINTER, status);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_common_datetime_h_test)
    PKSAV_TEST(pksav_common_pokedex_h_test)
    PKSAV_TEST(pksav_common_pokerus_h_test)
    PKSAV_TEST(pksav_common_prng_h_test)
    PKSAV_TEST(pksav_common_stats_h_test)
    PKSAV_TEST(pksav_gen1_save_h_test)
    PKSAV_TEST(pksav_gen1_text_h_test)
    PKSAV_TEST(pksav_gen2_save_h_test)
    PKSAV_TEST(pksav_gen2_text_h_test)
    PKSAV_TEST(pksav_gen2_time_h_test)
    PKSAV_TEST(pksav_gba_save_h_test)
    PKSAV_TEST(pksav_gba_text_h_test)
)
