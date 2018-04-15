/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

#include <stdlib.h>

// Game Boy IVs

static void gb_IV_invalid_params_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint16_t raw_IV = 0;

    enum pksav_gb_IV stat_below_minimum = PKSAV_GB_IV_ATTACK - 1;
    enum pksav_gb_IV stat_above_maximum = PKSAV_GB_IV_HP + 1;
    uint8_t IV_above_maximum = PKSAV_MAX_GB_IV + 1;

    error = pksav_set_gb_IV(
                stat_below_minimum,
                0,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);

    error = pksav_set_gb_IV(
                stat_above_maximum,
                0,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);

    error = pksav_set_gb_IV(
                PKSAV_GB_IV_ATTACK,
                IV_above_maximum,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);
}

static void gb_IV_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint16_t raw_IV = 0;
    uint8_t IVs[PKSAV_NUM_GB_IVS] = {0};

    // Start from a random raw IV storage. Extract the IVs, put them back,
    // and make sure it matches.
    while(raw_IV == 0)
    {
        raw_IV = (uint16_t)(rand() % 0xFFFF);
    }

    error = pksav_get_gb_IVs(
                &raw_IV,
                IVs,
                sizeof(IVs)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    uint16_t test_raw_IV = 0;
    for(enum pksav_gb_IV stat = PKSAV_GB_IV_ATTACK;
        stat <= PKSAV_GB_IV_HP;
        ++stat)
    {
        error = pksav_set_gb_IV(
                    stat,
                    IVs[stat],
                    &test_raw_IV
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);
    }

    TEST_ASSERT_EQUAL(raw_IV, test_raw_IV);
}

// IVs

static void IV_invalid_params_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint32_t raw_IV = 0;

    enum pksav_IV stat_below_minimum = PKSAV_IV_ATTACK - 1;
    enum pksav_IV stat_above_maximum = PKSAV_IV_HP + 1;
    uint8_t IV_above_maximum = PKSAV_MAX_IV + 1;

    error = pksav_set_IV(
                stat_below_minimum,
                0,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);

    error = pksav_set_IV(
                stat_above_maximum,
                0,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);

    error = pksav_set_IV(
                PKSAV_IV_ATTACK,
                IV_above_maximum,
                &raw_IV
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);
}

static void IV_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint32_t raw_IV = 0;
    uint8_t IVs[PKSAV_NUM_IVS] = {0};

    // Start from a random raw IV storage. Extract the IVs, put them back,
    // and make sure it matches.
    while(raw_IV == 0)
    {
        raw_IV = (uint32_t)rand();
    }

    // The upper two bits are used for non-IV storage, so remove them to
    // remove false errors.
    raw_IV &= ~(3 << 29);

    error = pksav_get_IVs(
                &raw_IV,
                IVs,
                sizeof(IVs)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    uint32_t test_raw_IV = 0;
    for(enum pksav_IV stat = PKSAV_IV_ATTACK;
        stat <= PKSAV_IV_HP;
        ++stat)
    {
        error = pksav_set_IV(
                    stat,
                    IVs[stat],
                    &test_raw_IV
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);
    }

    TEST_ASSERT_EQUAL(raw_IV, test_raw_IV);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(gb_IV_invalid_params_test)
    PKSAV_TEST(gb_IV_test)

    PKSAV_TEST(IV_invalid_params_test)
    PKSAV_TEST(IV_test)
)
