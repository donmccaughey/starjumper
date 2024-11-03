#include "checks.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static int
count_lines(char const *str)
{
    int count = 0;
    char const *line = str;
    while ((line = strchr(line, '\n'))) {
        count++;
        line++;
    }
    return count;
}


static size_t
first_line_length(char const *str)
{
    if (!str) return 0;
    char const *end_of_line = strchr(str, '\n');
    return end_of_line ? end_of_line - str : strlen(str);
}


static bool
is_line_eq(char const *line1, size_t len1, char const *line2, size_t len2)
{
    return len1 == len2
        && 0 == strncmp(line1, line2, len1);
}


static bool
is_str_literal(char const *expression)
{
    size_t length = strlen(expression);
    return length >= 2
        && expression[0] == '"'
        && expression[length - 1] == '"';
}


static bool
is_str_multi_line(char const *str)
{
    return NULL != strchr(str, '\n');
}


static char const *
next_line(char const *str, size_t len)
{
    char const *line = str + len;
    return line[0] ? line + 1 : line;
}


static void
print_multi_line_string_diff(
    char const *str1_expression,
    char const *str1,
    char const *str2_expression,
    char const *str2,
    FILE *out
) {
    fprintf(
        out, "    `%s` expected to equal `%s`\n",
        str1_expression, str2_expression
    );
    fprintf(out, "\n");
    int str1_lines = count_lines(str1);
    int str2_lines = count_lines(str2);
    fprintf(out, "    str1 (%s): %d lines\n", str1_expression, str1_lines);
    fprintf(out, "    str2 (%s): %d lines\n", str2_expression, str2_lines);
    fprintf(out, "\n");
    print_str_diff(out, str1, str2);
    fprintf(out, "\n");
}


static void
print_single_line_string_diff(
    char const *str1_expression,
    char const *str1,
    char const *str2_expression,
    char const *str2,
    FILE *out
) {
    fprintf(
        out, "    `%s` expected to equal `%s`\n",
        str1_expression, str2_expression
    );
    fprintf(out, "\n");
    print_str_expression(out, "str1", str1_expression, str1);
    print_str_expression(out, "str2", str2_expression, str2);
}


void
check_false(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    enum check_type type,
    FILE *out
) {
    if (false == value) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(out, "    `%s` expected to be false\n", expression);
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


void
check_int_eq(
    char const *file,
    int line,
    char const *function,
    char const *int1_expression,
    int int1,
    char const *int2_expression,
    int int2,
    enum check_type type,
    FILE *out
) {
    if (int1 == int2) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(
        out, "    `%s` expected to equal `%s`\n",
        int1_expression, int2_expression
    );
    fprintf(out, "\n");
    print_int_expression(out, "int1", int1_expression, int1);
    print_int_expression(out, "int2", int2_expression, int2);
    if (check_type_assert == type) exit(EXIT_FAILURE);

}


void
check_not_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    enum check_type type,
    FILE *out
) {
    if (NULL != value) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(out, "    `%s` expected to be non-NULL\n", expression);
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


void
check_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    enum check_type type,
    FILE *out
) {
    if (NULL == value) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(out, "    `%s` expected to be NULL\n", expression);
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


void
check_str_contains(
    char const *file,
    int line,
    char const *function,
    char const *str_expression,
    char const *str,
    char const *substr_expression,
    char const *substr,
    enum check_type type,
    FILE *out
) {
    if (str_contains(str, substr)) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(
        out, "    `%s` expected to contain `%s`\n",
        str_expression, substr_expression
    );
    fprintf(out, "\n");
    print_str_expression(out, "str", str_expression, str);
    print_str_expression(out, "substr", substr_expression, substr);
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


void
check_str_eq(
    char const *file,
    int line,
    char const *function,
    char const *str1_expression,
    char const *str1,
    char const *str2_expression,
    char const *str2,
    enum check_type type,
    FILE *out
) {
    if (str_eq(str1, str2)) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    if (is_str_multi_line(str1) || is_str_multi_line(str2)) {
        print_multi_line_string_diff(
                str1_expression, str1, str2_expression, str2, out);
    } else {
        print_single_line_string_diff(
                str1_expression, str1, str2_expression, str2, out);
    }
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


void
check_true(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    enum check_type type,
    FILE *out
) {
    if (true == value) return;
    fprintf(out, "%s:%d: in %s():\n", file, line, function);
    fprintf(out, "    `%s` expected to be true\n", expression);
    if (check_type_assert == type) exit(EXIT_FAILURE);
}


FILE *
open_buffer(char buffer[], size_t buffer_size)
{
    memset(buffer, 0, buffer_size);
    return fmemopen(buffer, buffer_size, "w+");
}


void
print_int_expression(
    FILE *out, char const *parameter, char const *expression, int value
) {
    char buffer[21]; // holds INT64_MIN and INT64_MAX as decimal strings
    snprintf(buffer, sizeof buffer, "%d", value);
    if (str_eq(expression, buffer)) {
        fprintf(out, "    %s: %s\n", parameter, expression);
    } else {
        fprintf(out, "    %s (%s): %d\n", parameter, expression, value);
    }
}


void
print_str_diff(FILE *out, char const *str1, char const *str2)
{
    size_t len1;
    size_t len2;
    do {
        len1 = first_line_length(str1);
        len2 = first_line_length(str2);
        if (is_line_eq(str1, len1, str2, len2)) {
            fprintf(out, "====== \"%.*s\"\n", (int)len1, str1);
        } else {
            fprintf(out, "str1 < \"%.*s\"\n", (int)len1, str1);
            fprintf(out, "str2 > \"%.*s\"\n", (int)len2, str2);
        }
        str1 = next_line(str1, len1);
        str2 = next_line(str2, len2);
    } while (str1[0] || str2[0]);
}


void
print_str_expression(
    FILE *out, char const *parameter, char const *expression, char const *value
) {
    if (!value) {
        fprintf(out, "    %s (%s): NULL\n", parameter, expression);
    } else if (is_str_literal(expression)) {
        fprintf(out, "    %s: %s\n", parameter, expression);
    } else {
        fprintf(out, "    %s (%s): \"%s\"\n", parameter, expression, value);
    }
}


bool
str_contains(char const *str, char const *substring)
{
    if (NULL == str || NULL == substring) return false;
    return NULL != strstr(str, substring);
}


bool
str_eq(char const *s1, char const *s2)
{
    if (s1 == s2) return true;
    if (NULL == s1 || NULL == s2) return false;
    return 0 == strcmp(s1, s2);
}
