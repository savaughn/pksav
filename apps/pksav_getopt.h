/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GETOPT_H
#define PKSAV_GETOPT_H

#include <getopt/getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char filepath[1024] = {'\0'};

static int pksav_save_specified = 0;
static int pksav_getopt_party   = 0;
static int pksav_getopt_pc      = 0;
static int pksav_getopt_items   = 0;
static int pksav_getopt_all     = 0;

static const struct option pksav_getopt_options[] = {
    {"input", required_argument, NULL, 'i'},
    {"party", no_argument,       &pksav_getopt_party, 1},
    {"pc",    no_argument,       &pksav_getopt_pc,    1},
    {"items", no_argument,       &pksav_getopt_items, 1},
    {"all",   no_argument,       &pksav_getopt_all,   1},
    {NULL,    0,                 NULL,                0}
};

int pksav_getopt(int argc, char** argv) {
    int opt, option_index;
    while((opt = getopt_long(argc, argv, "i:pbca", pksav_getopt_options, &option_index)) != -1) {
        switch(opt) {
            case 'i':
                strcpy(filepath, optarg);
                pksav_save_specified = 1;
                break;

            default:
                break;
        }
    }

    if(!pksav_save_specified) {
        fputs("No save specified.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#endif /* PKSAV_GETOPT_H */
