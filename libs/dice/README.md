# dice

A C99 library for rolling dice for games.

[![builds.sr.ht status](https://builds.sr.ht/~donmcc/dice.svg)](https://builds.sr.ht/~donmcc/dice?)


## License

`dice` is made available under a BSD-style license; see the [LICENSE][101] file
for details.

[101]: ./LICENSE


## Library Conventions

Functions that allocate memory will call `abort()` if memory allocation fails,
so checking for NULL is not necessary.  Most functions use `assert()` to
validate parameters and invariants but do no validation when `NDEBUG` is
defined.  However, the `dice_alloc_parse()` function will return NULL if the
given dice expression is invalid so that it can be used to validate user input.


## The `die` struct

A `die` struct is a number generator that produces the roll of a single die.
The number produced by a `die` is in the range `[1, sides]` where `sides` is
determined by a `dice` struct.  A `die` struct can generate random or
deterministic rolls.

The global variables `high_die`, `low_die` and `median_die` are deterministic
`die` structs that are useful for testing.  The `high_die` and `low_die` structs
generate max and min rolls respectively.  The `median_die` struct generates the
middle roll for an odd number of sides, or the higher of the two middle rolls
for an even number of sides.

The global variable `random_die` generates a random roll using 
[arc4random_uniform(3)][10], a high quality random number generator.

[10]: https://man.bsd.lv/arc4random_uniform

The `die_make_ascending()` function initializes a `die` struct that emits a
monotonically increasing sequence of rolls starting at the given initial value.
The `die_make_fixed()` function initializes a `die` struct that emits a fixed
roll.


## The `mod` struct

A `mod` struct is a modifier applied to a dice roll.  The `op` field is the
operator ('+', '-' or 'x') and `value` is the integer value of the modifier.
The `mod_make()` function initializes a `mod` struct; it will normalize negative
numbers, for example `mod_make('-', -3)` is equivalent to `mod_make('+', 3)`.


## The `dice` struct

A `dice` struct represents a set of dice to be rolled and modifiers applied to
the roll.  The set of dice is determined by fields `count` and `sides`.  The
modifiers are stored in unsized array `mods`, with `mods_count` and
`mods_capacity` tracking the size of `mods`.

The `dice_alloc()` and `dice_alloc_with_mods()` functions allocate a `dice`
struct without or with modifiers.  The `dice_alloc_with_mods_capacity()`
functions allocate a `dice` struct with a specified capacity for modifiers.

The `dice_alloc_parse()` function allocates a `dice` struct by parsing a dice
expression like `3d6+2` or `2d8-1x10`; it returns NULL if the dice expression is
invalid.

The `dice_alloc_init()` and `dice_init()` functions perform generalized
initialization of a `dice` struct with and without memory allocation.

The `dice_realloc_add_mod()` and `dice_realloc_set_mods_capacity()` functions
modify the `mods` array field of a `dice` struct, possibly reallocating the
`dice` struct in the process.  Both return a pointer to the modified `dice`
struct; the `dice` struct parameter may be invalid on return.

The `dice_description_alloc()` function allocates a string containing the dice
expression for a `dice` struct.

Use `free()` to deallocate `dice` structs and dice descriptions.


## The `roll` struct

A `roll` struct represents the result of rolling dice.  It contains a const
pointer to the `dice` struct that was rolled and the unsized `die_rolls` array
containing the values rolled.  The creator of a `roll` struct is responsible for
ensuring that the referenced `dice` struct lives at least as long as the `roll`.

The `roll_alloc()` function allocates a `roll` struct given a `dice` and a `die`
struct respectively.  The `roll_init()` functions generates a new roll given
an existing correctly-sized `roll` struct and a `dice` and `die` struct.

Use `free()` to deallocate `roll` structs.

The `roll_total()` function calculates the die roll total by summing the die
roll values and applying the modifiers.


## The roll functions

The `roll_die()` function rolls a die given the number of sides and a `die`
struct.

The `roll_dice()` function rolls a set of dice given a `dice` struct and a `die`
struct.

The `roll()` function rolls a set of dice given the count of dice, number of
sides and a `die` struct.

The `roll_with_mod()` and `roll_with_mods()` functions roll a set of dice given
the count of dice, number of sides, modifier(s) and a `die` struct.

The `roll_parse()` function rolls a set of dice given a dice expression and a
`die` struct; it should not be used with untrusted input.
