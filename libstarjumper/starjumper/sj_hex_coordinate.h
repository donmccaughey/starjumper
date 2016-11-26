#ifndef STARJUMPER_SJ_HEX_COORDINATE_H_INCLUDED
#define STARJUMPER_SJ_HEX_COORDINATE_H_INCLUDED


#include <stdbool.h>


struct sj_hex_coordinate
{
  int horizontal;
  int vertical;
};


bool
sj_hex_coordinate_from_string(char const *string, struct sj_hex_coordinate *hex_coordinate);

char *
sj_string_alloc_from_hex_coordinate(struct sj_hex_coordinate hex_coordinate);

struct sj_hex_coordinate
sj_hex_coordinate_make(int horizontal, int vertical);


#endif
