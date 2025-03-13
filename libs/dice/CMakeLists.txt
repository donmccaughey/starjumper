cmake_minimum_required(VERSION 3.15...3.31)
project(dice
        VERSION 0.0.1
        DESCRIPTION "Gaming dice library"
        HOMEPAGE_URL https://git.sr.ht/~donmcc/dice
        LANGUAGES C
)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

option(BUILD_TESTING "Build the tests" ON)

option(COVERAGE "Enable code coverage analysis")
if(COVERAGE)
    add_compile_options(--coverage -g -O0)
    add_link_options(--coverage)
endif()

option(WALL "Enable all warnings")
if(WALL)
    add_compile_options(-Wall -Werror)
endif()


set(CMAKE_C_STANDARD 99)
enable_testing()
include(CheckSymbolExists)
include(GNUInstallDirs)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(is_top_level_project TRUE)
endif()


check_symbol_exists(arc4random_uniform "stdlib.h" has_arc4random_uniform)
if(NOT has_arc4random_uniform)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(bsd IMPORTED_TARGET REQUIRED libbsd-overlay)
endif()


add_library(dice STATIC dice.h dice.c)
if(bsd_FOUND)
    target_link_libraries(dice PUBLIC PkgConfig::bsd)
endif()


if(is_top_level_project AND BUILD_TESTING)
    add_executable(dice_tests dice_tests.c)
    target_link_libraries(dice_tests dice)
    add_test(dice_tests dice_tests)
endif()
