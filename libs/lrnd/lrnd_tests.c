#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "lrnd.h"


static void
test_free_for_global_lrnd(void)
{
    lrnd_free(lrnd_best);

#ifdef LRND_HAS_ARC4RANDOM
    lrnd_free(lrnd_arc4random);
#endif

    lrnd_free(lrnd_lrand48);
    lrnd_free(lrnd_rand);
    lrnd_free(lrnd_random);
}


static void
test_next_for_global_lrnd(void)
{
    uint32_t value = lrnd_next(lrnd_best);
    assert(value >= 0);
    assert(value <= lrnd_best->max);
}


#ifdef LRND_HAS_ARC4RANDOM

static void
test_next_for_arc4random(void)
{
    uint32_t value = lrnd_next(lrnd_arc4random);
    assert(value >= 0);
    assert(value <= lrnd_arc4random->max);
    assert(0xffffffff == lrnd_arc4random->max);
}

#endif


static void
test_next_for_lrand48(void)
{
    uint32_t value = lrnd_next(lrnd_lrand48);
    assert(value >= 0);
    assert(value <= lrnd_lrand48->max);
    assert(0x7fffffff == lrnd_lrand48->max);
}


static void
test_next_for_rand(void)
{
    uint32_t value = lrnd_next(lrnd_rand);
    assert(value >= 0);
    assert(value <= lrnd_rand->max);
    assert(RAND_MAX == lrnd_rand->max);
}


static void
test_next_for_random(void)
{
    uint32_t value = lrnd_next(lrnd_random);
    assert(value >= 0);
    assert(value <= lrnd_random->max);
    assert(0x7fffffff == lrnd_random->max);
}


static void
test_next_for_nrand48(void)
{
    unsigned short seed[] = {1, 2, 3};
    struct lrnd *nrand48 = lrnd_alloc_nrand48(seed);
    assert(nrand48);

    uint32_t value = lrnd_next(nrand48);
    assert(value >= 0);
    assert(value <= nrand48->max);

    lrnd_free(nrand48);
}


static void
test_next_for_poor(void)
{
    struct lrnd *poor = lrnd_alloc_poor(12345);
    assert(poor);
    
    uint32_t value = lrnd_next(poor);
    assert(value >= 0);
    assert(value <= RAND_MAX);

    lrnd_free(poor);
}


static void
test_next_for_rand_r(void)
{
    struct lrnd *rand_r = lrnd_alloc_rand_r(12345);
    assert(rand_r);

    uint32_t value = lrnd_next(rand_r);
    assert(value >= 0);
    assert(value <= rand_r->max);
    assert(RAND_MAX == rand_r->max);

    lrnd_free(rand_r);
}


#ifdef LRND_HAS_RANDOM_R

static void
test_next_for_random_r(void)
{
    struct lrnd *random_r = lrnd_alloc_random_r(256, 12345);
    assert(random_r);

    uint32_t value = lrnd_next(random_r);
    assert(value >= 0);
    assert(value <= random_r->max);
    assert(0x7fffffff == random_r->max);

    lrnd_free(random_r);
}

#endif


static void
test_next_for_fake_fixed(void)
{
    struct lrnd *fake_fixed = lrnd_alloc_fake_fixed(17);
    assert(fake_fixed);
    assert(UINT32_MAX == fake_fixed->max);

    for (int i = 0; i < 10; ++i) {
        uint32_t value = lrnd_next(fake_fixed);
        assert(value == 17);
    }

    lrnd_free(fake_fixed);
}


static void
test_next_for_fake_sequence(void)
{
    uint32_t sequence[] = {7, 79, 16, 36, 15};
    int count = sizeof sequence / sizeof sequence[0];
    struct lrnd *fake_sequence = lrnd_alloc_fake_sequence(sequence, count);
    assert(fake_sequence);
    assert(UINT32_MAX == fake_sequence->max);

    for (int i = 0; i < count; ++i) {
        uint32_t value = lrnd_next(fake_sequence);
        assert(sequence[i] == value);
    }

    uint32_t value = lrnd_next(fake_sequence);
    assert(sequence[0] == value);

    lrnd_free(fake_sequence);
}


static void
test_next_for_fake_start_step(void)
{
    struct lrnd *fake_stepping = lrnd_alloc_fake_start_step(100, 3);
    assert(fake_stepping);
    assert(UINT32_MAX == fake_stepping->max);

    uint32_t value = lrnd_next(fake_stepping);
    assert(100 == value);

    value = lrnd_next(fake_stepping);
    assert(103 == value);

    lrnd_free(fake_stepping);
}


static void
test_next_for_fake_start_step_when_descending_wraps_around(void)
{
    struct lrnd *fake_stepping = lrnd_alloc_fake_start_step(2, -2);
    assert(fake_stepping);
    assert(UINT32_MAX == fake_stepping->max);

    uint32_t value = lrnd_next(fake_stepping);
    assert(2 == value);

    value = lrnd_next(fake_stepping);
    assert(0 == value);

    value = lrnd_next(fake_stepping);
    assert(UINT32_MAX - 1 == value);

    lrnd_free(fake_stepping);
}


static void
test_next_for_fake_start_step_for_zero_step(void)
{
    struct lrnd *fake_stepping = lrnd_alloc_fake_start_step(100, 0);
    assert(fake_stepping);
    assert(UINT32_MAX == fake_stepping->max);

    uint32_t value = lrnd_next(fake_stepping);
    assert(100 == value);

    value = lrnd_next(fake_stepping);
    assert(100 == value);

    value = lrnd_next(fake_stepping);
    assert(100 == value);

    lrnd_free(fake_stepping);
}


static void
test_next_uniform(void)
{
    for (int i = 0; i < 100; ++i) {
        uint32_t value = lrnd_next_uniform(lrnd_arc4random, 5);
        assert(value >= 0);
        assert(value <= 5);
    }
}


static void
test_next_uniform_when_range_max_equals_max(void)
{
    struct lrnd *poor = lrnd_alloc_poor(12345);
    errno = 0;
    uint32_t value = lrnd_next_uniform(poor, 0x7fff); // range [0, 0x8000)
    assert(value >= 0);
    assert(value <= 0x7fff);
    assert( ! errno);
    lrnd_free(poor);
}


static void
test_next_uniform_range(void)
{
    for (int i = 0; i < 100; ++i) {
        uint32_t value = lrnd_next_uniform_range(lrnd_arc4random, 1, 6);
        assert(value >= 1);
        assert(value <= 6);
    }
}


static bool
is_array_eq(int const *a, int const *b, int count)
{
    for (int i = 0; i < count; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}


static void
test_shuffle(void)
{
    int sorted[] = {1, 2, 3, 4, 5};
    int items[] = {1, 2, 3, 4, 5};
    int count = sizeof items / sizeof items[0];
    lrnd_shuffle(lrnd_best, items, count, sizeof items[0]);

    assert( ! is_array_eq(items, sorted, count));
}


static void
test_max_multiple(void)
{
    // for generator [0, 20)

    uint32_t m = lrnd_max_multiple(19, 2); // range [0, 3)
    assert(17 == m);

    m = lrnd_max_multiple(19, 3); // range [0, 4)
    assert(19 == m);

    m = lrnd_max_multiple(19, 4); // range [0, 5)
    assert(19 == m);

    m = lrnd_max_multiple(19, 5); // range [0, 6)
    assert(17 == m);

    m = lrnd_max_multiple(19, 6); // range [0, 7)
    assert(13 == m);

    m = lrnd_max_multiple(19, 7); // range [0, 8)
    assert(15 == m);
}


int
main(int argc, char *argv[])
{
    test_free_for_global_lrnd();

    test_next_for_global_lrnd();
#ifdef LRND_HAS_ARC4RANDOM
    test_next_for_arc4random();
#endif
    test_next_for_lrand48();
    test_next_for_rand();
    test_next_for_random();

    test_next_for_nrand48();
    test_next_for_poor();
    test_next_for_rand_r();
#ifdef LRND_HAS_RANDOM_R
    test_next_for_random_r();
#endif

    test_next_for_fake_fixed();

    test_next_for_fake_sequence();

    test_next_for_fake_start_step();
    test_next_for_fake_start_step_when_descending_wraps_around();
    test_next_for_fake_start_step_for_zero_step();

    test_next_uniform();
    test_next_uniform_when_range_max_equals_max();

    test_next_uniform_range();

    test_shuffle();

    test_max_multiple();

    return EXIT_SUCCESS;
}
