#include "sj_subsector.h"

#include <alloc_or_die.h>

#include "sj_dice_throw.h"
#include "sj_world.h"


int const sj_subsector_height = 10;
int const sj_subsector_width = 8;

static int const occurrence_throw = 4;


struct sj_subsector *
sj_subsector_alloc(char const *name, struct rnd *rnd)
{
  struct sj_subsector *subsector = calloc_or_die(1, sizeof(struct sj_subsector));
  
  subsector->name = strdup_or_die(name);
  int max_world_count = sj_subsector_width * sj_subsector_height;
  subsector->worlds = calloc_or_die(max_world_count, sizeof(struct sj_world *));
  
  for (int h = 1; h <= sj_subsector_width; ++h) {
    for (int v = 1; v <= sj_subsector_height; ++v) {
      int throw = sj_dice_throw(1, 6, NULL, 0, rnd);
      if (throw >= occurrence_throw) {
        int i = subsector->worlds_count;
        subsector->worlds[i] = sj_world_alloc("No Name",
                                              sj_hex_coordinate_make(h, v),
                                              rnd);
        ++subsector->worlds_count;
      }
    }
  }
  
  subsector->worlds = reallocarray_or_die(subsector->worlds,
                                          subsector->worlds_count,
                                          sizeof(struct sj_world *));
  
  return subsector;
}


void
sj_subsector_free(struct sj_subsector *subsector)
{
  free_or_die(subsector->name);
  for (int i = 0; i < subsector->worlds_count; ++i) {
    sj_world_free(subsector->worlds[i]);
  }
  free_or_die(subsector->worlds);
  free_or_die(subsector);
}
