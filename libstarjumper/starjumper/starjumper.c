#include "starjumper.h"


void
_sj_dice_throw_init(void);

void
_sj_die_modifier_init(void);

void
_sj_world_init(void);


void
sj_init(void)
{
  sf_init();
  _sj_dice_throw_init();
  _sj_die_modifier_init();
  _sj_world_init();
}


void
sj_fin(void)
{
  sf_fin();
}
