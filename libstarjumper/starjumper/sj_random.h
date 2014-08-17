#ifndef STARJUMPER_SJ_RANDOM_H_INCLUDED
#define STARJUMPER_SJ_RANDOM_H_INCLUDED


enum sj_random_fake_type
{
  sj_random_fake_type_unknown = 0,
  sj_random_fake_type_min,
  sj_random_fake_type_average,
  sj_random_fake_type_max,
};


struct sj_random
{
  void *data;
  int (*next_value_in_range)(void *data, long range);
  void (*free_data)(void *data);
};


struct sj_random *
sj_random_alloc_fake(enum sj_random_fake_type type);

struct sj_random *
sj_random_alloc_nrand48(void);

void
sj_random_free(struct sj_random *random);

int
sj_random_next_value_in_range(struct sj_random *random, long range);


#endif
