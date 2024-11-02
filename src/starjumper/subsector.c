#include "subsector.h"

#include <xmalloc.h>
#include <xstring.h>

#include "dice_throw.h"
#include "world.h"


int const sj_subsector_height = 10;
int const sj_subsector_width = 8;

static int const occurrence_throw = 4;


struct sj_subsector *
sj_subsector_alloc(char const *name, struct rnd *rnd)
{
  struct sj_subsector *subsector = xcalloc(1, sizeof(struct sj_subsector));
  
  subsector->name = xstrdup(name);
  int max_world_count = sj_subsector_width * sj_subsector_height;
  subsector->worlds = xcalloc(max_world_count, sizeof(struct sj_world *));
  
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
  
  subsector->worlds = xreallocarray(subsector->worlds,
                                    subsector->worlds_count,
                                    sizeof(struct sj_world *));
  
  return subsector;
}


void
sj_subsector_free(struct sj_subsector *subsector)
{
  free(subsector->name);
  for (int i = 0; i < subsector->worlds_count; ++i) {
    sj_world_free(subsector->worlds[i]);
  }
  free(subsector->worlds);
  free(subsector);
}
