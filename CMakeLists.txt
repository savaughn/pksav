########################################################################
# CMake configuration
########################################################################
cmake_minimum_required(VERSION 3.0)
project(pksav C CXX)

set(PKSAV_VERSION_MAJOR 2)
set(PKSAV_VERSION_MINOR 1)
set(PKSAV_VERSION_PATCH 0)
set(PKSAV_VERSION "${PKSAV_VERSION_MAJOR}.${PKSAV_VERSION_MINOR}.${PKSAV_VERSION_PATCH}")

set(PKSAV_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PKSAV_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKSAV_SOURCE_DIR})
    set(PKSAV_USED_AS_SUBPROJECT TRUE)
    if (NOT DEFINED PKSAV_DONT_INSTALL_HEADERS)
        set(PKSAV_DONT_INSTALL_HEADERS TRUE)
    endif (NOT DEFINED PKSAV_DONT_INSTALL_HEADERS)
endif (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKSAV_SOURCE_DIR})

set(CMAKE_MODULE_PATH "${PKSAV_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

####################################################################
# Project configuration
####################################################################
include(PKSavPlatform)
include(PKSavComponent)

if (NOT PKSAV_USED_AS_SUBPROJECT)
    find_package(Doxygen)
endif (NOT PKSAV_USED_AS_SUBPROJECT)

########################################################################
# Create uninstall target
########################################################################
if (NOT PKSAV_USED_AS_SUBPROJECT)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        @ONLY)

    add_custom_target(uninstall
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif (NOT PKSAV_USED_AS_SUBPROJECT)

####################################################################
# Set install subdirectories
####################################################################
if (NOT DEFINED LIB_SUFFIX AND REDHAT AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    set(LIB_SUFFIX 64)
endif ()
set(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
set(RUNTIME_DIR bin)
set(LIBRARY_DIR lib${LIB_SUFFIX})
set(INCLUDE_DIR include)
set(PKG_DATA_DIR share/pksav)
set(DOCS_DIR share/doc/pksav)

####################################################################
# Components
####################################################################
pksav_register_component("Library" PKSAV_ENABLE_LIBRARY ON "" OFF)

if (NOT PKSAV_USED_AS_SUBPROJECT)
    pksav_register_component("Applications"          PKSAV_ENABLE_APPS  ON "PKSAV_ENABLE_LIBRARY" OFF)
    pksav_register_component("Doxygen Documentation" PKSAV_ENABLE_DOCS  ON "PKSAV_ENABLE_LIBRARY;DOXYGEN_FOUND" OFF)
    pksav_register_component("Unit Tests"            PKSAV_ENABLE_TESTS ON "PKSAV_ENABLE_LIBRARY" OFF)
endif (NOT PKSAV_USED_AS_SUBPROJECT)

####################################################################
# Actual building
####################################################################
add_subdirectory(include)
add_subdirectory(lib)

if (PKSAV_ENABLE_APPS)
    add_subdirectory(apps)
endif (PKSAV_ENABLE_APPS)

if (PKSAV_ENABLE_DOCS)
    add_subdirectory(docs)
endif (PKSAV_ENABLE_DOCS)

if (PKSAV_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(testing)
endif (PKSAV_ENABLE_TESTS)

####################################################################
# Final display
####################################################################
pksav_print_component_summary()
if (NOT PKSAV_USED_AS_SUBPROJECT)
    message(STATUS "")
    message(STATUS "Building PKSav.")
    message(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    message(STATUS " - Compiler: ${CMAKE_C_COMPILER_ID}")
    message(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "")
endif (NOT PKSAV_USED_AS_SUBPROJECT)

# Add Emscripten-specific settings
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_C_COMPILER emcc)
    set(CMAKE_CXX_COMPILER em++)
    
    add_compile_options(-O3)
    add_compile_options(-s WASM=1)
    add_compile_options(-s EXPORTED_RUNTIME_METHODS='["cwrap"]')
    
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    include_directories(/opt/homebrew/Cellar/emscripten/3.1.50/libexec/system/include)
    
    # Add your source files manually
    add_library(pksav_lib STATIC
        lib/pksav.c
        # Add more source files as needed
    )
    
    # Link against the Emscripten-specific libraries
    target_link_libraries(pksav_lib
        GL
        al
        html5
        stubs
        noexit
        c
        dlmalloc
        compiler_rt
        c++-noexcept
        c++abi-noexcept
        sockets
    )
    
    # Specify the output binary
    add_executable(pksav_html apps/main.c)
    target_link_libraries(pksav_html pksav_lib)
endif (EMSCRIPTEN)
