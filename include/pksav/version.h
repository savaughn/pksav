/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_VERSION_H
#define PKSAV_VERSION_H

/*!
 * @brief A string literal representing this PKSav build's version number.
 *
 * The version number is laid out as "X.Y.Z", where X corresponds to the
 * major version number, Y corresponds to the minor version number, and
 * Z corresponds to the release version number.
 */
#define PKSAV_VERSION        "0.0.0"

/*!
 * @brief An integer representation of this PKSav build's version number.
 *
 * This number corresponds to the "X.Y.Z" version number as follows:
 *
 * (X * 1000000) + (Y * 1000) + Z
 */
#define PKSAV_VERSION_NUMBER 000000000

#endif /* PKSAV_VERSION_H */
