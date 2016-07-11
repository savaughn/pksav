/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_STATS_H
#define PKSAV_STATS_H

#include <pksav/config.h>

#include <stdint.h>

typedef enum {
    PKSAV_STAT_NONE = 0,
    PKSAV_STAT_HP,
    PKSAV_STAT_ATTACK,
    PKSAV_STAT_DEFENSE,
    PKSAV_STAT_SPATK,
    PKSAV_STAT_SPDEF,
    PKSAV_STAT_SPEED,
    PKSAV_STAT_SPECIAL = 9
} pksav_battle_stat_t;

typedef struct {
    uint16_t first;
    uint16_t second;
} pksav_stat_pair_t;

#ifdef __cplusplus
extern "C" {
#endif

PKSAV_API int pksav_get_gb_IV(
    uint16_t* raw,
    pksav_battle_stat_t stat,
    uint8_t* IV_out
);

PKSAV_API int pksav_set_gb_IV(
    uint16_t* raw,
    pksav_battle_stat_t stat,
    uint8_t new_IV
);

PKSAV_API int pksav_get_IV(
    uint32_t* raw,
    pksav_battle_stat_t stat,
    uint8_t* IV_out
);

PKSAV_API int pksav_set_IV(
    uint32_t* raw,
    pksav_battle_stat_t stat,
    uint8_t new_IV
);

PKSAV_API int pksav_get_gb_stat(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    uint16_t EV,
    uint8_t IV,
    uint16_t* stat_out
);

PKSAV_API int pksav_get_gb_stat_range(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    pksav_stat_pair_t* stat_pair_out
);

PKSAV_API int pksav_get_stat(
    pksav_battle_stat_t stat,
    uint8_t level,
    float nature_modifier,
    uint8_t base_stat,
    uint16_t EV,
    uint8_t IV,
    uint16_t* stat_out
);

PKSAV_API int pksav_get_stat_range(
    pksav_battle_stat_t stat,
    uint8_t level,
    uint8_t base_stat,
    pksav_stat_pair_t* stat_pair_out
);

    /*
    PKSAV_API uint16_t get_modern_stat(
        battle_stats_t stat,
        uint8_t level,
        float nature_modifier,
        uint8_t base_stat,
        uint8_t EV,
        uint8_t IV
    );

    PKSAV_INLINE std::pair<uint16_t, uint16_t> get_modern_stat_range(
        battle_stats_t stat,
        uint8_t level,
        uint8_t base_stat
    ) {
        return std::make_pair(
                   get_modern_stat(stat, level, 0.9f, base_stat, 0, 0),
                   get_modern_stat(stat, level, 1.1f, base_stat, 255, 31)
               );
    }*/

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_STATS_H */
