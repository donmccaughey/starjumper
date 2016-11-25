#include <alloc_or_die.h>
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
  free_or_die(description);
  
  sj_world_free(world);
  sj_random_free(random);
  options_free(options);
  
  alloc_count_is_zero_or_die();
  return EXIT_SUCCESS;
}
