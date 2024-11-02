cmake_minimum_required(VERSION 3.13...3.30)
project(starjumper
        VERSION 0.0.1
        DESCRIPTION "Starjumper"
        HOMEPAGE_URL https://github.com/donmccaughey/starjumper
        LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
enable_testing()

# ----- compatibility checks -----

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # make asprintf() visible
    add_definitions(-D_GNU_SOURCE)
endif()


# ----- add build files in subdirectories -----

add_subdirectory(libstarjumper)
add_subdirectory(worldgen)
