#include "sj_trade_classification.h"

#include "sj_memory.h"
#include "sj_world.h"


static bool
is_agricultural(sj_world_t world);

static bool
is_asteroid_belt(sj_world_t world);

static bool
is_barren_world(sj_world_t world);

static bool
is_desert_world(sj_world_t world);

static bool
is_fluid_oceans(sj_world_t world);

static bool
is_high_population(sj_world_t world);

static bool
is_ice_capped(sj_world_t world);

static bool
is_industrial(sj_world_t world);

static bool
is_low_population(sj_world_t world);

static bool
is_non_agricultural(sj_world_t world);

static bool
is_non_industrial(sj_world_t world);

static bool
is_poor(sj_world_t world);

static bool
is_rich(sj_world_t world);

static bool
is_vacuum_world(sj_world_t world);

static bool
is_water_world(sj_world_t world);


struct sj_trade_classification const sj_trade_classification_agricultural = {
  .name="Agricultural",
  .short_name="Ag",
  .abbreviation="Ag",
  .applies_to=is_agricultural,
};

struct sj_trade_classification const sj_trade_classification_asteroid_belt = {
  .name="Asteroid Belt",
  .short_name="Asteroids",
  .abbreviation="As",
  .applies_to=is_asteroid_belt,
};

struct sj_trade_classification const sj_trade_classification_barren_world = {
  .name="Barren World",
  .short_name="Barren",
  .abbreviation="Ba",
  .applies_to=is_barren_world,
};

struct sj_trade_classification const sj_trade_classification_desert_world = {
  .name="Desert World",
  .short_name="Desert",
  .abbreviation="De",
  .applies_to=is_desert_world,
};

struct sj_trade_classification const sj_trade_classification_fluid_oceans = {
  .name="Fluid Oceans",
  .short_name="Fluid Oceans",
  .abbreviation="Fl",
  .applies_to=is_fluid_oceans,
};

struct sj_trade_classification const sj_trade_classification_high_population = {
  .name="High Population",
  .short_name="High Pop",
  .abbreviation="Hi",
  .applies_to=is_high_population,
};

struct sj_trade_classification const sj_trade_classification_ice_capped = {
  .name="Ice-capped",
  .short_name="Ice-capped",
  .abbreviation="Ic",
  .applies_to=is_ice_capped,
};

struct sj_trade_classification const sj_trade_classification_industrial = {
  .name="Industrial",
  .short_name="Industrial",
  .abbreviation="In",
  .applies_to=is_industrial,
};

struct sj_trade_classification const sj_trade_classification_low_population = {
  .name="Low Population",
  .short_name="Low Pop",
  .abbreviation="Lo",
  .applies_to=is_low_population,
};

struct sj_trade_classification const sj_trade_classification_non_agricultural = {
  .name="Non-agricultural",
  .short_name="Non-ag",
  .abbreviation="Na",
  .applies_to=is_non_agricultural,
};

struct sj_trade_classification const sj_trade_classification_non_industrial = {
  .name="Non-industrial",
  .short_name="Non-industrial",
  .abbreviation="Ni",
  .applies_to=is_non_industrial,
};

struct sj_trade_classification const sj_trade_classification_poor = {
  .name="Poor",
  .short_name="Poor",
  .abbreviation="Po",
  .applies_to=is_poor,
};

struct sj_trade_classification const sj_trade_classification_rich = {
  .name="Rich",
  .short_name="Rich",
  .abbreviation="Ri",
  .applies_to=is_rich,
};

struct sj_trade_classification const sj_trade_classification_vacuum_world = {
  .name="Vacuum World",
  .short_name="Vacuum",
  .abbreviation="Va",
  .applies_to=is_vacuum_world,
};

struct sj_trade_classification const sj_trade_classification_water_world = {
  .name="Water World",
  .short_name="Water World",
  .abbreviation="Wa",
  .applies_to=is_water_world,
};

static struct sj_trade_classification const *all_trade_classifications[] = {
    &sj_trade_classification_agricultural,
    &sj_trade_classification_asteroid_belt,
    &sj_trade_classification_barren_world,
    &sj_trade_classification_desert_world,
    &sj_trade_classification_fluid_oceans,
    &sj_trade_classification_high_population,
    &sj_trade_classification_ice_capped,
    &sj_trade_classification_industrial,
    &sj_trade_classification_low_population,
    &sj_trade_classification_non_agricultural,
    &sj_trade_classification_non_industrial,
    &sj_trade_classification_poor,
    &sj_trade_classification_rich,
    &sj_trade_classification_vacuum_world,
    &sj_trade_classification_water_world,
};

static int all_trade_classifications_count = sizeof all_trade_classifications
                                           / sizeof all_trade_classifications[0];


static bool
is_agricultural(sj_world_t world)
{
  if (world) {
    if (sj_world_atmosphere(world) >= 4 && sj_world_atmosphere(world) <= 9) {
      if (sj_world_hydrographics(world) >= 4 && sj_world_hydrographics(world) <= 8) {
        if (sj_world_population(world) >= 5 && sj_world_population(world) <= 7) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_asteroid_belt(sj_world_t world)
{
  if (world) {
    if (sj_world_size(world) == 0) {
      // from Book 7: Merchant Prince
      if (sj_world_atmosphere(world) == 0) {
        if (sj_world_hydrographics(world) == 0) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_barren_world(sj_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (sj_world_population(world) == 0) {
      if (sj_world_government(world) == 0) {
        if (sj_world_law_level(world) == 0) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_desert_world(sj_world_t world)
{
  if (world) {
    // from Book 3: Worlds and Adventures (1981)
    //     hydrographic 0
    // from Supplement 10: The Solomani Rim
    //     atmosphere 2 through 9 and hydrographic 0
    // from Book 7: Merchant Prince
    //     atmosphere 2+ and hydrographics 0
    if (sj_world_atmosphere(world) >= 2) {
      if (sj_world_hydrographics(world) == 0) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_fluid_oceans(sj_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (sj_world_atmosphere(world) >= 10) {
      if (sj_world_hydrographics(world) >= 1) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_high_population(sj_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (sj_world_population(world) >= 9) {
      return true;
    }
  }
  return false;
}


static bool
is_ice_capped(sj_world_t world)
{
  if (world) {
    if (sj_world_atmosphere(world) == 0 || sj_world_atmosphere(world) == 1) {
      if (sj_world_hydrographics(world) >= 1) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_industrial(sj_world_t world)
{
  if (world) {
    if (   sj_world_atmosphere(world) == 0 || sj_world_atmosphere(world) == 1
        || sj_world_atmosphere(world) == 2 || sj_world_atmosphere(world) == 4
        || sj_world_atmosphere(world) == 7 || sj_world_atmosphere(world) == 9)
    {
      if (sj_world_population(world) >= 9) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_low_population(sj_world_t world)
{
  if (world) {
    // from Book 7: Merchant Prince
    if (sj_world_population(world) <= 3) {
      return true;
    }
  }
  return false;
}


static bool
is_non_agricultural(sj_world_t world)
{
  if (world) {
    if (sj_world_atmosphere(world) <= 3) {
      if (sj_world_hydrographics(world) <= 3) {
        if (sj_world_population(world) >= 6) {
          return true;
        }
      }
    }
  }
  return false;
}


static bool
is_non_industrial(sj_world_t world)
{
  if (world) {
    if (sj_world_population(world) <= 6) {
      return true;
    }
  }
  return false;
}


static bool
is_poor(sj_world_t world)
{
  if (world) {
    if (sj_world_atmosphere(world) >= 2 && sj_world_atmosphere(world) <= 5) {
      if (sj_world_hydrographics(world) <= 3) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_rich(sj_world_t world)
{
  if (world) {
    if (sj_world_atmosphere(world) == 6 || sj_world_atmosphere(world) == 8) {
      if (sj_world_population(world) >= 6 && sj_world_population(world) <= 8) {
        if (sj_world_government(world) >= 4 && sj_world_government(world) <= 9) {
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
is_vacuum_world(sj_world_t world)
{
  if (world) {
    // from Book 3: Worlds and Adventures (1981)
    //     atmosphere of 0
    // from Supplement 10: The Solomani Rim
    //     size of at least 1 and an atmosphere of 0
    // from Book 7: Merchant Prince
    //     atmosphere of 0
    if (sj_world_atmosphere(world) == 0) {
      return true;
    }
  }
  return false;
}


static bool
is_water_world(sj_world_t world)
{
  if (world) {
    if (sj_world_hydrographics(world) == 10) {
      return true;
    }
  }
  return false;
}


struct sj_trade_classification const **
sj_world_alloc_trade_classifications(sj_world_t world, int *count)
{
  size_t max_count = all_trade_classifications_count + 1;
  size_t item_size = sizeof all_trade_classifications[0];
  struct sj_trade_classification const **classifications_for_world = sj_calloc(max_count, item_size);
  
  *count = 0;
  for (int i = 0; i < all_trade_classifications_count; ++i) {
    if (all_trade_classifications[i]->applies_to(world)) {
      int next_index = *count;
      classifications_for_world[next_index] = all_trade_classifications[i];
      ++*count;
    }
  }
  
  size_t final_size = (*count + 1) * item_size;
  return sj_realloc(classifications_for_world, final_size);
}
