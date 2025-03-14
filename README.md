# Starjumper

_Old school paper-and-pencil science fiction role playing rules in code._

> **starjumper**
> 1. A person who travels frequently by starship to other worlds.
> 2. An archaic term for a jump-capable starship, esp. a small scout or yacht.

[![GitHub Actions][11]][12]
[![Code Coverage][13]][14]

[11]: https://github.com/donmccaughey/starjumper/actions/workflows/tests.yml/badge.svg?branch=main
[12]: https://github.com/donmccaughey/starjumper/actions/workflows/tests.yml
[13]: https://codecov.io/gh/donmccaughey/starjumper/branch/main/graph/badge.svg
[14]: https://codecov.io/gh/donmccaughey/starjumper


## Building From Source

Building _Starjumper_ requires that you system have a C toolchain and a recent
version of [CMake](https://cmake.org).

    git clone https://github.com/donmccaughey/starjumper.git
    cd starjumper
    cmake -S . -B tmp
    cmake --build tmp --target all test


## Utilities

**`worldgen`**: generate a world or a subsector.

Command line options:

        usage: worldgen [OPTIONS]
          -h, --help                       display this help message
          -b, --subsector                  generate a subsector
          -n, --name NAME                  set the world name
          -r, --rng TYPE                   set the random number generator type
                                             [arc4|ascending|fixed]
          -x, --hex XXYY                   set the world hex coordinate
                                     in the format "0101"

Examples:

        $ worldgen --name "Don's World" --hex 0304
        Don's World        0304 B59A9B9-9   High Population. Industrial. Water World. G

        $ worldgen --subsector
        Name               Statistics       Remarks
        No Name            0104 B300575-8 N Non-industrial. Vacuum World.             G
        No Name            0105 AA69479-B   Non-industrial.                            
        No Name            0106 A310201-C   Low Population. Non-industrial.            
        No Name            0107 D000000-0   As. Ba. Lo. Ni. Va.                       G
        No Name            0108 B221125-A A Low Population. Non-industrial. Poor.     G
        No Name            0202 X356741-1   Agricultural.                             G
        ...
        No Name            0806 B862458-8 A Non-industrial.                           G
        No Name            0807 A656978-B   High Population.                          G
        No Name            0810 C79A767-5   Water World.                              G


## License

_Starjumper_ is available under a BSD-style license.  See the [`LICENSE`][51]
file for details.

[51]: ./LICENSE
