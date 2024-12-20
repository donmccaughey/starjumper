#ifndef WORLDGEN_OPTIONS_H_INCLUDED
#define WORLDGEN_OPTIONS_H_INCLUDED


#include <stdbool.h>
#include <starjumper/starjumper.h>


struct lrnd;
struct rnd;


struct options
{
  struct sj_hex_coordinate hex_coordinate;
  struct lrnd *lrnd;
  char *name;
  bool subsector;
};


struct options *
options_alloc(int argc, char *argv[]);

void
options_free(struct options *options);


#endif
