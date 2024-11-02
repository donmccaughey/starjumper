#ifndef STARJUMPER_TRADE_CLASSIFICATION_H_INCLUDED
#define STARJUMPER_TRADE_CLASSIFICATION_H_INCLUDED


#include <stdbool.h>


struct sj_world;


struct sj_trade_classification
{
    char const *name;
    char const *short_name;
    char const *abbreviation;
    bool (*applies_to)(struct sj_world const *world);
};


extern struct sj_trade_classification const sj_trade_classification_agricultural;
extern struct sj_trade_classification const sj_trade_classification_asteroid_belt;
extern struct sj_trade_classification const sj_trade_classification_barren_world;
extern struct sj_trade_classification const sj_trade_classification_desert_world;
extern struct sj_trade_classification const sj_trade_classification_fluid_oceans;
extern struct sj_trade_classification const sj_trade_classification_high_population;
extern struct sj_trade_classification const sj_trade_classification_ice_capped;
extern struct sj_trade_classification const sj_trade_classification_industrial;
extern struct sj_trade_classification const sj_trade_classification_low_population;
extern struct sj_trade_classification const sj_trade_classification_non_agricultural;
extern struct sj_trade_classification const sj_trade_classification_non_industrial;
extern struct sj_trade_classification const sj_trade_classification_poor;
extern struct sj_trade_classification const sj_trade_classification_rich;
extern struct sj_trade_classification const sj_trade_classification_vacuum_world;
extern struct sj_trade_classification const sj_trade_classification_water_world;


struct sj_trade_classification const **
sj_world_alloc_trade_classifications(struct sj_world const *world, int *count);


#endif
