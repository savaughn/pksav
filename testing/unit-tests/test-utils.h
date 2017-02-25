/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_TEST_UTILS_H
#define PKSAV_TEST_UTILS_H

#include <pksav/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef PKSAV_PLATFORM_WIN32
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

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
