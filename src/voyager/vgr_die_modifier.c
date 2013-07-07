#include "vgr_die_modifier.h"


static sf_string_t 
string_from(sf_any_t self);


struct _vgr_die_modifier
{
  SF_OBJECT_FIELDS;
  int value;
};


sf_type_t vgr_die_modifier_type;


void
_vgr_die_modifier_init(void)
{
  vgr_die_modifier_type = sf_type("vgr_die_modifier_t", NULL, string_from, NULL, NULL, NULL, NULL);
}


static sf_string_t 
string_from(sf_any_t self)
{
  vgr_die_modifier_t die_modifier = self;
  return sf_string_from_format("%+i", die_modifier->value);
}


vgr_die_modifier_t
vgr_die_modifier(int value)
{
  struct _vgr_die_modifier *die_modifier = sf_object_calloc(sizeof(struct _vgr_die_modifier), vgr_die_modifier_type);
  if ( ! die_modifier) return NULL;
  
  die_modifier->value = value;
  
  return sf_move_to_temp_pool(die_modifier);
}


int
vgr_die_modifier_value(vgr_die_modifier_t die_modifier)
{
  return die_modifier ? die_modifier->value : 0;
}
