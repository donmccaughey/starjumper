#include "vgr_trade_classification.h"

#include "vgr_memory.h"
#include "vgr_world.h"


static bool
is_agricultural(vgr_world_t world);

static bool
is_asteroid_belt(vgr_world_t world);

static bool
is_barren_world(vgr_world_t world);

static bool
is_desert_world(vgr_world_t world);

static bool
is_fluid_oceans(vgr_world_t world);

static bool
is_high_population(vgr_world_t world);

static bool
is_ice_capped(vgr_world_t world);

static bool
is_industrial(vgr_world_t world);

static bool
is_low_population(vgr_world_t world);

static bool
is_non_agricultural(vgr_world_t world);

static bool
is_non_industrial(vgr_world_t world);

static bool
is_poor(vgr_world_t world);

static bool
is_rich(vgr_world_t world);

static bool
is_vacuum_world(vgr_world_t world);

static bool
is_water_world(vgr_world_t world);


struct vgr_trade_classification const vgr_trade_classification_agricultural = {
  .name="Agricultural",
  .short_name="Ag",
  .abbreviation="Ag",
  .applies_to=is_agricultural,
};

struct vgr_trade_classification const vgr_trade_classification_asteroid_belt = {
  .name="Asteroid Belt",
  .short_name="Asteroids",
  .abbreviation="As",
  .applies_to=is_asteroid_belt,
};

struct vgr_trade_classification const vgr_trade_classification_barren_world = {
  .name="Barren World",
  .short_name="Barren",
  .abbreviation="Ba",
  .applies_to=is_barren_world,
};

struct vgr_trade_classification const vgr_trade_classification_desert_world = {
  .name="Desert World",
  .short_name="Desert",
  .abbreviation="De",
  .applies_to=is_desert_world,
};

struct vgr_trade_classification const vgr_trade_classification_fluid_oceans = {
  .name="Fluid Oceans",
  .short_name="Fluid Oceans",
  .abbreviation="Fl",
  .applies_to=is_fluid_oceans,
};

struct vgr_trade_classification const vgr_trade_classification_high_population = {
  .name="High Population",
  .short_name="High Pop",
  .abbreviation="Hi",
  .applies_to=is_high_population,
};

struct vgr_trade_classification const vgr_trade_classification_ice_capped = {
  .name="Ice-capped",
  .short_name="Ice-capped",
  .abbreviation="Ic",
  .applies_to=is_ice_capped,
};

struct vgr_trade_classification const vgr_trade_classification_industrial = {
  .name="Industrial",
  .short_name="Industrial",
  .abbreviation="In",
  .applies_to=is_industrial,
};

struct vgr_trade_classification const vgr_trade_classification_low_population = {
  .name="Low Population",
  .short_name="Low Pop",
  .abbreviation="Lo",
  .applies_to=is_low_population,
};

struct vgr_trade_classification const vgr_trade_classification_non_agricultural = {
  .name="Non-agricultural",
  .short_name="Non-ag",
  .abbreviation="Na",
  .applies_to=is_non_agricultural,
};

struct vgr_trade_classification const vgr_trade_classification_non_industrial = {
  .name="Non-industrial",
  .short_name="Non-industrial",
  .abbreviation="Ni",
  .applies_to=is_non_industrial,
};

struct vgr_trade_classification const vgr_trade_classification_poor = {
  .name="Poor",
  .short_name="Poor",
  .abbreviation="Po",
  .applies_to=is_poor,
};

struct vgr_trade_classification const vgr_trade_classification_rich = {
  .name="Rich",
  .short_name="Rich",
  .abbreviation="Ri",
  .applies_to=is_rich,
};

struct vgr_trade_classification const vgr_trade_classification_vacuum_world = {
  .name="Vacuum World",
  .short_name="Vacuum",
  .abbreviation="Va",
  .applies_to=is_vacuum_world,
};

struct vgr_trade_classification const vgr_trade_classification_water_world = {
  .name="Water World",
  .short_name="Water World",
  .abbreviation="Wa",
  .applies_to=is_water_world,
};

static struct vgr_trade_classification const *all_trade_classifications[] = {
    &vgr_trade_classification_agricultural,
    &vgr_trade_classification_asteroid_belt,
    &vgr_trade_classification_barren_world,
    &vgr_trade_classification_desert_world,
    &vgr_trade_classification_fluid_oceans,
    &vgr_trade_classification_high_population,
    &vgr_trade_classification_ice_capped,
    &vgr_trade_classification_industrial,
    &vgr_trade_classification_low_population,
    &vgr_trade_classification_non_agricultural,
    &vgr_trade_classification_non_industrial,
    &vgr_trade_classification_poor,
    &vgr_trade_classification_rich,
    &vgr_trade_classification_vacuum_world,
    &vgr_trade_classification_water_world,
};

static int all_trade_classifications_count = sizeof all_trade_classifications
                                           / sizeof all_trade_classifications[0];


static bool
is_agricultural(vgr_world_t world)
{
  if (world) {
    if (vgr_world_atmosphere(world) >= 4 && vgr_world_atmosphere(world) <= 9) {
      if (vgr_world_hydrographics(world) >= 4 && vgr_world_hydrographics(world) <= 8) {
        if (vgr_world_population(world) >= 5 && vgr_world_population(world) <= 7) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_asteroid_belt(vgr_world_t world)
{
  if (world) {
    if (vgr_world_size(world) == 0) {
      // from Book 7: Merchant Prince
      if (vgr_world_atmosphere(world) == 0) {
        if (vgr_world_hydrographics(world) == 0) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_barren_world(vgr_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (vgr_world_population(world) == 0) {
      if (vgr_world_government(world) == 0) {
        if (vgr_world_law_level(world) == 0) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_desert_world(vgr_world_t world)
{
  if (world) {
    // from Book 3: Worlds and Adventures (1981)
    //     hydrographic 0
    // from Supplement 10: The Solomani Rim
    //     atmosphere 2 through 9 and hydrographic 0
    // from Book 7: Merchant Prince
    //     atmosphere 2+ and hydrographics 0
    if (vgr_world_atmosphere(world) >= 2) {
      if (vgr_world_hydrographics(world) == 0) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_fluid_oceans(vgr_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (vgr_world_atmosphere(world) >= 10) {
      if (vgr_world_hydrographics(world) >= 1) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_high_population(vgr_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (vgr_world_population(world) >= 9) {
      return true;
    }
  }
  return false;
}


static bool
is_ice_capped(vgr_world_t world)
{
  if (world) {
    if (vgr_world_atmosphere(world) == 0 || vgr_world_atmosphere(world) == 1) {
      if (vgr_world_hydrographics(world) >= 1) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_industrial(vgr_world_t world)
{
  if (world) {
    if (   vgr_world_atmosphere(world) == 0 || vgr_world_atmosphere(world) == 1
        || vgr_world_atmosphere(world) == 2 || vgr_world_atmosphere(world) == 4
        || vgr_world_atmosphere(world) == 7 || vgr_world_atmosphere(world) == 9)
    {
      if (vgr_world_population(world) >= 9) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_low_population(vgr_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (vgr_world_population(world) <= 3) {
      return true;
    }
  }
  return false;
}


static bool
is_non_agricultural(vgr_world_t world)
{
  if (world) {
    if (vgr_world_atmosphere(world) <= 3) {
      if (vgr_world_hydrographics(world) <= 3) {
        if (vgr_world_population(world) >= 6) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_non_industrial(vgr_world_t world)
{
  if (world) {
    if (vgr_world_population(world) <= 6) {
      return true;
    }
  }
  return false;
}


static bool
is_poor(vgr_world_t world)
{
  if (world) {
    if (vgr_world_atmosphere(world) >= 2 && vgr_world_atmosphere(world) <= 5) {
      if (vgr_world_hydrographics(world) <= 3) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_rich(vgr_world_t world)
{
  if (world) {
    if (vgr_world_atmosphere(world) == 6 || vgr_world_atmosphere(world) == 8) {
      if (vgr_world_population(world) >= 6 && vgr_world_population(world) <= 8) {
        if (vgr_world_government(world) >= 4 && vgr_world_government(world) <= 9) {
          // from Book 7: Merchant Prince
          //     Aslan rich worlds ignore government type.
          //     Vargr rich worlds may government type 4,5,6,8or 9 (not type 7).
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_vacuum_world(vgr_world_t world)
{
  if (world) {
    // from Book 3: Worlds and Adventures (1981)
    //     atmosphere of 0
    // from Supplement 10: The Solomani Rim
    //     size of at least 1 and an atmosphere of 0
    // from Book 7: Merchant Prince
    //     atmosphere of 0
    if (vgr_world_atmosphere(world) == 0) {
      return true;
    }
  }
  return false;
}


static bool
is_water_world(vgr_world_t world)
{
  if (world) {
    if (vgr_world_hydrographics(world) == 10) {
      return true;
    }
  }
  return false;
}


struct vgr_trade_classification const **
vgr_world_alloc_trade_classifications(vgr_world_t world, int *count)
{
  size_t max_count = all_trade_classifications_count + 1;
  size_t item_size = sizeof all_trade_classifications[0];
  struct vgr_trade_classification const **classifications_for_world = vgr_calloc(max_count, item_size);
  
  *count = 0;
  for (int i = 0; i < all_trade_classifications_count; ++i) {
    if (all_trade_classifications[i]->applies_to(world)) {
      int next_index = *count;
      classifications_for_world[next_index] = all_trade_classifications[i];
      ++*count;
    }
  }
  
  size_t final_size = (*count + 1) * item_size;
  return vgr_realloc(classifications_for_world, final_size);
}
