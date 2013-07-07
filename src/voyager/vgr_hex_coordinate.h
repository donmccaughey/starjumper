#ifndef VOYAGER_VGR_HEX_COORDINATE_H_INCLUDED
#define VOYAGER_VGR_HEX_COORDINATE_H_INCLUDED


#include <voyager/vgr_defs.h>


extern sf_type_t vgr_hex_coordinate_type;


vgr_hex_coordinate_t
vgr_hex_coordinate(int horizontal, int vertical);

vgr_hex_coordinate_t
vgr_hex_coordinate_from_string(sf_string_t string);


#endif
