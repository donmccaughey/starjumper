#include <alloc_or_die.h>
#include <getopt.h>
#include <libgen.h>
#include <rnd.h>
#include <stdio.h>
#include <stdlib.h>
#include <starjumper/starjumper.h>

#include "options.h"


static void
generate_subsector(char const *name, struct rnd *rnd);

static void
generate_world(char const *name,
               struct sj_hex_coordinate hex_coordinate,
               struct rnd *rnd);


int
main(int argc, char **argv)
{
  struct options *options = options_alloc(argc, argv);
  if (options->subsector) {
    generate_subsector(options->name, options->rnd);
  } else {
    generate_world(options->name, options->hex_coordinate, options->rnd);
  }
  
  options_free(options);
  alloc_count_is_zero_or_die();
  return EXIT_SUCCESS;
}


static void
generate_subsector(char const *name, struct rnd *rnd)
{
  struct sj_subsector *subsector = sj_subsector_alloc(name, rnd);
  
  char *header = sj_string_from_world(NULL);
  fprintf(stdout, "%s\n", header);
  free_or_die(header);
  
  for (int i = 0; i < subsector->worlds_count; ++i) {
    char *description = sj_string_from_world(subsector->worlds[i]);
    fprintf(stdout, "%s\n", description);
    free_or_die(description);
  }
  
  sj_subsector_free(subsector);
}


static void
generate_world(char const *name,
               struct sj_hex_coordinate hex_coordinate,
               struct rnd *rnd)
{
  struct sj_world *world = sj_world_alloc(name, hex_coordinate, rnd);
  
  char *description = sj_string_from_world(world);
  fprintf(stdout, "%s\n", description);
  free_or_die(description);
  
  sj_world_free(world);
}
