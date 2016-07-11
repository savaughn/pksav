/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_ITEMS_H
#define PKSAV_ITEMS_H

#include <stdint.h>

#pragma pack(push,1)

typedef struct {
    uint8_t index;
    uint8_t count;
} pksav_gen1_item_t;

typedef struct {
    uint8_t count;
    pksav_gen1_item_t items[20];
    uint8_t terminator;
} pksav_gen1_item_bag_t;

typedef struct {
    uint8_t count;
    pksav_gen1_item_t items[50];
    uint8_t terminator;
} pksav_gen1_item_pc_t;

typedef struct {
    uint8_t count;
    uint8_t index;
} pksav_gen2_item_t;

typedef struct {
    uint8_t tm_count[50];
    uint8_t hm_count[7];
} pksav_gen2_tmhm_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[20];
    uint8_t terminator;
} pksav_gen2_item_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[26];
    uint8_t terminator;
} pksav_gen2_key_item_pocket_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[12];
    uint8_t terminator;
} pksav_gen2_ball_pocket_t;

typedef struct {
    pksav_gen2_tmhm_pocket_t tmhm_pocket;
    pksav_gen2_item_pocket_t item_pocket;
    pksav_gen2_key_item_pocket_t key_item_pocket;
    pksav_gen2_ball_pocket_t ball_pocket;
} pksav_gen2_item_bag_t;

typedef struct {
    uint8_t count;
    pksav_gen2_item_t items[50];
    uint8_t terminator;
} pksav_gen2_item_pc_t;

typedef struct {
    uint16_t index;
    uint16_t count;
} pksav_item_t;

typedef struct {
    pksav_item_t pc_items[50];
    pksav_item_t items[20];
    pksav_item_t key_items[20];
    pksav_item_t balls[16];
    pksav_item_t tms_hms[64];
    pksav_item_t berries[46];
} pksav_rs_item_storage_t;

typedef struct {
    pksav_item_t pc_items[50];
    pksav_item_t items[30];
    pksav_item_t key_items[30];
    pksav_item_t balls[16];
    pksav_item_t tms_hms[64];
    pksav_item_t berries[46];
} pksav_emerald_item_storage_t;

typedef struct {
    pksav_item_t pc_items[50];
    pksav_item_t items[42];
    pksav_item_t key_items[30];
    pksav_item_t balls[13];
    pksav_item_t tms_hms[58];
    pksav_item_t berries[43];
} pksav_frlg_item_storage_t;

typedef union {
    pksav_rs_item_storage_t rs;
    pksav_emerald_item_storage_t emerald;
    pksav_frlg_item_storage_t frlg;
} pksav_gba_item_storage_t;

typedef struct {
    pksav_item_t items[20];
    pksav_item_t key_items[43];
    pksav_item_t balls[16];
    pksav_item_t tms[64];
    pksav_item_t berries[46];
    pksav_item_t colognes[3];
} pksav_colosseum_item_bag_t;

typedef struct {
    pksav_item_t items[30];
    pksav_item_t key_items[43];
    pksav_item_t balls[16];
    pksav_item_t tms[64];
    pksav_item_t berries[46];
    pksav_item_t colognes[3];
    pksav_item_t battle_cds[60];
} pksav_xd_item_bag_t;

typedef union {
    pksav_colosseum_item_bag_t colosseum;
    pksav_xd_item_bag_t xd;
} pksav_gamecube_item_bag_t;

typedef struct {
    pksav_item_t items[235];
} pksav_gamecube_item_pc_t;

typedef struct {
    pksav_item_t items[165];
    pksav_item_t key_items[50];
    pksav_item_t tms_hms[100];
    pksav_item_t mail[12];
    pksav_item_t medicine[40];
    pksav_item_t berries[64];
    pksav_item_t balls[15];
    pksav_item_t battle_items[30];
} pksav_dppt_item_bag_t;

typedef struct {
    pksav_item_t items[165];
    pksav_item_t key_items[43];
    pksav_item_t tms_hms[100];
    pksav_item_t mail[12];
    pksav_item_t medicine[40];
    pksav_item_t berries[64];
    pksav_item_t balls[24];
    pksav_item_t battle_items[30];
} pksav_hgss_item_bag_t;

typedef union {
    pksav_dppt_item_bag_t dppt;
    pksav_hgss_item_bag_t hgss;
} pksav_gen4_item_bag_t;

typedef struct {
    pksav_item_t items[310];
    pksav_item_t key_items[83];
    pksav_item_t tms_hms[109];
    pksav_item_t medicine[48];
    pksav_item_t berries[64];
    uint8_t unknown[0x66];
    uint16_t checksum;
} pksav_bag_t;

#pragma pack(pop)

#endif /* PKSAV_ITEMS_H */
