#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checks.h"


static void
test_verify_false_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    bool value = false;
    cks_verify_false(
            "file.c", 10, "test_file",
            "value", value,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq(buffer, "");
}


static void
test_verify_false_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    bool value = true;
    cks_verify_false(
            "file.c", 10, "test_file",
            "value", value,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`value` expected to be false");
}


static void
test_verify_int_eq_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int i = 42;
    cks_verify_int_eq(
            "file.c", 10, "test_file",
            "i", i, "42", 42,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq(buffer, "");
}


static void
test_verify_int_eq_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int i = 17;
    cks_verify_int_eq(
            "file.c", 10, "test_file",
            "i", i, "42", 42,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`i` expected to equal `42`");
    check_str_contains(buffer, "int1 (i): 17");
    check_str_contains(buffer, "int2: 42");
}


static void
test_verify_not_null_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int i = 42;
    int *ptr = &i;
    cks_verify_not_null(
            "file.c", 10, "test_file",
            "ptr", ptr,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq(buffer, "");
}


static void
test_verify_not_null_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int *ptr = NULL;
    cks_verify_not_null(
            "file.c", 10, "test_file",
            "ptr", ptr,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`ptr` expected to be non-NULL");
}


static void
test_verify_null_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int *ptr = NULL;
    cks_verify_null(
            "file.c", 10, "test_file",
            "ptr", ptr,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq(buffer, "");
}


static void
test_verify_null_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    int i = 42;
    int *ptr = &i;
    cks_verify_null(
            "file.c", 10, "test_file",
            "ptr", ptr,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`ptr` expected to be NULL");
}


static void
test_verify_str_contains_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *str = "foobar";
    char const *substring = "foo";
    cks_verify_str_contains(
            "file.c", 10, "test_file",
            "str", str, "substring", substring,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq("", buffer);
}


static void
test_verify_str_contains_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *line = "foo";
    char const *fragment = "bar";
    cks_verify_str_contains(
            "file.c", 10, "test_file",
            "line", line, "fragment", fragment,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`line` expected to contain `fragment`");
    check_str_contains(buffer, "str (line): \"foo\"");
    check_str_contains(buffer, "substr (fragment): \"bar\"");
}


static void
test_verify_str_eq_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo";
    char const *name2 = "foo";
    cks_verify_str_eq(
            "file.c", 10, "test_file",
            "name1", name1, "name2", name2,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq("", buffer);
}


static void
test_verify_str_eq_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo";
    char const *name2 = "bar";
    cks_verify_str_eq(
            "file.c", 10, "test_file",
            "name1", name1, "name2", name2,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`name1` expected to equal `name2`");
    check_str_contains(buffer, "str1 (name1): \"foo\"");
    check_str_contains(buffer, "str2 (name2): \"bar\"");
}


static void
test_verify_str_eq_fails_when_multiline(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *name1 = "foo\n  bar\n  baz\n";
    char const *name2 = "foo\n  xam\n  baz\n";
    cks_verify_str_eq(
            "file.c", 10, "test_file",
            "name1", name1, "name2", name2,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`name1` expected to equal `name2`");
    check_str_contains(buffer, "str1 (name1): 3 lines");
    check_str_contains(buffer, "str2 (name2): 3 lines");
    check_str_contains(buffer, "====== \"foo\"");
    check_str_contains(buffer, "str1 < \"  bar\"");
    check_str_contains(buffer, "str2 > \"  xam\"");
    check_str_contains(buffer, "====== \"  baz\"");
}


static void
test_verify_true_passes(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    bool value = true;
    cks_verify_true(
            "file.c", 10, "test_file",
            "value", value,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_eq(buffer, "");
}


static void
test_verify_true_fails(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    bool value = false;
    cks_verify_true(
            "file.c", 10, "test_file",
            "value", value,
            out, cks_on_fail_continue
    );

    fclose(out);
    check_str_contains(buffer, "file.c:10: in test_file():");
    check_str_contains(buffer, "`value` expected to be true");
}


static void
test_is_substr(void)
{
    check_true(cks_is_substr("foobar", "foo"));
    check_true(cks_is_substr("foo", "foo"));
    check_true(cks_is_substr("foo", ""));
    check_true(cks_is_substr("", ""));

    check_false(cks_is_substr("foo", NULL));
    check_false(cks_is_substr(NULL, "bar"));
    check_false(cks_is_substr(NULL, NULL));
    check_false(cks_is_substr("foo", "bar"));
}


static void
test_is_str_eq(void)
{
    char foo1[] = "foo";
    char foo2[] = "foo";
    check_true(cks_is_str_eq(NULL, NULL));
    check_true(cks_is_str_eq(foo1, foo1));
    check_true(cks_is_str_eq(foo1, foo2));

    check_false(cks_is_str_eq("foo", NULL));
    check_false(cks_is_str_eq(NULL, "foo"));
    check_false(cks_is_str_eq("foo", "bar"));
}


static void
test_open_buffer()
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    check_true(buffer[0] == 0);
    check_true(buffer[199] == 0);

    fprintf(out, "Hello, world!");
    fclose(out);

    check_str_eq(buffer, "Hello, world!");
}


static void
test_print_int_expression(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    cks_print_int_expression(out, "int1", "i", 42);

    fclose(out);
    check_str_contains(buffer, "int1 (i): 42\n");
}


static void
test_print_int_expression_for_int_literal(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    cks_print_int_expression(out, "int1", "42", 42);

    fclose(out);
    check_str_contains(buffer, "int1: 42\n");
}


static void
test_print_str_diff_for_equal_line_count(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *str1 = "foo\n  bar\n  baz\n";
    char const *str2 = "foo\n  xam\n  baz\n";
    cks_print_str_diff(out, str1, str2);

    fclose(out);
    check_str_contains(buffer, "====== \"foo\"");
    check_str_contains(buffer, "str1 < \"  bar\"");
    check_str_contains(buffer, "str2 > \"  xam\"");
    check_str_contains(buffer, "====== \"  baz\"");
}


static void
test_print_str_diff_for_different_line_count(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    char const *str1 = "foo\n  bar\n  baz\n";
    char const *str2 = "foo\n  xam";
    cks_print_str_diff(out, str1, str2);

    fclose(out);
    check_str_contains(buffer, "====== \"foo\"");
    check_str_contains(buffer, "str1 < \"  bar\"");
    check_str_contains(buffer, "str2 > \"  xam\"");
    check_str_contains(buffer, "str1 < \"  baz\"");
    check_str_contains(buffer, "str2 > \"\"");
}


static void
test_print_str_expression(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    cks_print_str_expression(out, "str", "buffer", "Hello!");

    fclose(out);
    check_str_contains(buffer, "str (buffer): \"Hello!\"\n");
}


static void
test_print_str_expression_for_string_literal(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    cks_print_str_expression(out, "str", "\"Hello!\"", "Hello!");

    fclose(out);
    check_str_contains(buffer, "str: \"Hello!\"\n");
}


static void
test_print_str_expression_for_null_value(void)
{
    char buffer[200];
    FILE *out = cks_open_buffer(buffer, sizeof buffer);

    cks_print_str_expression(out, "str", "title", NULL);

    fclose(out);
    check_str_contains(buffer, "str (title): NULL\n");
}


int
main(int argc, char *argv[])
{
    test_verify_false_passes();
    test_verify_false_fails();

    test_verify_int_eq_passes();
    test_verify_int_eq_fails();
    test_verify_str_eq_fails_when_multiline();

    test_verify_not_null_passes();
    test_verify_not_null_fails();

    test_verify_null_passes();
    test_verify_null_fails();

    test_verify_true_passes();
    test_verify_true_fails();

    test_is_substr();
    test_is_str_eq();

    test_open_buffer();

    test_print_int_expression();
    test_print_int_expression_for_int_literal();

    test_print_str_diff_for_equal_line_count();
    test_print_str_diff_for_different_line_count();

    test_print_str_expression();
    test_print_str_expression_for_string_literal();
    test_print_str_expression_for_null_value();

    test_verify_str_contains_passes();
    test_verify_str_contains_fails();

    test_verify_str_eq_passes();
    test_verify_str_eq_fails();

    return EXIT_SUCCESS;
}
