#ifndef STARJUMPER_VGR_TRADE_CLASSIFICATION_H_INCLUDED
#define STARJUMPER_VGR_TRADE_CLASSIFICATION_H_INCLUDED


#include <starjumper/vgr_defs.h>


struct vgr_trade_classification
{
    char const *name;
    char const *short_name;
    char const *abbreviation;
    bool (*applies_to)(vgr_world_t);
};


extern struct vgr_trade_classification const vgr_trade_classification_agricultural;
extern struct vgr_trade_classification const vgr_trade_classification_asteroid_belt;
extern struct vgr_trade_classification const vgr_trade_classification_barren_world;
extern struct vgr_trade_classification const vgr_trade_classification_desert_world;
extern struct vgr_trade_classification const vgr_trade_classification_fluid_oceans;
extern struct vgr_trade_classification const vgr_trade_classification_high_population;
extern struct vgr_trade_classification const vgr_trade_classification_ice_capped;
extern struct vgr_trade_classification const vgr_trade_classification_industrial;
extern struct vgr_trade_classification const vgr_trade_classification_low_population;
extern struct vgr_trade_classification const vgr_trade_classification_non_agricultural;
extern struct vgr_trade_classification const vgr_trade_classification_non_industrial;
extern struct vgr_trade_classification const vgr_trade_classification_poor;
extern struct vgr_trade_classification const vgr_trade_classification_rich;
extern struct vgr_trade_classification const vgr_trade_classification_vacuum_world;
extern struct vgr_trade_classification const vgr_trade_classification_water_world;


struct vgr_trade_classification const **
vgr_world_alloc_trade_classifications(vgr_world_t world, int *count);


#endif
