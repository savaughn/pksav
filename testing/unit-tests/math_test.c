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

    size_t output_num = 0;
    uint8_t output_bcd_buffer[3] = {0};

    // Test a number with an odd number of digits to test that the no digit
    // is added to the lower half of the first byte.

    const size_t test_num_odd = 12345;
    const uint8_t test_bcd_buffer_odd[3] = {0x01, 0x23, 0x45};

    error = pksav_export_bcd(
                test_num_odd,
                output_bcd_buffer,
                sizeof(output_bcd_buffer)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_bcd_buffer_odd,
        output_bcd_buffer,
        sizeof(test_bcd_buffer_odd)
    );

    error = pksav_import_bcd(
                test_bcd_buffer_odd,
                sizeof(test_bcd_buffer_odd),
                &output_num
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(test_num_odd, output_num);

    // Test a number with an even number of digits to test that all digits
    // are accounted for.

    const size_t test_num_even = 567890;
    const uint8_t test_bcd_buffer_even[3] = {0x56, 0x78, 0x90};

    error = pksav_export_bcd(
                test_num_even,
                output_bcd_buffer,
                sizeof(output_bcd_buffer)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_bcd_buffer_even,
        output_bcd_buffer,
        sizeof(test_bcd_buffer_even)
    );

    error = pksav_import_bcd(
                test_bcd_buffer_even,
                sizeof(test_bcd_buffer_even),
                &output_num
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(test_num_even, output_num);

    // Test with a multiple of 100 to make sure the function can account
    // for a valid empty byte.

    const size_t test_num_small = 7300;
    const uint8_t test_bcd_buffer_small[3] = {0x73, 0x00, 0x00};

    error = pksav_export_bcd(
                test_num_small,
                output_bcd_buffer,
                sizeof(output_bcd_buffer)
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_bcd_buffer_small,
        output_bcd_buffer,
        sizeof(test_bcd_buffer_small)
    );

    error = pksav_import_bcd(
                test_bcd_buffer_small,
                sizeof(test_bcd_buffer_small),
                &output_num
            );
    PKSAV_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(test_num_small, output_num);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(base256_test)
    PKSAV_TEST(bcd_test)
)
