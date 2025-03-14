#ifndef STARJUMPER_WORLD_H_INCLUDED
#define STARJUMPER_WORLD_H_INCLUDED


#include <dice.h>
#include <stdbool.h>
#include <starjumper/hex_coordinate.h>


struct sj_trade_classification;


struct sj_world {
    char *name;
    struct sj_hex_coordinate hex_coordinate;
    char starport;
    bool naval_base;
    bool scout_base;
    bool gas_giant;
    int size;
    int atmosphere;
    int hydrographics;
    int population;
    int government;
    int law_level;
    int tech_level;
    struct sj_trade_classification const **trade_classifications;
    int trade_classifications_count;
};


char *
sj_string_from_world(struct sj_world const *world);

struct sj_world *
sj_world_alloc(char const *name,
               struct sj_hex_coordinate const hex_coordinate,
               struct die die);

void
sj_world_free(struct sj_world *world);


#endif
