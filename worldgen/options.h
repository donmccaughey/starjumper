#ifndef WORLDGEN_OPTIONS_H_INCLUDED
#define WORLDGEN_OPTIONS_H_INCLUDED


#include <starjumper/starjumper.h>


struct rnd;


struct options
{
  char *name;
  struct sj_hex_coordinate hex_coordinate;
  struct rnd *rnd;
};


struct options *
options_alloc(int argc, char *argv[]);

void
options_free(struct options *options);


#endif
