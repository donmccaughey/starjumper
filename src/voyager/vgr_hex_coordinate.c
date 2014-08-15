#include "vgr_hex_coordinate.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vgr_memory.h"


bool
vgr_hex_coordinate_from_string(char const *string, struct vgr_hex_coordinate *hex_coordinate)
{
  if (4 != strlen(string)) return false;
  
  char horizontal_chars[3] = { '\0' };
  strncpy(horizontal_chars, string, 2);
  errno = 0;
  long horizontal = strtol(horizontal_chars, NULL, 10);
  if ( ! horizontal && errno) return false;
  if (horizontal < 1 || horizontal > 99) return false;
  
  char vertical_chars[3] = { '\0' };
  strncpy(vertical_chars, &string[2], 2);
  errno = 0;
  long vertical = strtol(vertical_chars, NULL, 10);
  if ( ! vertical && errno) return false;
  if (vertical < 1 || vertical > 99) return false;
  
  hex_coordinate->horizontal = (int) horizontal;
  hex_coordinate->vertical = (int) vertical;
  return true;
}


char *
vgr_string_alloc_from_hex_coordinate(struct vgr_hex_coordinate const hex_coordinate)
{
  char *string;
  vgr_asprintf(&string, "%02i%02i", hex_coordinate.horizontal, hex_coordinate.vertical);
  return string;
}
