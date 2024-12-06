#include <starjumper/starjumper.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"


static void
generate_subsector(char const *name, struct lrnd *lrnd);

static void
generate_world(char const *name,
               struct sj_hex_coordinate hex_coordinate,
               struct lrnd *lrnd);


int
main(int argc, char **argv)
{
  struct options *options = options_alloc(argc, argv);
  if (options->subsector) {
    generate_subsector(options->name, options->lrnd);
  } else {
    generate_world(options->name, options->hex_coordinate, options->lrnd);
  }
  
  options_free(options);
  return EXIT_SUCCESS;
}


static void
generate_subsector(char const *name, struct lrnd *lrnd)
{
  struct sj_subsector *subsector = sj_subsector_alloc(name, lrnd);
  
  char *header = sj_string_from_world(NULL);
  fprintf(stdout, "%s\n", header);
  free(header);
  
  for (int i = 0; i < subsector->worlds_count; ++i) {
    char *description = sj_string_from_world(subsector->worlds[i]);
    fprintf(stdout, "%s\n", description);
    free(description);
  }
  
  sj_subsector_free(subsector);
}


static void
generate_world(char const *name,
               struct sj_hex_coordinate hex_coordinate,
               struct lrnd *lrnd)
{
  struct sj_world *world = sj_world_alloc(name, hex_coordinate, lrnd);
  
  char *description = sj_string_from_world(world);
  fprintf(stdout, "%s\n", description);
  free(description);
  
  sj_world_free(world);
}
