#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "lrnd.h"


int
getentropy(void *bytes, size_t size);


#ifdef LRND_HAS_RANDOM_R

static void
test_alloc_random_r_with_invalid_state_array_size(void)
{
    errno = 0;
    size_t invalid = 7;
    struct lrnd *random_r = lrnd_alloc_random_r(invalid, 12345);
    assert(random_r);

    uint32_t value = lrnd_next(random_r);
    assert(value >= 0);
    assert(value <= random_r->max);
    assert(0x7fffffff == random_r->max);

    lrnd_free(random_r);
}

#endif


static void
test_getentropy(void)
{
    bool buffer_changed = false;
    for (int i = 0; i < 100; ++i) {
        char buffer[4] = {0};
        int failed = getentropy(buffer, 1);
        assert( ! failed);
        assert( ! buffer[1]);
        assert( ! buffer[2]);
        assert( ! buffer[3]);
        if (buffer[0]) return;
    }
    assert(buffer_changed);
}


static void
test_getentropy_for_null_buffer(void)
{
    errno = 0;
    int failed = getentropy(NULL, 1);
    assert(failed);
    assert(EINVAL == errno);
}


static void
test_getentropy_for_invalid_size(void)
{
    char buffer[4] = {0};

    errno = 0;
    int failed = getentropy(buffer, 0);
    assert(failed);
    assert(EIO == errno);

    errno = 0;
    failed = getentropy(buffer, 257);
    assert(failed);
    assert(EIO == errno);
}


static void
test_next_for_fake_sequence_with_invalid_values(void)
{
    struct lrnd *fake_sequence = lrnd_alloc_fake_sequence(NULL, 5);
    assert(fake_sequence);
    assert(UINT32_MAX == fake_sequence->max);
    assert(EINVAL == errno);

    uint32_t value = lrnd_next(fake_sequence);
    assert(UINT32_MAX == value);

    value = lrnd_next(fake_sequence);
    assert(UINT32_MAX == value);

    lrnd_free(fake_sequence);
}


static void
test_next_for_fake_sequence_with_zero_count(void)
{
    uint32_t sequence[] = {7, 79, 16, 36, 15};
    int count = 0;
    struct lrnd *fake_sequence = lrnd_alloc_fake_sequence(sequence, count);
    assert(fake_sequence);
    assert(UINT32_MAX == fake_sequence->max);
    assert(EINVAL == errno);

    uint32_t value = lrnd_next(fake_sequence);
    assert(UINT32_MAX == value);

    value = lrnd_next(fake_sequence);
    assert(UINT32_MAX == value);

    lrnd_free(fake_sequence);
}


static void
test_next_with_invalid_lrnd(void)
{
    errno = 0;
    uint32_t value = lrnd_next(NULL);
    assert(UINT32_MAX == value);
    assert(EINVAL == errno);
}


static void
test_next_with_invalid_lrnd_next(void)
{
    struct lrnd lrnd = {
        .max = 0xffff,
        .next = NULL,
    };
    errno = 0;
    uint32_t value = lrnd_next(&lrnd);
    assert(lrnd.max == value);
    assert(EINVAL == errno);
}


static void
test_next_uniform_for_invalid_lrnd(void)
{
    errno = 0;
    uint32_t value = lrnd_next_uniform(NULL, 5); // range [0, 6)
    assert(5 == value);
    assert(EINVAL == errno);
}


static void
test_next_uniform_for_invalid_lrnd_next(void)
{
    struct lrnd lrnd = {
        .max = 0xffff,
        .next = NULL,
    };
    errno = 0;
    uint32_t value = lrnd_next_uniform(&lrnd, 5); // range [0, 6)
    assert(5 == value);
    assert(EINVAL == errno);
}


static void
test_next_uniform_for_invalid_range_max(void)
{
    struct lrnd *poor = lrnd_alloc_poor(12345);
    errno = 0;
    uint32_t value = lrnd_next_uniform(poor, 0); // range [0, 1)
    assert( ! value);
    assert(EDOM == errno);
    lrnd_free(poor);
}


static void
test_next_uniform_for_range_max_too_big(void)
{
    struct lrnd *poor = lrnd_alloc_poor(12345);
    errno = 0;
    uint32_t value = lrnd_next_uniform(poor, poor->max + 1); // range [0, 0x8001)
    assert(poor->max + 1 == value);
    assert(EDOM == errno);
    lrnd_free(poor);
}


static void
test_next_uniform_range_for_invalid_range_min(void)
{
    errno = 0;
    uint32_t value = lrnd_next_uniform_range(lrnd_best, 6, 6);
    assert(6 == value);
    assert(EDOM == errno);

    errno = 0;
    value = lrnd_next_uniform_range(lrnd_best, 6, 0);
    assert(0 == value);
    assert(EDOM == errno);
}


static void
test_shuffle_for_invalid_generator(void)
{
    int items[] = {1, 2, 3, 4, 5};
    int count = sizeof items / sizeof items[0];
    errno = 0;
    lrnd_shuffle(NULL, items, count, sizeof items[0]);
    assert(EINVAL == errno);
}


static void
test_shuffle_for_null_array_with_item_count(void)
{
    errno = 0;
    lrnd_shuffle(lrnd_best, NULL, 5, sizeof(int));
    assert(EINVAL == errno);
}


static void
test_shuffle_for_null_array_with_zero_item_count(void)
{
    errno = 0;
    lrnd_shuffle(lrnd_best, NULL, 0, sizeof(int));
    assert(0 == errno);
}


static void
test_shuffle_for_negative_item_count(void)
{
    int items[] = {1, 2, 3, 4, 5};
    int count = -1;
    errno = 0;
    lrnd_shuffle(NULL, items, count, sizeof items[0]);
    assert(EINVAL == errno);
}


static void
test_shuffle_when_item_count_exceeds_generator_max(void)
{
    struct lrnd *poor = lrnd_alloc_poor(12345);

    int count = 0xffff;
    assert(count > poor->max);
    int *items = calloc(count, sizeof(int));
    for (int i = 0; i < count; ++i) {
        items[i] = i;
    }

    errno = 0;
    lrnd_shuffle(poor, items, count, sizeof items[0]);
    assert(EINVAL == errno);

    free(items);
    lrnd_free(poor);
}


static void
test_shuffle_for_zero_item_size(void)
{
    int items[] = {1, 2, 3, 4, 5};
    int count = sizeof items / sizeof items[0];
    errno = 0;
    lrnd_shuffle(lrnd_best, items, count, 0);
    assert(EINVAL == errno);
}


static void
test_max_multiple_for_invalid_generator_max(void)
{
    errno = 0;
    uint32_t m = lrnd_max_multiple(0, 5); // range [0, 6)
    assert(0 == m);
    assert(EDOM == errno);
}


static void
test_max_multiple_for_invalid_range_max(void)
{
    errno = 0;
    uint32_t m = lrnd_max_multiple(19, 0); // range [0, 1)
    assert(19 == m);
    assert(EDOM == errno);
}


int
main(int argc, char *argv[])
{
#ifdef LRND_HAS_RANDOM_R
    test_alloc_random_r_with_invalid_state_array_size();
#endif

    test_getentropy();
    test_getentropy_for_null_buffer();
    test_getentropy_for_invalid_size();

    test_next_for_fake_sequence_with_invalid_values();
    test_next_for_fake_sequence_with_zero_count();

    test_next_with_invalid_lrnd();
    test_next_with_invalid_lrnd_next();

    test_next_uniform_for_invalid_lrnd();
    test_next_uniform_for_invalid_lrnd_next();
    test_next_uniform_for_invalid_range_max();
    test_next_uniform_for_range_max_too_big();

    test_next_uniform_range_for_invalid_range_min();

    test_shuffle_for_invalid_generator();
    test_shuffle_for_null_array_with_item_count();
    test_shuffle_for_null_array_with_zero_item_count();
    test_shuffle_for_negative_item_count();
    test_shuffle_when_item_count_exceeds_generator_max();
    test_shuffle_for_zero_item_size();

    test_max_multiple_for_invalid_generator_max();
    test_max_multiple_for_invalid_range_max();

    return EXIT_SUCCESS;
}
