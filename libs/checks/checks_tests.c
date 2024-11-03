#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checks.h"


static void
test_check_false_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    bool value = false;
    check_false(
        "file.c", 10, "test_file",
        "value", value,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ(buffer, "");
}


static void
test_check_false_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    bool value = true;
    check_false(
        "file.c", 10, "test_file",
        "value", value,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`value` expected to be false");
}


static void
test_check_int_eq_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int i = 42;
    check_int_eq(
        "file.c", 10, "test_file",
        "i", i, "42", 42,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ(buffer, "");
}


static void
test_check_int_eq_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int i = 17;
    check_int_eq(
        "file.c", 10, "test_file",
        "i", i, "42", 42,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`i` expected to equal `42`");
    ASSERT_STR_CONTAINS(buffer, "int1 (i): 17");
    ASSERT_STR_CONTAINS(buffer, "int2: 42");
}


static void
test_check_not_null_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int i = 42;
    int *ptr = &i;
    check_not_null(
        "file.c", 10, "test_file",
        "ptr", ptr,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ(buffer, "");
}


static void
test_check_not_null_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int *ptr = NULL;
    check_not_null(
        "file.c", 10, "test_file",
        "ptr", ptr,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`ptr` expected to be non-NULL");
}


static void
test_check_null_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int *ptr = NULL;
    check_null(
        "file.c", 10, "test_file",
        "ptr", ptr,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ(buffer, "");
}


static void
test_check_null_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    int i = 42;
    int *ptr = &i;
    check_null(
        "file.c", 10, "test_file",
        "ptr", ptr,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`ptr` expected to be NULL");
}


static void
test_check_str_contains_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *str = "foobar";
    char const *substring = "foo";
    check_str_contains(
        "file.c", 10, "test_file",
        "str", str, "substring", substring,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ("", buffer);
}


static void
test_check_str_contains_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *line = "foo";
    char const *fragment = "bar";
    check_str_contains(
        "file.c", 10, "test_file",
        "line", line, "fragment", fragment,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`line` expected to contain `fragment`");
    ASSERT_STR_CONTAINS(buffer, "str (line): \"foo\"");
    ASSERT_STR_CONTAINS(buffer, "substr (fragment): \"bar\"");
}


static void
test_check_str_eq_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo";
    char const *name2 = "foo";
    check_str_eq(
        "file.c", 10, "test_file",
        "name1", name1, "name2", name2,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ("", buffer);
}


static void
test_check_str_eq_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo";
    char const *name2 = "bar";
    check_str_eq(
        "file.c", 10, "test_file",
        "name1", name1, "name2", name2,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`name1` expected to equal `name2`");
    ASSERT_STR_CONTAINS(buffer, "str1 (name1): \"foo\"");
    ASSERT_STR_CONTAINS(buffer, "str2 (name2): \"bar\"");
}


static void
test_check_str_eq_fails_when_multiline(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo\n  bar\n  baz\n";
    char const *name2 = "foo\n  xam\n  baz\n";
    check_str_eq(
        "file.c", 10, "test_file",
        "name1", name1, "name2", name2,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`name1` expected to equal `name2`");
    ASSERT_STR_CONTAINS(buffer, "str1 (name1): 3 lines");
    ASSERT_STR_CONTAINS(buffer, "str2 (name2): 3 lines");
    ASSERT_STR_CONTAINS(buffer, "====== \"foo\"");
    ASSERT_STR_CONTAINS(buffer, "str1 < \"  bar\"");
    ASSERT_STR_CONTAINS(buffer, "str2 > \"  xam\"");
    ASSERT_STR_CONTAINS(buffer, "====== \"  baz\"");
}


static void
test_check_true_passes(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    bool value = true;
    check_true(
        "file.c", 10, "test_file",
        "value", value,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_EQ(buffer, "");
}


static void
test_check_true_fails(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    bool value = false;
    check_true(
        "file.c", 10, "test_file",
        "value", value,
        check_type_expect, out
    );

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "file.c:10: in test_file():");
    ASSERT_STR_CONTAINS(buffer, "`value` expected to be true");
}


static void
test_open_buffer()
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    ASSERT_TRUE(buffer[0] == 0);
    ASSERT_TRUE(buffer[199] == 0);

    fprintf(out, "Hello, world!");
    fclose(out);

    ASSERT_STR_EQ(buffer, "Hello, world!");
}


static void
test_print_int_expression(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    print_int_expression(out, "int1", "i", 42);

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "int1 (i): 42\n");
}


static void
test_print_int_expression_for_int_literal(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    print_int_expression(out, "int1", "42", 42);

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "int1: 42\n");
}


static void
test_print_str_diff_for_equal_line_count(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *str1 = "foo\n  bar\n  baz\n";
    char const *str2 = "foo\n  xam\n  baz\n";
    print_str_diff(out, str1, str2);

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "====== \"foo\"");
    ASSERT_STR_CONTAINS(buffer, "str1 < \"  bar\"");
    ASSERT_STR_CONTAINS(buffer, "str2 > \"  xam\"");
    ASSERT_STR_CONTAINS(buffer, "====== \"  baz\"");
}


static void
test_print_str_diff_for_different_line_count(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    char const *str1 = "foo\n  bar\n  baz\n";
    char const *str2 = "foo\n  xam";
    print_str_diff(out, str1, str2);

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "====== \"foo\"");
    ASSERT_STR_CONTAINS(buffer, "str1 < \"  bar\"");
    ASSERT_STR_CONTAINS(buffer, "str2 > \"  xam\"");
    ASSERT_STR_CONTAINS(buffer, "str1 < \"  baz\"");
    ASSERT_STR_CONTAINS(buffer, "str2 > \"\"");
}


static void
test_print_str_expression(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    print_str_expression(out, "str", "buffer", "Hello!");

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "str (buffer): \"Hello!\"\n");
}


static void
test_print_str_expression_for_string_literal(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    print_str_expression(out, "str", "\"Hello!\"", "Hello!");

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "str: \"Hello!\"\n");
}


static void
test_print_str_expression_for_null_value(void)
{
    char buffer[200];
    FILE *out = open_buffer(buffer, sizeof buffer);

    print_str_expression(out, "str", "title", NULL);

    fclose(out);
    ASSERT_STR_CONTAINS(buffer, "str (title): NULL\n");
}


static void
test_str_contains(void)
{
    ASSERT_TRUE(str_contains("foobar", "foo"));
    ASSERT_TRUE(str_contains("foo", "foo"));
    ASSERT_TRUE(str_contains("foo", ""));
    ASSERT_TRUE(str_contains("", ""));

    ASSERT_FALSE(str_contains("foo", NULL));
    ASSERT_FALSE(str_contains(NULL, "bar"));
    ASSERT_FALSE(str_contains(NULL, NULL));
    ASSERT_FALSE(str_contains("foo", "bar"));
}


static void
test_str_eq(void)
{
    char foo1[] = "foo";
    char foo2[] = "foo";
    ASSERT_TRUE(str_eq(NULL, NULL));
    ASSERT_TRUE(str_eq(foo1, foo1));
    ASSERT_TRUE(str_eq(foo1, foo2));

    ASSERT_FALSE(str_eq("foo", NULL));
    ASSERT_FALSE(str_eq(NULL, "foo"));
    ASSERT_FALSE(str_eq("foo", "bar"));
}


int
main(int argc, char *argv[])
{
    test_check_false_passes();
    test_check_false_fails();

    test_check_int_eq_passes();
    test_check_int_eq_fails();
    test_check_str_eq_fails_when_multiline();

    test_check_not_null_passes();
    test_check_not_null_fails();

    test_check_null_passes();
    test_check_null_fails();

    test_open_buffer();

    test_print_int_expression();
    test_print_int_expression_for_int_literal();

    test_print_str_diff_for_equal_line_count();
    test_print_str_diff_for_different_line_count();

    test_print_str_expression();
    test_print_str_expression_for_string_literal();
    test_print_str_expression_for_null_value();

    test_check_str_contains_passes();
    test_check_str_contains_fails();

    test_check_str_eq_passes();
    test_check_str_eq_fails();

    test_check_true_passes();
    test_check_true_fails();

    test_str_contains();
    test_str_eq();

    return EXIT_SUCCESS;
}
