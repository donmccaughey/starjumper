#ifndef VOYAGER_VGR_WORLD_H_INCLUDED
#define VOYAGER_VGR_WORLD_H_INCLUDED


#include <voyager/vgr_defs.h>


struct vgr_hex_coordinate;


extern sf_type_t vgr_world_type;


vgr_world_t
vgr_world(sf_string_t name,
          struct vgr_hex_coordinate const hex_coordinate,
          sf_random_t random_in,
          sf_random_t *random_out);

int
vgr_world_atmosphere(vgr_world_t world);

int
vgr_world_government(vgr_world_t world);

struct vgr_hex_coordinate
vgr_world_hex_coordinate(vgr_world_t world);

int
vgr_world_hydrographics(vgr_world_t world);

int
vgr_world_law_level(vgr_world_t world);

sf_string_t
vgr_world_name(vgr_world_t world);

int
vgr_world_population(vgr_world_t world);

int
vgr_world_size(vgr_world_t world);

char
vgr_world_starport(vgr_world_t world);


#endif
