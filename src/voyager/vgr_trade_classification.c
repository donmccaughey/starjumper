#include "vgr_trade_classification.h"

#include "vgr_world.h"


struct _vgr_trade_classification
{
  SF_OBJECT_FIELDS;
  sf_string_t name;
  sf_string_t short_name;
  sf_string_t abbreviation;
  bool (*applies_to)(vgr_world_t);
};


static void
dealloc(sf_any_t self);

static bool
has_trade_classification(sf_any_t any, void *context);

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

static sf_string_t 
string_from(sf_any_t self);

static vgr_trade_classification_t
trade_classification(char const *name, char const *short_name, char const *abbreviation, bool (*applies_to)(vgr_world_t));


vgr_trade_classification_t vgr_trade_classification_agricultural;
vgr_trade_classification_t vgr_trade_classification_asteroid_belt;
vgr_trade_classification_t vgr_trade_classification_barren_world;
vgr_trade_classification_t vgr_trade_classification_desert_world;
vgr_trade_classification_t vgr_trade_classification_fluid_oceans;
vgr_trade_classification_t vgr_trade_classification_high_population;
vgr_trade_classification_t vgr_trade_classification_ice_capped;
vgr_trade_classification_t vgr_trade_classification_industrial;
vgr_trade_classification_t vgr_trade_classification_low_population;
vgr_trade_classification_t vgr_trade_classification_non_agricultural;
vgr_trade_classification_t vgr_trade_classification_non_industrial;
vgr_trade_classification_t vgr_trade_classification_poor;
vgr_trade_classification_t vgr_trade_classification_rich;
vgr_trade_classification_t vgr_trade_classification_vacuum_world;
vgr_trade_classification_t vgr_trade_classification_water_world;

sf_array_t vgr_trade_classifications;
sf_type_t vgr_trade_classification_type;


void
_vgr_trade_classification_init(void)
{
  vgr_trade_classification_type = sf_type("vgr_trade_classification_t", dealloc, string_from, NULL, NULL, NULL, NULL);
  
  vgr_trade_classification_agricultural = trade_classification("Agricultural",     "Ag",             "Ag", is_agricultural);
  vgr_trade_classification_asteroid_belt = trade_classification("Asteroid Belt",    "Asteroids",      "As", is_asteroid_belt);
  vgr_trade_classification_barren_world = trade_classification("Barren World",     "Barren",         "Ba", is_barren_world);
  vgr_trade_classification_desert_world = trade_classification("Desert World",     "Desert",         "De", is_desert_world);
  vgr_trade_classification_fluid_oceans = trade_classification("Fluid Oceans",     "Fluid Oceans",   "Fl", is_fluid_oceans);
  vgr_trade_classification_high_population = trade_classification("High Population",  "High Pop",       "Hi", is_high_population);
  vgr_trade_classification_ice_capped = trade_classification("Ice-capped",       "Ice-capped",     "Ic", is_ice_capped);
  vgr_trade_classification_industrial = trade_classification("Industrial",       "Industrial",     "In", is_industrial);
  vgr_trade_classification_low_population = trade_classification("Low Population",   "Low Pop",        "Lo", is_low_population);
  vgr_trade_classification_non_agricultural = trade_classification("Non-agricultural", "Non-ag",         "Na", is_non_agricultural);
  vgr_trade_classification_non_industrial = trade_classification("Non-industrial",   "Non-industrial", "Ni", is_non_industrial);
  vgr_trade_classification_poor = trade_classification("Poor",             "Poor",           "Po", is_poor);
  vgr_trade_classification_rich = trade_classification("Rich",             "Rich",           "Ri", is_rich);
  vgr_trade_classification_vacuum_world = trade_classification("Vacuum World",     "Vacuum",         "Va", is_vacuum_world);
  vgr_trade_classification_water_world = trade_classification("Water World",      "Water World",    "Wa", is_water_world);
  
  vgr_trade_classifications = sf_array_from_items(
  vgr_trade_classification_agricultural,
  vgr_trade_classification_asteroid_belt,
  vgr_trade_classification_barren_world,
  vgr_trade_classification_desert_world,
  vgr_trade_classification_fluid_oceans,
  vgr_trade_classification_high_population,
  vgr_trade_classification_ice_capped,
  vgr_trade_classification_industrial,
  vgr_trade_classification_low_population,
  vgr_trade_classification_non_agricultural,
  vgr_trade_classification_non_industrial,
  vgr_trade_classification_poor,
  vgr_trade_classification_rich,
  vgr_trade_classification_vacuum_world,
  vgr_trade_classification_water_world
  );
}


static void
dealloc(sf_any_t self)
{
  vgr_trade_classification_t trade_classification = self;
  
  sf_release(trade_classification->name);
  sf_release(trade_classification->short_name);
  sf_release(trade_classification->abbreviation);
}


static bool
has_trade_classification(sf_any_t any, void *context)
{
  if ( ! any) return false;
  if ( ! context) return false;

  vgr_trade_classification_t trade_classification = any;
  vgr_world_t world = context;
  return vgr_trade_classification_applies_to(trade_classification, world);
}


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


static sf_string_t 
string_from(sf_any_t self)
{
  vgr_trade_classification_t trade_classification = self;
  return sf_copy_to_temp_pool(trade_classification->name);
}


static vgr_trade_classification_t
trade_classification(char const *name, char const *short_name, char const *abbreviation, bool (*applies_to)(vgr_world_t))
{
  struct _vgr_trade_classification *trade_classification = sf_object_calloc(sizeof(struct _vgr_trade_classification), vgr_trade_classification_type);
  if ( ! trade_classification) return NULL;
  
  trade_classification->name = sf_retain(sf_string(name));
  trade_classification->short_name = sf_retain(sf_string(short_name));
  trade_classification->abbreviation = sf_retain(sf_string(abbreviation));
  trade_classification->applies_to = applies_to;
  
  return sf_move_to_temp_pool(trade_classification);
}


sf_string_t 
vgr_trade_classification_abbreviation(vgr_trade_classification_t trade_classification)
{
  return trade_classification ? sf_copy_to_temp_pool(trade_classification->abbreviation) : NULL;
}


bool
vgr_trade_classification_applies_to(vgr_trade_classification_t trade_classification,
                                     vgr_world_t world)
{
  if ( ! trade_classification) return false;
  if ( ! world) return false;
  
  return trade_classification->applies_to(world);
}


sf_string_t 
vgr_trade_classification_name(vgr_trade_classification_t trade_classification)
{
  return trade_classification ? sf_copy_to_temp_pool(trade_classification->name) : NULL;
}


sf_string_t 
vgr_trade_classification_short_name(vgr_trade_classification_t trade_classification)
{
  return trade_classification ? sf_copy_to_temp_pool(trade_classification->short_name) : NULL;
}


sf_list_t
vgr_trade_classifications_for(vgr_world_t world)
{
  return sf_filter(vgr_trade_classifications, has_trade_classification, (void *) world);
}
