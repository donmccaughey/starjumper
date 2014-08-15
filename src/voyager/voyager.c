#include "voyager.h"


void
_vgr_dice_throw_init(void);

void
_vgr_die_modifier_init(void);

void
_vgr_trade_classification_init(void);

void
_vgr_world_init(void);


void
vgr_init(void)
{
  sf_init();
  _vgr_dice_throw_init();
  _vgr_die_modifier_init();
  _vgr_trade_classification_init();
  _vgr_world_init();
}


void
vgr_fin(void)
{
  sf_fin();
}
