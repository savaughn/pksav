#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(__enabled_components "" CACHE INTERNAL "" FORCE)
SET(__disabled_components "" CACHE INTERNAL "" FORCE)

INCLUDE(CMakeDependentOption)

MACRO(PKSAV_REGISTER_COMPONENT name var)
    CMAKE_DEPENDENT_OPTION(${var} "${name}" ON "" OFF)

    IF(${var})
        LIST(APPEND _pksav_enabled_components ${name})
    ELSE()
        LIST(APPEND _pksav_disabled_components ${name})
    ENDIF(${var})
ENDMACRO(PKSAV_REGISTER_COMPONENT)

FUNCTION(PKSAV_PRINT_COMPONENT_SUMMARY)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# PKSav - Enabled Components                         #")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_pksav_enabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
    MESSAGE(STATUS "######################################################")
    MESSAGE(STATUS "# PKSav - Disabled Components                        #")
    MESSAGE(STATUS "######################################################")
    FOREACH(comp ${_pksav_disabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    MESSAGE(STATUS "")
ENDFUNCTION(PKSAV_PRINT_COMPONENT_SUMMARY)

