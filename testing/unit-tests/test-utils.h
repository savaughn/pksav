/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_TEST_UTILS_H
#define PKSAV_TEST_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

int get_filesize(
    const char* filename,
    size_t* result_out
);

int read_file_into_buffer(
    const char* filename,
    uint8_t* buffer,
    size_t buffer_len
);

int do_files_differ(
    const char* filename1,
    const char* filename2,
    bool* result_out
);

#endif /* PKSAV_TEST_UTILS_H */
