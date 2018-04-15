/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

#include <stdlib.h>

static void test_setting_gb_IV_from_random_raw_storage()
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
                sizeof(IVs),
                NULL
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

static void test_setting_gb_IV_from_random_IVs()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint16_t raw_IV = 0;
    uint8_t IVs[PKSAV_NUM_GB_IVS] = {0};

    // Start from random individual IVs. Write the IVs, read them back,
    // and make sure they match.
    for(enum pksav_gb_IV stat = PKSAV_GB_IV_ATTACK;
        stat <= PKSAV_GB_IV_HP;
        ++stat)
    {
        IVs[stat] = (uint8_t)(rand() % 16);

        error = pksav_set_gb_IV(
                    stat,
                    IVs[stat],
                    &raw_IV
                );
        PKSAV_TEST_ASSERT_SUCCESS(error);
    }

    uint8_t test_IVs[PKSAV_NUM_GB_IVS] = {0};
    error = pksav_get_gb_IVs(
                &raw_IV,
                test_IVs,
                sizeof(test_IVs),
                NULL
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_MEMORY(IVs, test_IVs, sizeof(IVs));
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(test_setting_gb_IV_from_random_raw_storage)
    PKSAV_TEST(test_setting_gb_IV_from_random_IVs)
)
