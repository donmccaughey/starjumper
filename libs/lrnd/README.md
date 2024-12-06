# lrnd

A C99 library for generating random numbers.

[![builds.sr.ht status](https://builds.sr.ht/~donmcc/lrnd.svg)](https://builds.sr.ht/~donmcc/lrnd?)


## Overview

The **lrnd** library contains functions for generating random numbers using
pluggable random number generators.


## Structures

The `lrnd` structure exposes a common interface to different random number 
generators.

    struct lrnd {
        uint32_t const max;
        void (*free)(struct lrnd *lrnd);
        uint32_t (*next)(struct lrnd *lrnd);
    };

The `max` field contains the _inclusive_ maximum value that the generator
can produce.  The `free` field points to a function that releases any resources
this generator uses; it may be `NULL` if no clean up is needed.  The `next`
field points to a function that generates the next random value for this
generator.


## Global Variables

These variables are singleton instances of generators that wrap functions that
maintain their own global state.

    extern struct lrnd *const lrnd_best;

The "best" global generator available on the system.  Points to 
`lrnd_arc4random` when [`arc4random(3)`][31] is available and `lrnd_lrand48`
otherwise.

    extern struct lrnd *const lrnd_arc4random;

A global generator that uses [`arc4random(3)`][31].

    extern struct lrnd *const lrnd_lrand48;

A global generator that uses [`lrand48(3)`][32].  Seed by calling
[`srand48()`][32] or [`seed48()`][32] before first use.

    extern struct lrnd *const lrnd_rand;

A global generator that uses [`rand(3)`][33].  Seed by calling [`srand()`][33]
before first use.

    extern struct lrnd *const lrnd_random;

A global generator that uses [`random(3)`][34].  Seed by calling [`srandom`][34]
before first use.

[31]: https://man.bsd.lv/arc4random
[32]: https://pubs.opengroup.org/onlinepubs/9799919799/functions/drand48.html
[33]: https://pubs.opengroup.org/onlinepubs/9799919799/functions/rand.html#
[34]: https://pubs.opengroup.org/onlinepubs/9799919799/functions/random.html


## Allocators

These functions allocate and initialize instances of different generators that
have local state.

    struct lrnd *
    lrnd_alloc_nrand48(unsigned short seed[3]);

Allocate a generator that uses [`nrand48(3)`][32] with the given `seed` value.

    struct lrnd *
    lrnd_alloc_poor(uint32_t seed);

Allocate a poor random number generator.  This returns a poor PRNG that might be
useful for testing or when a consistent pseudo-random sequence is needed across
different systems.  The algorithm is from the `myrand()` example in the POSIX
documentation for [`rand(3)`][33].

    struct lrnd *
    lrnd_alloc_rand_r(unsigned int seed);

Allocate a generator that uses [`rand_r(3)`][41] initialized with the given
`seed` value.

    struct lrnd *
    lrnd_alloc_random_r(size_t state_array_size, unsigned int seed);

Allocate a generator that uses [`random_r(3)`][42] with a _state array_ of the
given size, initialized with the given `seed` value.  Use `state_array_size`
values of 8, 32, 64, 128 or 256, where the larger sizes produce better random
numbers.  Available when `LRND_HAS_RANDOM_R` is defined.

[41]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/rand_r.html
[42]: https://man.bsd.lv/Linux-5.13/random_r.3


## Fake Generators

These functions allocate and initialize fake generators that return
predictable sequences of values.  Fake generators are useful for writing
deterministic tests of code that uses random numbers.

    struct lrnd *
    lrnd_alloc_fake_fixed(uint32_t value);

Allocate a generator that always returns the given value.

    struct lrnd *
    lrnd_alloc_fake_sequence(uint32_t const values[], int count);

Allocate a generator that returns values from the given sequence.  Repeats from
the sequence from the start after reaching the end.

    struct lrnd *
    lrnd_alloc_fake_start_step(uint32_t initial_value, int32_t step);

Allocate a generator that returns a sequence of ascending (or descending) values
starting at the given `value`.


## Methods

These functions provide the common interface to the random number generators.

    void
    lrnd_free(struct lrnd *lrnd);

Clean up and deallocate a generator that was created by one of the
`lrnd_alloc_()` functions.

    uint32_t
    lrnd_next(struct lrnd *lrnd);

Generate the next random value for the given generator.  Returns `UINT32_MAX`
and sets `errno` if `lrnd` is `NULL`.

    uint32_t
    lrnd_next_uniform(struct lrnd *lrnd, uint32_t range_max);

Generates the next random value in the range [0, range_max] for the given
generator, accounting for [modulo bias][50].  Returns `range_max` and sets
`errno` if `lrnd` is `NULL`.

    uint32_t
    lrnd_next_uniform_range(struct lrnd *lrnd, uint32_t range_min, uint32_t range_max);

Generates the next random value in the range [range_min, range_max] for the
given generator, accounting for [modulo bias][50].  Returns `range_max` and sets
`errno` if `lrnd` is `NULL` or if `range_min` equals or exceeds `range_max`.

    void
    lrnd_shuffle(struct lrnd *lrnd, void *items, int item_count, size_t item_size);

Shuffle an array using the [Fisher-Yates][51] algorithm.

[50]: https://en.wikipedia.org/wiki/Random_number_generation#Uniform_distributions
[51]: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle

## Ancillary Functions

    uint32_t
    lrnd_max_multiple(uint32_t generator_max, uint32_t range_max);

The largest value less than or equal to `generator_max` that is an exact
multiple of the range [0, range_max].  Used in calculating [modulo bias][50]
when generating a uniform value distribution.


## Preprocessor Definitions

These macros control features of **lrnd** that may not be available on all
systems.  The CMake build system will test for and set these automatically.

- `LRND_HAS_ARC4RANDOM` - defined if the `arc4random(3)` function is available.
- `LRND_HAS_RANDOM_R` - defined if the `random_r(3)` function is available.

These macros control features of **lrnd** that may be located in different
headers on different systems.  The CMake build system will test for and set
these automatically. 

- `LRND_HAS_GETENTROPY_IN_UNISTD` - defined if the `getentropy()` function is
    available and declared in `<unistd.h>`.
- `LRND_HAS_GETENTROPY_IN_SYS_RANDOM` - defined if the `getentropy()` function
    is available and declared in `<sys/random.h>` (macOS).

If neither of the `LRND_HAS_GETENTROPY` macros is defined, **lrnd** will include
a `getentropy()` polyfill that reads from `/dev/urandom`.


## Dependencies

Building requires [CMake 3.15][60] or later.

[60]: https://cmake.org


## Building from Repository Source

Clone the repository, generate the build system and build.

    git clone https://git.sr.ht/~donmcc/lrnd
    cd lrnd
    cmake -S . -B tmp
    cmake --build tmp
    cmake --install tmp

To run tests:

    cmake --build tmp --target all test

### Build Options

To build with the [AddressSanitizer][71] enabled, set the `ADDRESS_SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Set the `BUILD_TESTING` option to `OFF` to disable building tests.  (It's `ON`
by default.)

    cmake -S . -B tmp -DBUILD_TESTING=OFF

Set the `COVERAGE` option to `ON` to generate coverage files.

    cmake -S . -B tmp -DCOVERAGE=ON

Set the `WALL` option to `ON` turns on [additional warnings][72] using the 
`-Wall` compiler flag and also treats warnings as errors.  `WALL` is off by
default but is recommended for development and integration builds.

    cmake -S . -B tmp -DWALL=ON

[71]: https://clang.llvm.org/docs/LeakSanitizer.html
[72]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


## License
`lrnd` is made available under a BSD-style license; see the [LICENSE][101] file
for details.

[101]: ./LICENSE
