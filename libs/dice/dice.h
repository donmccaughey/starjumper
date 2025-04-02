#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED


typedef int (roll_die_fn)(int sides, void *state);


struct die {
    roll_die_fn *roll;
    void *state;
};

struct mod {
    char op;  // ['+', '-', 'x']
    int value;
};

struct dice {
    int count;
    int sides;
    int mods_capacity;
    int mods_count;
    struct mod mods[];
};

struct roll {
    struct dice const *dice;
    int die_rolls_count;
    int die_rolls[];
};


extern struct die high_die;
extern struct die low_die;
extern struct die median_die;
extern struct die random_die;


struct die
die_make_ascending(int *initial_value);

struct die
die_make_fixed(int *fixed_value);


struct mod
mod_make(char op, int value);


struct dice *
dice_alloc(int count, int sides);

struct dice *
dice_alloc_with_mods(int count, int sides, struct mod mods[], int mods_count);

struct dice *
dice_alloc_with_mods_capacity(int count, int sides, int mods_capacity);

struct dice *
dice_alloc_parse(char const *dice_expression);

struct dice *
dice_alloc_init(int count, int sides, int mods_capacity, int mods_count, struct mod mods[]);

void
dice_init(struct dice *dice, int count, int sides, int mods_capacity, int mods_count, struct mod mods[]);

struct dice *
dice_realloc_add_mod(struct dice *dice, struct mod mod);

struct dice *
dice_realloc_set_mods_capacity(struct dice *dice, int mods_capacity);

char *
dice_description_alloc(struct dice const *dice);


struct roll *
roll_alloc(struct dice const *dice, struct die die);

void
roll_init(struct roll *roll, struct dice const *dice, struct die die);

int
roll_total(struct roll const *roll);


int
roll_die(int sides, struct die die);

int
roll_dice(struct dice const *dice, struct die die);

int
roll(int count, int sides, struct die die);

int
roll_with_mod(int count, int sides, struct mod mod, struct die die);

int
roll_with_mods(int count, int sides, struct mod mods[], int mods_count, struct die die);

int
roll_parse(char const *dice_expression, struct die die);


#endif
