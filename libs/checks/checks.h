#ifndef CHECKS_H_INCLUDED
#define CHECKS_H_INCLUDED


#include <stdbool.h>
#include <stdio.h>


#define ASSERT_FALSE(expression) \
    check_false( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        check_type_assert, stderr \
    )


#define ASSERT_INT_EQ(int1, int2) \
    check_int_eq( \
        __FILE__, __LINE__, __func__, \
        #int1, (int1), #int2, (int2), \
        check_type_assert, stderr \
    )


#define ASSERT_TRUE(expression) \
    check_true( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        check_type_assert, stderr \
    )


#define ASSERT_NOT_NULL(expression) \
    check_not_null( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        check_type_assert, stderr \
    )


#define ASSERT_NULL(expression) \
    check_null( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        check_type_assert, stderr \
    )


#define ASSERT_STR_CONTAINS(str, substring) \
    check_str_contains( \
        __FILE__, __LINE__, __func__, \
        #str, (str), #substring, (substring), \
        check_type_assert, stderr \
    )


#define ASSERT_STR_EQ(str1, str2) \
    check_str_eq( \
        __FILE__, __LINE__, __func__, \
        #str1, (str1), #str2, (str2), \
        check_type_assert, stderr \
    )


enum check_type {
    check_type_assert,
    check_type_expect
};


void
check_false(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    enum check_type type,
    FILE *out
);

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
);

void
check_not_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    enum check_type type,
    FILE *out
);

void
check_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    enum check_type type,
    FILE *out
);

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
);

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
);

void
check_true(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    enum check_type type,
    FILE *out
);

FILE *
open_buffer(char buffer[], size_t buffer_size);

void
print_int_expression(
    FILE *out, char const *parameter, char const *expression, int value
);

void
print_str_diff(FILE *out, char const *str1, char const *str2);

void
print_str_expression(
    FILE *out, char const *parameter, char const *expression, char const *value
);

bool
str_contains(char const *str, char const *substring);

bool
str_eq(char const *s1, char const *s2);


#endif
