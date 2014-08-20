#include <earmark.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <starjumper/starjumper.h>

#include "options.h"


int
main(int argc, char **argv)
{
  struct options *options = options_alloc(argc, argv);
  struct sj_random *random = sj_random_alloc_nrand48();
  struct sj_world *world = sj_world_alloc(options->name, options->hex_coordinate, random);

  char *description = sj_string_from_world(world);
  fprintf(stdout, "%s\n", description);
  em_free(description);
  
  sj_world_free(world);
  sj_random_free(random);
  options_free(options);
  
  em_expect_alloc_count_zero();
  return EXIT_SUCCESS;
}
