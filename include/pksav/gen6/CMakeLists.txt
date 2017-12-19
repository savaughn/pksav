#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_gen6_headers
    battle_maison_data.h
    coords.h
    daycare_data.h
    hall_of_fame.h
    items.h
    picture_data.h
    pokemon.h
    pokepuff_inventory.h
    ribbons.h
    save.h
    select_bound_items.h
    super_training_data.h
    text.h
    trainer_card.h
)

INSTALL(
    FILES ${pksav_gen6_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/gen6
    COMPONENT Headers
)
