/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_UTIL_FS_H
#define PKSAV_UTIL_FS_H

#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

enum pksav_error pksav_fs_filesize(
    const char* filepath,
    size_t* filesize_out
);

enum pksav_error pksav_fs_read_file_to_buffer(
    const char* filepath,
    uint8_t** buffer_ptr,
    size_t* buffer_len_out
);

enum pksav_error pksav_fs_write_buffer_to_file(
    const char* filepath,
    const uint8_t* buffer,
    size_t buffer_len
);

#endif /* PKSAV_UTIL_FS_H */
