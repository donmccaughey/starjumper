#ifndef STARJUMPER_SUBSECTOR_H_INCLUDED
#define STARJUMPER_SUBSECTOR_H_INCLUDED


#include <dice.h>
#include <starjumper/hex_coordinate.h>


struct sj_world;


extern int const sj_subsector_height;
extern int const sj_subsector_width;


struct sj_subsector {
    char *name;
    struct sj_world **worlds;
    int worlds_count;
};


struct sj_subsector *
sj_subsector_alloc(char const *name, struct die die);

void
sj_subsector_free(struct sj_subsector *subsector);


#endif
