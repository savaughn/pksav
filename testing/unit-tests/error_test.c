/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pksav/error.h>

#include <string.h>

static void pksav_strerror_test()
{
    static const enum pksav_error MIN_ERROR_CODE = PKSAV_ERROR_NONE;
    static const enum pksav_error MAX_ERROR_CODE = PKSAV_ERROR_NULL_POINTER;

    const char* min_strerror = pksav_strerror(MIN_ERROR_CODE);
    TEST_ASSERT_NOT_NULL(min_strerror);
    TEST_ASSERT_TRUE(strlen(min_strerror) > 0ULL);

    const char* max_strerror = pksav_strerror(MAX_ERROR_CODE);
    TEST_ASSERT_NOT_NULL(max_strerror);
    TEST_ASSERT_TRUE(strlen(max_strerror) > 0ULL);

    const char* below_min_strerror = pksav_strerror(MIN_ERROR_CODE-1);
    TEST_ASSERT_NULL(below_min_strerror);

    const char* below_max_strerror = pksav_strerror(MAX_ERROR_CODE+1);
    TEST_ASSERT_NULL(below_max_strerror);
}

PKSAV_TEST_MAIN(
    PKSAV_TEST(pksav_strerror_test)
)
