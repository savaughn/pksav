/*!
 * @file    pksav.h
 * @ingroup PKSav
 * @brief   Global include file, includes all PKSav headers.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_H
#define PKSAV_H

#include <pksav/config.h>

#include <pksav/error.h>
#include <pksav/version.h>

#include <pksav/common/contest_stats.h>
#include <pksav/common/datetime.h>
#include <pksav/common/nds_pokemon.h>
#include <pksav/common/pokedex.h>
#include <pksav/common/pokerus.h>
#include <pksav/common/prng.h>
#include <pksav/common/stats.h>
#include <pksav/common/trainer_id.h>

#if PKSAV_ENABLE_GEN1
#include <pksav/gen1/items.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/save.h>
#include <pksav/gen1/text.h>
#endif /* PKSAV_ENABLE_GEN1 */

#if PKSAV_ENABLE_GEN2
#include <pksav/gen2/text.h>
#endif /* PKSAV_ENABLE_GEN2 */

#if PKSAV_ENABLE_GBA
#include <pksav/gba/text.h>
#endif /* PKSAV_ENABLE_GBA */

#if PKSAV_ENABLE_GCN
#include <pksav/gcn/text.h>
#endif /* PKSAV_ENABLE_GCN */

#if PKSAV_ENABLE_GEN4
#include <pksav/gen4/text.h>
#endif /* PKSAV_ENABLE_GEN4 */

#if PKSAV_ENABLE_GEN5
#include <pksav/gen5/text.h>
#endif /* PKSAV_ENABLE_GEN5 */

#if PKSAV_ENABLE_GEN6
#endif /* PKSAV_ENABLE_GEN6 */

#if PKSAV_ENABLE_GEN1 || PKSAV_ENABLE_GEN2
#include <pksav/math/base256.h>
#include <pksav/math/bcd.h>
#endif /* PKSAV_ENABLE_GEN1 || PKSAV_ENABLE_GEN2 */

#include <pksav/math/endian.h>

#endif /* PKSAV_H */
