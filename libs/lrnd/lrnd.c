#include "lrnd.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#if defined(LRND_HAS_GETENTROPY_IN_UNISTD)
#include <unistd.h>
#elif defined(LRND_HAS_GETENTROPY_IN_SYS_RANDOM)
#include <sys/random.h>
#else

#include <fcntl.h>
#include <unistd.h>

int
getentropy(void *bytes, size_t size)
{
    assert(bytes);
    assert(size);
    assert(size <= 256);

    if ( ! bytes) {
        errno = EINVAL;
        return -1;
    }
    if ( ! size || size > 256) {
        errno = EIO;
        return -1;
    }

    int fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    if (fd == -1) return -1;

    size_t total_read = 0;
    while (total_read < size) {
        ssize_t bytes_read = read(fd, bytes + total_read, size - total_read);

        if (bytes_read == -1) {
            if (errno == EINTR) continue;
            close(fd);
            return -1;
        }

        if ( ! bytes_read) {
            close(fd);
            errno = EIO;
            return -1;
        }

        total_read += bytes_read;
    }

    close(fd);
    return 0;
}

#endif


#ifdef LRND_HAS_ARC4RANDOM

static uint32_t
arc4random_next(struct lrnd *lrnd)
{
    assert(lrnd);
    return arc4random();
}

struct lrnd *const lrnd_arc4random = &((struct lrnd) {
    .max=UINT32_MAX,
    .next=arc4random_next,
});

#endif


struct lrnd48_state {
    struct lrnd lrnd;
    uint16_t reseed_counter;
};

static uint32_t
lrand48_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct lrnd48_state *lrnd48_state = (struct lrnd48_state *)lrnd;
    if ( ! lrnd48_state->reseed_counter) {
        unsigned short seed[3];
        int error = getentropy(seed, sizeof seed);
        assert( ! error);
        if (error) abort();
        seed48(seed);
    }
    ++lrnd48_state->reseed_counter;
    return lrand48();
}

static struct lrnd48_state lrnd48_state = {
    .lrnd.max=0x7fffffff,
    .lrnd.next=lrand48_next,
    .reseed_counter=0,
};

struct lrnd *const lrnd_lrand48 = (struct lrnd *)&lrnd48_state;


struct rand_state {
    struct lrnd lrnd;
    uint16_t reseed_counter;
};

static uint32_t
rand_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct rand_state *rand_state = (struct rand_state *)lrnd;
    if ( ! rand_state->reseed_counter) {
        unsigned seed;
        int error = getentropy(&seed, sizeof seed);
        assert( ! error);
        if (error) abort();
        srand(seed);
    }
    ++rand_state->reseed_counter;
    return rand();
}

static struct rand_state rand_state = {
    .lrnd.max=RAND_MAX,
    .lrnd.next=rand_next,
    .reseed_counter=0,
};

struct lrnd *const lrnd_rand = (struct lrnd *)&rand_state;


struct random_state {
    struct lrnd lrnd;
    uint16_t reseed_counter;
};

static uint32_t
random_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct random_state *random_state = (struct random_state *)lrnd;
    if ( ! random_state->reseed_counter) {
        unsigned seed;
        int error = getentropy(&seed, sizeof seed);
        assert( ! error);
        if (error) abort();
        srandom(seed);
    }
    ++random_state->reseed_counter;
    return random();
}

static struct random_state random_state = {
    .lrnd.max=0x7fffffff,
    .lrnd.next=random_next,
    .reseed_counter=0,
};

struct lrnd *const lrnd_random = (struct lrnd *)&random_state;


#ifdef LRND_HAS_ARC4RANDOM
struct lrnd *const lrnd_best = lrnd_arc4random;
#else
struct lrnd *const lrnd_best = lrnd_lrand48;
#endif


static bool
is_global_generator(struct lrnd *lrnd)
{
    if (lrnd_best == lrnd) return true;
#ifdef LRND_HAS_ARC4RANDOM
    if (lrnd_arc4random == lrnd) return true;
#endif
    if (lrnd_lrand48 == lrnd) return true;
    if (lrnd_rand == lrnd) return true;
    if (lrnd_random == lrnd) return true;
    return false;
}


struct nrand48_state {
    struct lrnd lrnd;
    unsigned short seed[3];
};

static uint32_t
nrand48_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct nrand48_state *nrand48_state = (struct nrand48_state *)lrnd;
    return nrand48(nrand48_state->seed);
}

struct lrnd *
lrnd_alloc_nrand48(unsigned short seed[3])
{
    assert(seed);

    struct nrand48_state *nrand48_state = calloc(1, sizeof(struct nrand48_state));
    if ( ! nrand48_state) abort();

    nrand48_state->lrnd.max = 0x7fffffff;
    nrand48_state->lrnd.next = nrand48_next;
    memcpy(nrand48_state->seed, seed, sizeof nrand48_state->seed);

    return (struct lrnd *)nrand48_state;
}


struct poor {
    struct lrnd lrnd;
    uint32_t state;
};

static uint32_t
poor_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct poor *poor = (struct poor *)lrnd;

    uint32_t const factor = 1103515245;
    uint32_t const addend = 12345;
    poor->state = poor->state * factor + addend;

    uint32_t const drop_two_lower_bytes = 0x10000;
    uint16_t two_upper_bytes = poor->state / drop_two_lower_bytes;

    uint32_t const exclusive_upper_bound = poor->lrnd.max + 1;
    return two_upper_bytes % exclusive_upper_bound;
}

struct lrnd *
lrnd_alloc_poor(uint32_t seed)
{
    struct poor* poor = calloc(1, sizeof(struct poor));
    if ( ! poor) abort();

    poor->lrnd.max = 0x7fff;
    poor->lrnd.next = poor_next;
    poor->state = seed;

    return (struct lrnd *)poor;
}


struct rand_r_state {
    struct lrnd lrnd;
    unsigned int seed;
};

static uint32_t
rand_r_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct rand_r_state *rand_r_state = (struct rand_r_state *)lrnd;
    return rand_r(&rand_r_state->seed);
}

struct lrnd *
lrnd_alloc_rand_r(unsigned int seed)
{
    struct rand_r_state *rand_r_state = calloc(1, sizeof(struct rand_r_state));
    if ( ! rand_r_state) abort();

    rand_r_state->lrnd.max = RAND_MAX;
    rand_r_state->lrnd.next = rand_r_next;
    rand_r_state->seed = seed;

    return (struct lrnd *)rand_r_state;
}


#ifdef LRND_HAS_RANDOM_R

struct random_r_state {
    struct lrnd lrnd;
    struct random_data random_data;
    char *state_array;
};

static void
random_r_free(struct lrnd *lrnd)
{
    assert(lrnd);
    struct random_r_state *random_r_state = (struct random_r_state *)lrnd;
    free(random_r_state->state_array);
}

static uint32_t
random_r_next(struct lrnd *lrnd) {
    assert(lrnd);
    struct random_r_state *random_r_state = (struct random_r_state *)lrnd;
    int32_t signed_value_out = 0;
    int error = random_r(&random_r_state->random_data, &signed_value_out);
    assert( ! error);
    if (error) abort();
    return signed_value_out;
}

struct lrnd *
lrnd_alloc_random_r(size_t state_array_size, unsigned int seed)
{
    assert(state_array_size >= 8);
    if (state_array_size < 8) state_array_size = 8;

    struct random_r_state *random_r_state
            = calloc(1, sizeof(struct random_r_state));
    if ( ! random_r_state) abort();

    random_r_state->lrnd.max = 0x7fffffff;
    random_r_state->lrnd.free = random_r_free;
    random_r_state->lrnd.next = random_r_next;

    random_r_state->state_array = malloc(state_array_size);
    if ( ! random_r_state->state_array) abort();
    
    int error = initstate_r(seed, random_r_state->state_array,
            state_array_size, &random_r_state->random_data);
    assert( ! error);
    if (error) abort();

    return (struct lrnd *)random_r_state;
}

#endif


struct lrnd *
lrnd_alloc_fake_fixed(uint32_t value)
{
    return lrnd_alloc_fake_sequence(&value, 1);
}


struct sequence_state {
    struct lrnd lrnd;
    uint32_t *values;
    size_t count;
    size_t i;
};

static void
sequence_free(struct lrnd *lrnd)
{
    assert(lrnd);
    struct sequence_state *sequence_state = (struct sequence_state *)lrnd;
    free(sequence_state->values);
}

static uint32_t
sequence_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct sequence_state *sequence_state = (struct sequence_state *)lrnd;
    uint32_t value = sequence_state->values[sequence_state->i];
    ++sequence_state->i;
    if (sequence_state->i >= sequence_state->count) sequence_state->i = 0;
    return value;
}

struct lrnd *
lrnd_alloc_fake_sequence(uint32_t const values[], int count)
{
    assert(values);
    assert(count > 0);

    uint32_t error_values[] = {UINT32_MAX};
    if ( ! values || count < 1) {
        errno = EINVAL;
        values = error_values;
        count = 1;
    }

    struct sequence_state *sequence_state = calloc(1, sizeof(struct sequence_state));
    if ( ! sequence_state) abort();

    sequence_state->lrnd.max = UINT32_MAX;
    sequence_state->lrnd.free = sequence_free;
    sequence_state->lrnd.next = sequence_next;

    size_t size = sizeof values[0] * count;
    sequence_state->values = malloc(size);
    if ( ! sequence_state->values) abort();
    memcpy(sequence_state->values, values, size);

    sequence_state->count = count;

    return (struct lrnd *)sequence_state;
}


struct start_step_state {
    struct lrnd lrnd;
    uint32_t value;
    int32_t step;
};

static uint32_t
start_step_next(struct lrnd *lrnd)
{
    assert(lrnd);
    struct start_step_state *start_step_state = (struct start_step_state *)lrnd;
    uint32_t value = start_step_state->value;
    start_step_state->value += start_step_state->step;
    return value;
}

struct lrnd *
lrnd_alloc_fake_start_step(uint32_t start, int32_t step)
{
    struct start_step_state *start_step_state = calloc(1, sizeof(struct start_step_state));
    if ( ! start_step_state) abort();

    start_step_state->lrnd.max = UINT32_MAX;
    start_step_state->lrnd.next = start_step_next;
    start_step_state->value = start;
    start_step_state->step = step;

    return (struct lrnd *)start_step_state;
}


void
lrnd_free(struct lrnd *lrnd)
{
    if (is_global_generator(lrnd)) return;
    if (lrnd && lrnd->free) lrnd->free(lrnd);
    free(lrnd);
}


uint32_t
lrnd_next(struct lrnd *lrnd)
{
    assert(lrnd);
    assert(lrnd->next);

    if ( ! lrnd) {
        errno = EINVAL;
        return UINT32_MAX;
    }
    if ( ! lrnd->next) {
        errno = EINVAL;
        return lrnd->max;
    }

    return lrnd->next(lrnd);
}


uint32_t
lrnd_next_uniform(struct lrnd *lrnd, uint32_t range_max)
{
    assert(lrnd);
    assert(lrnd->next);
    assert(range_max);
    assert(range_max <= lrnd->max);

    if ( ! lrnd || ! lrnd->next) {
        errno = EINVAL;
        return range_max;
    }
    if ( ! range_max || range_max > lrnd->max) {
        errno = EDOM;
        return range_max;
    }

    uint32_t const range_min = 0;
    uint64_t modulus = (uint64_t)range_max - range_min + 1;

    uint32_t const max_multiple = lrnd_max_multiple(lrnd->max, range_max);
    uint32_t dividend;
    do {
        dividend = lrnd_next(lrnd);
    } while (dividend > max_multiple);
    return dividend % modulus;
}


uint32_t
lrnd_next_uniform_range(struct lrnd *lrnd, uint32_t range_min, uint32_t range_max)
{
    assert(range_min < range_max);

    if (range_min >= range_max) {
        errno = EDOM;
        return range_max;
    }

    uint32_t offset_range_max = range_max - range_min;
    return range_min + lrnd_next_uniform(lrnd, offset_range_max);
}


void
lrnd_shuffle(struct lrnd *lrnd, void *items, int item_count, size_t item_size)
{
    assert(lrnd);
    assert(items || 0 == item_count);
    assert(item_count >= 0 && item_count <= lrnd->max);
    assert(item_size);

    if ( ! lrnd) {
        errno = EINVAL;
        return;
    }
    if ( ! items && item_count) {
        errno = EINVAL;
        return;
    }
    if (item_count < 0 || item_count > lrnd->max) {
        errno = EINVAL;
        return;
    }
    if ( ! item_size) {
        errno = EINVAL;
        return;
    }

    char temp[item_size];
    for (int i = item_count - 1; i > 0; --i) {
        int j = (int)lrnd_next_uniform(lrnd, i);
        if (i != j) {
            void *item_i = items + (i * item_size);
            void *item_j = items + (j * item_size);
            memcpy(temp, item_i, item_size);
            memcpy(item_i, item_j, item_size);
            memcpy(item_j, temp, item_size);
        }
    }
}


uint32_t
lrnd_max_multiple(uint32_t generator_max, uint32_t range_max)
{
    assert(generator_max);
    assert(range_max);

    if ( ! generator_max || ! range_max) {
        errno = EDOM;
        return generator_max;
    }

    uint32_t const range_min = 0;
    uint64_t modulus = (uint64_t)range_max - range_min + 1;

    uint32_t const lrnd_min = 0;
    uint64_t dividend = (uint64_t)generator_max - lrnd_min + 1;

    uint32_t remainder = dividend % modulus;
    return generator_max - remainder;
}
