#ifndef STARJUMPER_SJ_RANDOM_H_INCLUDED
#define STARJUMPER_SJ_RANDOM_H_INCLUDED


struct sj_random
{
  void *data;
  int (*next_value_in_range)(void *data, long range);
};


struct sj_random *
sj_random_alloc_fake(void);

struct sj_random *
sj_random_alloc_nrand48(void);

void
sj_random_free_fake(struct sj_random *random);

void
sj_random_free_nrand48(struct sj_random *random);

int
sj_random_next_value_in_range(struct sj_random *random, long range);


#endif
