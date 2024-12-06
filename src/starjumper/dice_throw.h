#ifndef STARJUMPER_DICE_THROW_H_INCLUDED
#define STARJUMPER_DICE_THROW_H_INCLUDED


struct lrnd;


struct sj_dice_throw
{
  int count;
  int sides;
  int *rolls;
  int *modifiers;
  int modifiers_count;
};


int
sj_dice_throw(int count,
              int sides,
              int modifiers[],
              int modifiers_count,
              struct lrnd *lrnd);

void
sj_dice_throw_add_modifier(struct sj_dice_throw *dice_throw, int modifier);

struct sj_dice_throw *
sj_dice_throw_alloc(int count,
                    int sides,
                    struct lrnd *lrnd);

void
sj_dice_throw_free(struct sj_dice_throw *dice_throw);

int
sj_dice_throw_total(struct sj_dice_throw *dice_throw);

char *
sj_string_alloc_from_dice_throw(struct sj_dice_throw const *dice_throw);


#endif
