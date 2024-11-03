#ifndef CHECKS_H_INCLUDED
#define CHECKS_H_INCLUDED


#include <stdbool.h>
#include <stdio.h>


#define check_false(expression) \
    cks_verify_false( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        stderr, cks_on_fail_halt \
    )


#define check_int_eq(int1, int2) \
    cks_verify_int_eq( \
        __FILE__, __LINE__, __func__, \
        #int1, (int1), #int2, (int2), \
        stderr, cks_on_fail_halt \
    )


#define check_not_null(expression) \
    cks_verify_not_null( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        stderr, cks_on_fail_halt \
    )


#define check_null(expression) \
    cks_verify_null( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        stderr, cks_on_fail_halt \
    )


#define check_str_contains(str, substring) \
    cks_verify_str_contains( \
        __FILE__, __LINE__, __func__, \
        #str, (str), #substring, (substring), \
        stderr, cks_on_fail_halt \
    )


#define check_str_eq(str1, str2) \
    cks_verify_str_eq( \
        __FILE__, __LINE__, __func__, \
        #str1, (str1), #str2, (str2), \
        stderr, cks_on_fail_halt \
    )


#define check_true(expression) \
    cks_verify_true( \
        __FILE__, __LINE__, __func__, \
        #expression, (expression), \
        stderr, cks_on_fail_halt \
    )


enum cks_on_fail {
    cks_on_fail_continue,
    cks_on_fail_halt,
};


void
cks_verify_false(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_int_eq(
    char const *file,
    int line,
    char const *function,
    char const *int1_expression,
    int int1,
    char const *int2_expression,
    int int2,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_not_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_null(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    void const *value,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_str_contains(
    char const *file,
    int line,
    char const *function,
    char const *str_expression,
    char const *str,
    char const *substr_expression,
    char const *substr,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_str_eq(
    char const *file,
    int line,
    char const *function,
    char const *str1_expression,
    char const *str1,
    char const *str2_expression,
    char const *str2,
    FILE *out,
    enum cks_on_fail on_fail
);

void
cks_verify_true(
    char const *file,
    int line,
    char const *function,
    char const *expression,
    bool value,
    FILE *out,
    enum cks_on_fail on_fail
);

bool
cks_is_substr(char const *str, char const *substring);

bool
cks_is_str_eq(char const *str1, char const *str2);

FILE *
cks_open_buffer(char buffer[], size_t buffer_size);

void
cks_print_int_expression(
    FILE *out, char const *parameter, char const *expression, int value
);

void
cks_print_str_diff(FILE *out, char const *str1, char const *str2);

void
cks_print_str_expression(
    FILE *out, char const *parameter, char const *expression, char const *value
);


#endif
