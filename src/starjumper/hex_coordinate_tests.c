#include <starjumper/starjumper.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


static void
test_from_string(void)
{
    struct sj_hex_coordinate hex_coordinate = {
        .horizontal = 0,
        .vertical = 0
    };
    bool success = sj_hex_coordinate_from_string("0105", &hex_coordinate);
    assert(success);
    assert(1 == hex_coordinate.horizontal);
    assert(5 == hex_coordinate.vertical);

    success = sj_hex_coordinate_from_string("1242", &hex_coordinate);
    assert(success);
    assert(12 == hex_coordinate.horizontal);
    assert(42 == hex_coordinate.vertical);

    success = sj_hex_coordinate_from_string("", &hex_coordinate);
    assert(!success);

    success = sj_hex_coordinate_from_string("01", &hex_coordinate);
    assert(!success);

    success = sj_hex_coordinate_from_string("011", &hex_coordinate);
    assert(!success);
}


static void
test_string_alloc(void)
{
    struct sj_hex_coordinate hex_coordinate = {
        .horizontal = 0,
        .vertical = 0
    };
    char *string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    assert(string);
    assert(0 == strcmp("0000", string));
    free(string);

    hex_coordinate.horizontal = 1;
    hex_coordinate.vertical = 5;
    string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    assert(string);
    assert(0 == strcmp("0105", string));
    free(string);

    hex_coordinate.horizontal = 12;
    hex_coordinate.vertical = 42;
    string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    assert(string);
    assert(0 == strcmp("1242", string));
    free(string);
}


static void
test_make(void)
{
    struct sj_hex_coordinate hex_coordinate = sj_hex_coordinate_make(1, 5);
    assert(1 == hex_coordinate.horizontal);
    assert(5 == hex_coordinate.vertical);

    hex_coordinate = sj_hex_coordinate_make(12, 42);
    assert(12 == hex_coordinate.horizontal);
    assert(42 == hex_coordinate.vertical);
}


int
main(int argc, char *argv[])
{
    test_from_string();
    test_string_alloc();
    test_make();
    return EXIT_SUCCESS;
}
