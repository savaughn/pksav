/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav.h>

static void pokerus_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint8_t pokerus = 0;

    enum pksav_pokerus_strain strain_out = PKSAV_POKERUS_STRAIN_A;
    uint8_t duration_out = 0;

    // Test invalid duration.
    error = pksav_pokerus_set_duration(&pokerus, 16);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_PARAM_OUT_OF_RANGE, error);

    // Test all valid strains.
    for(int i = 0; i < 4; ++i)
    {
        error = pksav_pokerus_set_strain(
                    &pokerus,
                    (enum pksav_pokerus_strain)i
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_pokerus_get_strain(
                    &pokerus,
                    &strain_out
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
        TEST_ASSERT_EQUAL((enum pksav_pokerus_strain)i, strain_out);
    }

    // Test all valid durations.
    for(uint8_t i = 0; i < 16; ++i)
    {
        error = pksav_pokerus_set_duration(
                    &pokerus,
                    i
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);

        error = pksav_pokerus_get_duration(
                    &pokerus,
                    &duration_out
                );
        TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(i, duration_out);
    }
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pokerus_test)
)
