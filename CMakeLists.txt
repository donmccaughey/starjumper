cmake_minimum_required(VERSION 3.13...3.22)
project(starjumper
        VERSION 0.0.1
        DESCRIPTION "Starjumper"
        HOMEPAGE_URL https://github.com/donmccaughey/Starjumper
        LANGUAGES C
        )

# ----- compatibility checks -----

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # make asprintf() visible
    add_definitions(-D_GNU_SOURCE)
endif()


# ----- add build files in subdirectories -----

add_subdirectory(libstarjumper)
add_subdirectory(worldgen)

