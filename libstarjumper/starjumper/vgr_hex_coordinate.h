#ifndef STARJUMPER_VGR_HEX_COORDINATE_H_INCLUDED
#define STARJUMPER_VGR_HEX_COORDINATE_H_INCLUDED


#include <stdbool.h>


struct vgr_hex_coordinate
{
  int horizontal;
  int vertical;
};


bool
vgr_hex_coordinate_from_string(char const *string, struct vgr_hex_coordinate *hex_coordinate);

char *
vgr_string_alloc_from_hex_coordinate(struct vgr_hex_coordinate const hex_coordinate);


#endif
