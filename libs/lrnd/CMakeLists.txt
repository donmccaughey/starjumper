cmake_minimum_required(VERSION 3.15...3.30)
project(lrnd
        VERSION 0.0.1
        DESCRIPTION "lrnd"
        HOMEPAGE_URL https://git.sr.ht/~donmcc/lrnd
        LANGUAGES C
)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

option(BUILD_TESTING "Build the tests" ON)

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
include(GNUInstallDirs)


include(CheckSymbolExists)
check_symbol_exists(arc4random "stdlib.h" lrnd_has_arc4random)
check_symbol_exists(getentropy "unistd.h" lrnd_has_getentropy_in_unistd)
if(NOT lrnd_has_getentropy_in_unistd)
    check_symbol_exists(getentropy "sys/random.h" lrnd_has_getentropy_in_sys_random)
endif()
check_symbol_exists(random_r "stdlib.h" lrnd_has_random_r)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(lrnd_is_top_level_project TRUE)
endif()


function(add_lrnd_library name)
    add_library(${name} STATIC
            lrnd.h
            lrnd.c
    )
    if(lrnd_has_arc4random)
        target_compile_definitions(${name} PUBLIC LRND_HAS_ARC4RANDOM)
    endif()
    if(lrnd_has_random_r)
        target_compile_definitions(${name} PUBLIC LRND_HAS_RANDOM_R)
    endif()
    target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()


add_lrnd_library(lrnd)
if(lrnd_has_getentropy_in_unistd)
    target_compile_definitions(lrnd PUBLIC LRND_HAS_GETENTROPY_IN_UNISTD)
elseif(lrnd_has_getentropy_in_sys_random)
    target_compile_definitions(lrnd PUBLIC LRND_HAS_GETENTROPY_IN_SYS_RANDOM)
endif()


if(lrnd_is_top_level_project AND BUILD_TESTING)
    add_lrnd_library(lrnd_ndebug)
    target_compile_definitions(lrnd_ndebug PRIVATE NDEBUG)
    # don't set LRND_HAS_GETENTROPY definition to test getentropy() polyfill

    add_executable(lrnd_tests
            lrnd_tests.c
    )
    target_link_libraries(lrnd_tests lrnd)
    add_test(lrnd_tests lrnd_tests)

    add_executable(lrnd_ndebug_tests
            lrnd_ndebug_tests.c
    )
    target_link_libraries(lrnd_ndebug_tests lrnd_ndebug)
    add_test(lrnd_ndebug_tests lrnd_ndebug_tests)
endif()


install(TARGETS lrnd)
install(FILES lrnd.h TYPE INCLUDE)
install(FILES lrnd.3 TYPE MAN)
