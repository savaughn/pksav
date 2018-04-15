/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/math/base256.h>
#include <pksav/math/bcd.h>

#include <string.h>

static void base256_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    const size_t test_num = 1338917;
    const uint8_t test_base256_buffer[3] = {20, 110, 37};

    size_t output_num = 0;
    uint8_t output_base256_buffer[3] = {0};

    error = pksav_export_base256(
                test_num,
                output_base256_buffer,
                sizeof(output_base256_buffer)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_base256_buffer,
        output_base256_buffer,
        sizeof(test_base256_buffer)
    );

    error = pksav_import_base256(
                test_base256_buffer,
                sizeof(test_base256_buffer),
                &output_num
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(test_num, output_num);
}

static void bcd_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    // Test a number with an odd number of digits to test that the no digit
    // is added to the lower half of the first byte.

    const size_t test_num_odd = 12345;
    const uint8_t test_bcd_buffer_odd[3] = {0x01, 0x23, 0x45};

    size_t output_num_odd = 0;
    uint8_t output_bcd_buffer_odd[3] = {0};

    error = pksav_export_bcd(
                test_num_odd,
                output_bcd_buffer_odd,
                sizeof(output_bcd_buffer_odd)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_bcd_buffer_odd,
        output_bcd_buffer_odd,
        sizeof(test_bcd_buffer_odd)
    );

    error = pksav_import_bcd(
                test_bcd_buffer_odd,
                sizeof(test_bcd_buffer_odd),
                &output_num_odd
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(test_num_odd, output_num_odd);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(base256_test)
    PKSAV_TEST(bcd_test)
)
