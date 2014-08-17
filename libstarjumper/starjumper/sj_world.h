#ifndef STARJUMPER_SJ_WORLD_H_INCLUDED
#define STARJUMPER_SJ_WORLD_H_INCLUDED


#include <starjumper/sj_defs.h>


struct sj_hex_coordinate;
struct sj_random;


extern sf_type_t sj_world_type;


sj_world_t
sj_world(sf_string_t name,
         struct sj_hex_coordinate const hex_coordinate,
         struct sj_random *random);

int
sj_world_atmosphere(sj_world_t world);

int
sj_world_government(sj_world_t world);

struct sj_hex_coordinate
sj_world_hex_coordinate(sj_world_t world);

int
sj_world_hydrographics(sj_world_t world);

int
sj_world_law_level(sj_world_t world);

sf_string_t
sj_world_name(sj_world_t world);

int
sj_world_population(sj_world_t world);

int
sj_world_size(sj_world_t world);

char
sj_world_starport(sj_world_t world);


#endif
