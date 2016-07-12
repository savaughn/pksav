#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pksav_nds_headers
    text.h
)

INSTALL(
    FILES ${pksav_nds_headers}
    DESTINATION ${INCLUDE_DIR}/pksav/nds
    COMPONENT Headers
)
