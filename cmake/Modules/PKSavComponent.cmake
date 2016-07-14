#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

set(_pksav_enabled_components "" CACHE INTERNAL "" FORCE)
set(_pksav_disabled_components "" CACHE INTERNAL "" FORCE)

########################################################################
# Register a component into the system
#  - name the component string name
#  - var the global enable variable
#  - enb the default enable setting
#  - deps a list of dependencies
#  - dis the default disable setting
########################################################################
MACRO(PKSAV_REGISTER_COMPONENT name var enb deps dis)
    IF(NOT PKSAV_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "")
        MESSAGE(STATUS "Determining support for ${name}.")
        FOREACH(dep ${deps})
            MESSAGE(STATUS " - Dependency ${dep} = ${${dep}}")
        ENDFOREACH(dep)
    ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

    #setup the dependent option for this component
    INCLUDE(CMakeDependentOption)
    CMAKE_DEPENDENT_OPTION(${var} "enable ${name} support" ${enb} "${deps}" ${dis})

    #append the component into one of the lists
    IF(${var})
        IF(NOT PKSAV_USED_AS_SUBPROJECT)
            MESSAGE(STATUS " - Enabling ${name}.")
        ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)
        LIST(APPEND _pksav_enabled_components ${name})
    ELSE(${var})
        IF(NOT PKSAV_USED_AS_SUBPROJECT)
            MESSAGE(STATUS " - Disabling ${name}.")
        ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)
        LIST(APPEND _pksav_disabled_components ${name})
    ENDIF(${var})
    IF(NOT PKSAV_USED_AS_SUBPROJECT)
        MESSAGE(STATUS " - Override with -D${var}=ON/OFF")
    ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

    #make components lists into global variables
    SET(_pksav_enabled_components ${_pksav_enabled_components} CACHE INTERNAL "" FORCE)
    SET(_pksav_disabled_components ${_pksav_disabled_components} CACHE INTERNAL "" FORCE)
ENDMACRO(PKSAV_REGISTER_COMPONENT)

#Print the summary of enabled/disabled components
FUNCTION(PKSAV_PRINT_COMPONENT_SUMMARY)
    IF(PKSAV_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "Building PKSav with the following components:")
    ELSE()
        MESSAGE(STATUS "")
        MESSAGE(STATUS "######################################################")
        MESSAGE(STATUS "# PKSav - Enabled Components                         #")
        MESSAGE(STATUS "######################################################")
    ENDIF(PKSAV_USED_AS_SUBPROJECT)
    FOREACH(comp ${_pksav_enabled_components})
        MESSAGE(STATUS " - ${comp}")
    ENDFOREACH(comp)

    IF(NOT PKSAV_USED_AS_SUBPROJECT)
        MESSAGE(STATUS "")
        MESSAGE(STATUS "######################################################")
        MESSAGE(STATUS "# PKSav - Disabled Components                        #")
        MESSAGE(STATUS "######################################################")
        FOREACH(comp ${_pksav_disabled_components})
            MESSAGE(STATUS " - ${comp}")
        ENDFOREACH(comp)
    ENDIF(NOT PKSAV_USED_AS_SUBPROJECT)

    MESSAGE(STATUS "")
ENDFUNCTION(PKSAV_PRINT_COMPONENT_SUMMARY)

