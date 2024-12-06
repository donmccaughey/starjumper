#ifndef LRND_H_INCLUDED
#define LRND_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


struct lrnd;


typedef void (lrnd_free_fn)(struct lrnd *lrnd);

typedef uint32_t (lrnd_next_fn)(struct lrnd *lrnd);


struct lrnd {
    uint32_t max;
    lrnd_free_fn *free;
    lrnd_next_fn *next;
};


/* The best generator available on the system. */
extern struct lrnd *const lrnd_best;

/* A global generator that uses arc4random(3). */
#ifdef LRND_HAS_ARC4RANDOM
extern struct lrnd *const lrnd_arc4random;
#endif

/* A global generator that uses lrand48(3). */
extern struct lrnd *const lrnd_lrand48;

/* A global generator that uses rand(3). */
extern struct lrnd *const lrnd_rand;

/* A global generator that uses random(3). */
extern struct lrnd *const lrnd_random;


/* Allocate a pseudo-random number generator that uses nrand48(3). */
struct lrnd *
lrnd_alloc_nrand48(unsigned short seed[3]);

/* Allocate a poor random number generator.

Create a poor PRNG with a limited range of [0, 32767] that might be useful for
testing or where you need a consistent sequence on different systems.

From the myrand() example in the POSIX documentation for rand():
https://pubs.opengroup.org/onlinepubs/9699919799/functions/rand.html */
struct lrnd *
lrnd_alloc_poor(uint32_t seed);

/* Allocate a pseudo-random number generator that uses rand_r(3). */
struct lrnd *
lrnd_alloc_rand_r(unsigned int seed);

/* Allocate a pseudo-random number generator that uses random_r(3). */
#ifdef LRND_HAS_RANDOM_R
struct lrnd *
lrnd_alloc_random_r(size_t state_array_size, unsigned int seed);
#endif


/* Allocate a fake generator that always returns the same value. */
struct lrnd *
lrnd_alloc_fake_fixed(uint32_t value);

/* Allocate a fake generator that returns values from a sequence.  The sequence
is copied. */
struct lrnd *
lrnd_alloc_fake_sequence(uint32_t const values[], int count);

/* Allocate a fake generator that returns ascending, descending or fixed
values. */
struct lrnd *
lrnd_alloc_fake_start_step(uint32_t start, int32_t step);


/* Free the generator.  Is a noop if called on any of the global generators. */
void
lrnd_free(struct lrnd *lrnd);

/* Get the next value from the generator in the range [0, lrnd->max]. */
uint32_t
lrnd_next(struct lrnd *lrnd);

/* Get the next value from the generator in the range [0, range_max].  Accounts
for modulo bias.  Sets `errno` to `EDOM` and returns `range_max` if `range_max`
is zero. */
uint32_t
lrnd_next_uniform(struct lrnd *lrnd, uint32_t range_max);

/* Get the next value from the generator in the range [range_min, range_max].
Accounts for modulo bias.  Sets `errno` to `EDOM` and returns `range_max` if
`range_min` equals or exceeds `range_max`. */
uint32_t
lrnd_next_uniform_range(struct lrnd *lrnd, uint32_t range_min, uint32_t range_max);

void
lrnd_shuffle(struct lrnd *lrnd, void *items, int item_count, size_t item_size);


/* The largest value less than or equal to `generator_max` that is an exact
multiple of the range [0, range_max].  Used to calculate modulo bias when
generating a uniform value distribution.  Sets `errno` to `EDOM` and returns
`generator_max` if `range_max` is zero. */
uint32_t
lrnd_max_multiple(uint32_t generator_max, uint32_t range_max);


#endif
