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
                3
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_MEMORY(
        test_base256_buffer,
        output_base256_buffer,
        sizeof(test_base256_buffer)
    );

    error = pksav_import_base256(
                test_base256_buffer,
                3,
                &output_num
            );
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(test_num, output_num);
}

static void bcd_odd_count_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint32_t num1 = 23456;
    uint8_t bcd_buffer1[3] = {0x02, 0x34, 0x56};

    uint32_t num2 = 0;
    uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

    error = pksav_to_bcd(num1, bcd_buffer2);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));

    error = pksav_from_bcd(bcd_buffer2, 3, &num2);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num1, num2);
}

static void bcd_even_count_test()
{
    enum pksav_error error = PKSAV_ERROR_NONE;

    uint32_t num1 = 123456;
    uint8_t bcd_buffer1[3] = {0x12, 0x34, 0x56};

    uint32_t num2 = 0;
    uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

    pksav_to_bcd(num1, bcd_buffer2);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));

    error = pksav_from_bcd(bcd_buffer2, 3, &num2);
    TEST_ASSERT_EQUAL(PKSAV_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num1, num2);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(base256_test)
    PKSAV_TEST(bcd_odd_count_test)
    PKSAV_TEST(bcd_even_count_test)
)
