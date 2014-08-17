#include "sj_random.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sj_memory.h"


#define SJ_RANDOM_MAX (SJ_RANDOM_RANGE - 1)
#define SJ_RANDOM_MIN (0l)
#define SJ_RANDOM_RANGE (1l << 31)


static int
fake_next_value_in_range(void *data, long range);

static int
nrand48_next_value_in_range(void *data, long range);

static void
seed_nrand48_state(unsigned short state[3]);


static int
fake_next_value_in_range(void *data, long range)
{
  return (int) (range / 2);
}


static int
nrand48_next_value_in_range(void *data, long range)
{
  unsigned short *state = data;
  
  long modulo_bias_range = SJ_RANDOM_RANGE % range;
  long largest_multiple = SJ_RANDOM_MAX - modulo_bias_range;
  long next_value;
  do {
    next_value = nrand48(state);
  } while (next_value > largest_multiple);
  
  int value = (int) (next_value % range);
  assert(value >= 0);
  assert(value < range);
  
  return value;
}


static void
seed_nrand48_state(unsigned short state[3])
{
  char const dev_random_path[] = "/dev/random";
  FILE *file = fopen(dev_random_path, "rb");
  if ( ! file) {
    if (errno) fprintf(stderr, "ERROR: (%i) %s\n", errno, strerror(errno));
    fprintf(stderr, "ERROR: unable to open \"%s\"\n", dev_random_path);
    exit(EXIT_FAILURE);
  }
  
  size_t element_count = 3;
  size_t elements_read = fread(state, sizeof state[0], element_count, file);
  fclose(file);
  
  if (elements_read < element_count) {
    if (errno) fprintf(stderr, "ERROR: (%i) %s\n", errno, strerror(errno));
    fprintf(stderr, "ERROR: unable to read \"%s\"\n", dev_random_path);
    exit(EXIT_FAILURE);
  }
}


struct sj_random *
sj_random_alloc_fake(void)
{
  struct sj_random *random = sj_malloc(sizeof(struct sj_random));
  
  random->data = NULL;
  random->next_value_in_range = fake_next_value_in_range;
  
  return random;
}


struct sj_random *
sj_random_alloc_nrand48(void)
{
  struct sj_random *random = sj_malloc(sizeof(struct sj_random));
  
  unsigned short state[3];
  seed_nrand48_state(state);
  
  random->data = sj_arraydup(state, 3, sizeof state[0]);
  random->next_value_in_range = nrand48_next_value_in_range;
  
  return random;
}


void
sj_random_free_fake(struct sj_random *random)
{
  sj_free(random);
}


void
sj_random_free_nrand48(struct sj_random *random)
{
  sj_free(random->data);
  sj_free(random);
}


int
sj_random_next_value_in_range(struct sj_random *random, long range)
{
  return random->next_value_in_range(random->data, range);
}
