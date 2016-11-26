#include "sj_trade_classification.h"

#include "alloc_or_die.h"
#include "sj_world.h"


static bool
is_agricultural(struct sj_world const *world);

static bool
is_asteroid_belt(struct sj_world const *world);

static bool
is_barren_world(struct sj_world const *world);

static bool
is_desert_world(struct sj_world const *world);

static bool
is_fluid_oceans(struct sj_world const *world);

static bool
is_high_population(struct sj_world const *world);

static bool
is_ice_capped(struct sj_world const *world);

static bool
is_industrial(struct sj_world const *world);

static bool
is_low_population(struct sj_world const *world);

static bool
is_non_agricultural(struct sj_world const *world);

static bool
is_non_industrial(struct sj_world const *world);

static bool
is_poor(struct sj_world const *world);

static bool
is_rich(struct sj_world const *world);

static bool
is_vacuum_world(struct sj_world const *world);

static bool
is_water_world(struct sj_world const *world);


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
is_agricultural(struct sj_world const *world)
{
  if (world->atmosphere >= 4 && world->atmosphere <= 9) {
    if (world->hydrographics >= 4 && world->hydrographics <= 8) {
      if (world->population >= 5 && world->population <= 7) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_asteroid_belt(struct sj_world const *world)
{
  if (world->size == 0) {
    // from Book 7: Merchant Prince
    if (world->atmosphere == 0) {
      if (world->hydrographics == 0) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_barren_world(struct sj_world const *world)
{
  // from Book 7: Merchant Prince
  if (world->population == 0) {
    if (world->government == 0) {
      if (world->law_level == 0) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_desert_world(struct sj_world const *world)
{
  // from Book 3: Worlds and Adventures (1981)
  //     hydrographic 0
  // from Supplement 10: The Solomani Rim
  //     atmosphere 2 through 9 and hydrographic 0
  // from Book 7: Merchant Prince
  //     atmosphere 2+ and hydrographics 0
  if (world->atmosphere >= 2) {
    if (world->hydrographics == 0) {
      return true;
    }
  }
  return false;
}


static bool
is_fluid_oceans(struct sj_world const *world)
{
  // from Book 7: Merchant Prince
  if (world->atmosphere >= 10) {
    if (world->hydrographics >= 1) {
      return true;
    }
  }
  return false;
}


static bool
is_high_population(struct sj_world const *world)
{
  // from Book 7: Merchant Prince
  if (world->population >= 9) {
    return true;
  }
  return false;
}


static bool
is_ice_capped(struct sj_world const *world)
{
  if (world->atmosphere == 0 || world->atmosphere == 1) {
    if (world->hydrographics >= 1) {
      return true;
    }
  }
  return false;
}


static bool
is_industrial(struct sj_world const *world)
{
  if (   world->atmosphere == 0 || world->atmosphere == 1
      || world->atmosphere == 2 || world->atmosphere == 4
      || world->atmosphere == 7 || world->atmosphere == 9)
  {
    if (world->population >= 9) {
      return true;
    }
  }
  return false;
}


static bool
is_low_population(struct sj_world const *world)
{
  // from Book 7: Merchant Prince
  if (world->population <= 3) {
    return true;
  }
  return false;
}


static bool
is_non_agricultural(struct sj_world const *world)
{
  if (world->atmosphere <= 3) {
    if (world->hydrographics <= 3) {
      if (world->population >= 6) {
        return true;
      }
    }
  }
  return false;
}


static bool
is_non_industrial(struct sj_world const *world)
{
  if (world->population <= 6) {
    return true;
  }
  return false;
}


static bool
is_poor(struct sj_world const *world)
{
  if (world->atmosphere >= 2 && world->atmosphere <= 5) {
    if (world->hydrographics <= 3) {
      return true;
    }
  }
  return false;
}


static bool
is_rich(struct sj_world const *world)
{
  if (world->atmosphere == 6 || world->atmosphere == 8) {
    if (world->population >= 6 && world->population <= 8) {
      if (world->government >= 4 && world->government <= 9) {
        // from Book 7: Merchant Prince
        //     Aslan rich worlds ignore government type.
        //     Vargr rich worlds may government type 4,5,6,8 or 9 (not type 7).
        return true;
      }
    }
  }
  return false;
}


static bool
is_vacuum_world(struct sj_world const *world)
{
  // from Book 3: Worlds and Adventures (1981)
  //     atmosphere of 0
  // from Supplement 10: The Solomani Rim
  //     size of at least 1 and an atmosphere of 0
  // from Book 7: Merchant Prince
  //     atmosphere of 0
  if (world->atmosphere == 0) {
    return true;
  }
  return false;
}


static bool
is_water_world(struct sj_world const *world)
{
  if (world->hydrographics == 10) {
    return true;
  }
  return false;
}


struct sj_trade_classification const **
sj_world_alloc_trade_classifications(struct sj_world const *world, int *count)
{
  size_t max_count = all_trade_classifications_count + 1;
  size_t item_size = sizeof all_trade_classifications[0];
  struct sj_trade_classification const **classifications_for_world = calloc_or_die(max_count, item_size);
  
  *count = 0;
  for (int i = 0; i < all_trade_classifications_count; ++i) {
    if (all_trade_classifications[i]->applies_to(world)) {
      int next_index = *count;
      classifications_for_world[next_index] = all_trade_classifications[i];
      ++*count;
    }
  }
  
  size_t final_size = (*count + 1) * item_size;
  return realloc_or_die(classifications_for_world, final_size);
}
