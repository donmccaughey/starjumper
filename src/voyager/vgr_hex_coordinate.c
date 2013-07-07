#include "vgr_hex_coordinate.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct _vgr_hex_coordinate
{
  SF_OBJECT_FIELDS;
  int horizontal;
  int vertical;
};


static sf_string_t 
string_from(sf_any_t self);


sf_type_t vgr_hex_coordinate_type;


void
_vgr_hex_coordinate_init(void)
{
  vgr_hex_coordinate_type = sf_type("vgr_hex_coordinate_t", NULL, string_from, NULL, NULL, NULL, NULL);
}


static sf_string_t 
string_from(sf_any_t self)
{
  vgr_hex_coordinate_t hex_coordinate = self;

  return sf_string_from_format("%02i%02i", hex_coordinate->horizontal, hex_coordinate->vertical);
}


vgr_hex_coordinate_t
vgr_hex_coordinate(int horizontal, int vertical)
{
  struct _vgr_hex_coordinate *hex_coordinate = sf_object_calloc(sizeof(struct _vgr_hex_coordinate), vgr_hex_coordinate_type);
  if ( ! hex_coordinate) return NULL;
  
  hex_coordinate->horizontal = horizontal;
  hex_coordinate->vertical = vertical;
  
  return sf_move_to_temp_pool(hex_coordinate);
}


vgr_hex_coordinate_t
vgr_hex_coordinate_from_string(sf_string_t string)
{
  char const *chars = sf_string_chars(string);
  if (4 != strlen(chars)) return NULL;
  
  char horizontal_chars[3] = { '\0' };
  strncpy(horizontal_chars, chars, 2);
  errno = 0;
  long horizontal = strtol(horizontal_chars, NULL, 10);
  if ( ! horizontal && errno) return NULL;
  if (horizontal < 1 || horizontal > 99) return NULL;
  
  char vertical_chars[3] = { '\0' };
  strncpy(vertical_chars, &chars[2], 2);
  errno = 0;
  long vertical = strtol(vertical_chars, NULL, 10);
  if ( ! vertical && errno) return NULL;
  if (vertical < 1 || vertical > 99) return NULL;
  
  return vgr_hex_coordinate((int) horizontal, (int) vertical);
}
