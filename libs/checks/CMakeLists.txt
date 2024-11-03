cmake_minimum_required(VERSION 3.15...3.30)
project(checks
        VERSION 0.0.1
        DESCRIPTION "checks"
        HOMEPAGE_URL https://git.sr.ht/~donmcc/checks
        LANGUAGES C
)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
option(BUILD_TESTING "Build the tests" ON)
option(COVERAGE "Enable code coverage analysis")
option(WALL "Enable all warnings")


set(CMAKE_C_STANDARD 99)
enable_testing()
include(GNUInstallDirs)


if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

if(COVERAGE)
    add_compile_options(--coverage -g -O0)
    add_link_options(--coverage)
endif()

if(WALL)
    add_compile_options(-Wall -Werror -Wno-deprecated-declarations)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(checks_is_top_level_project TRUE)
endif()


add_library(checks STATIC
        checks.h
        checks.c
)
target_include_directories(checks
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


if(checks_is_top_level_project AND BUILD_TESTING)
    add_executable(checks_test
            checks_tests.c
    )
    target_link_libraries(checks_test
            checks
    )
    add_test(checks_tests checks_test)
endif()


install(TARGETS checks)
install(FILES checks.h TYPE INCLUDE)
install(FILES checks.3 TYPE MAN)


