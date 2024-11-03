#include <starjumper/starjumper.h>

#include <checks.h>
#include <stdlib.h>


static void
test_from_string(void)
{
    struct sj_hex_coordinate hex_coordinate = {
        .horizontal = 0,
        .vertical = 0
    };
    bool success = sj_hex_coordinate_from_string("0105", &hex_coordinate);
    ASSERT_TRUE(success);
    ASSERT_INT_EQ(hex_coordinate.horizontal, 1);
    ASSERT_INT_EQ(hex_coordinate.vertical, 5);

    success = sj_hex_coordinate_from_string("1242", &hex_coordinate);
    ASSERT_TRUE(success);
    ASSERT_INT_EQ(hex_coordinate.horizontal, 12);
    ASSERT_INT_EQ(hex_coordinate.vertical, 42);

    success = sj_hex_coordinate_from_string("", &hex_coordinate);
    ASSERT_FALSE(success);

    success = sj_hex_coordinate_from_string("01", &hex_coordinate);
    ASSERT_FALSE(success);

    success = sj_hex_coordinate_from_string("011", &hex_coordinate);
    ASSERT_FALSE(success);
}


static void
test_string_alloc(void)
{
    struct sj_hex_coordinate hex_coordinate = {
        .horizontal = 0,
        .vertical = 0
    };
    char *string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    ASSERT_NOT_NULL(string);
    ASSERT_STR_EQ(string, "0000");
    free(string);

    hex_coordinate.horizontal = 1;
    hex_coordinate.vertical = 5;
    string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    ASSERT_NOT_NULL(string);
    ASSERT_STR_EQ(string, "0105");
    free(string);

    hex_coordinate.horizontal = 12;
    hex_coordinate.vertical = 42;
    string = sj_string_alloc_from_hex_coordinate(hex_coordinate);
    ASSERT_NOT_NULL(string);
    ASSERT_STR_EQ(string, "1242");
    free(string);
}


static void
test_make(void)
{
    struct sj_hex_coordinate hex_coordinate = sj_hex_coordinate_make(1, 5);
    ASSERT_INT_EQ(hex_coordinate.horizontal, 1);
    ASSERT_INT_EQ(hex_coordinate.vertical, 5);

    hex_coordinate = sj_hex_coordinate_make(12, 42);
    ASSERT_INT_EQ(hex_coordinate.horizontal, 12);
    ASSERT_INT_EQ(hex_coordinate.vertical, 42);
}


int
main(int argc, char *argv[])
{
    test_from_string();
    test_string_alloc();
    test_make();
    return EXIT_SUCCESS;
}
