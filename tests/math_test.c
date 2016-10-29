/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/math/base256.h>
#include <pksav/math/bcd.h>

PKSAV_TEST_MAIN(
    PKSAV_TEST(base256_test,
        pksav_error_t status = PKSAV_ERROR_NONE;

        uint32_t num1 = 1338917;
        uint8_t base256_buffer1[3] = {20, 110, 37};

        uint32_t num2 = 0;
        uint8_t base256_buffer2[3] = {0, 0, 0};

        status = pksav_to_base256(num1, base256_buffer2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT(!memcmp(base256_buffer1, base256_buffer2, 3));

        status = pksav_from_base256(base256_buffer2, 3, &num2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT_UINTS_EQUAL(num1, num2);
    )

    PKSAV_TEST(bcd_test_odd_count,
        pksav_error_t status = PKSAV_ERROR_NONE;

        uint32_t num1 = 23456;
        uint8_t bcd_buffer1[3] = {0x02, 0x34, 0x56};

        uint32_t num2 = 0;
        uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

        status = pksav_to_bcd(num1, bcd_buffer2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));

        status = pksav_from_bcd(bcd_buffer2, 3, &num2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT_UINTS_EQUAL(num1, num2);
    )

    PKSAV_TEST(bcd_test_even_count,
        pksav_error_t status = PKSAV_ERROR_NONE;

        uint32_t num1 = 123456;
        uint8_t bcd_buffer1[3] = {0x12, 0x34, 0x56};

        uint32_t num2 = 0;
        uint8_t bcd_buffer2[3] = {0x00, 0x00, 0x00};

        pksav_to_bcd(num1, bcd_buffer2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT(!memcmp(bcd_buffer1, bcd_buffer2, 3));

        status = pksav_from_bcd(bcd_buffer2, 3, &num2);
        PKSAV_ASSERT_INTS_EQUAL(status, PKSAV_ERROR_NONE);
        PKSAV_ASSERT_UINTS_EQUAL(num1, num2);
    )
)
