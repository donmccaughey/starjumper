cmake_minimum_required(VERSION 3.13...3.30)
project(starjumper
        VERSION 0.0.1
        DESCRIPTION "Starjumper"
        HOMEPAGE_URL https://github.com/donmccaughey/starjumper
        LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
enable_testing()


# ----- find external dependencies -----

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_library(LIB_BSD
        NAMES bsd
    )
    if(NOT LIB_BSD)
        message(FATAL_ERROR "Unable to locate bsd compatibility library")
    endif()
    find_path(LIB_BSD_PATH
        "bsd.h"
        PATH_SUFFIXES bsd
    )
    if(NOT LIB_BSD_PATH)
        message(FATAL_ERROR "Unable to locate libbsd header files")
    endif()
    include_directories(BEFORE SYSTEM "${LIB_BSD_PATH}")
    add_definitions(-DLIBBSD_OVERLAY)
    link_libraries(bsd)
endif()


# ----- compatibility checks -----

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # make asprintf() visible
    add_definitions(-D_GNU_SOURCE)
endif()


# ----- add build files in subdirectories -----

add_subdirectory(libstarjumper)
add_subdirectory(worldgen)
