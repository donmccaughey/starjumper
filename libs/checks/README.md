# checks

A C99 library of assertion macros for unit testing.

[![builds.sr.ht status](https://builds.sr.ht/~donmcc/checks.svg)](https://builds.sr.ht/~donmcc/checks?)


## Building from Repository Source

Clone the repository, generate the build system and build.

    git clone https://git.sr.ht/~donmcc/checks
    cd checks
    cmake -S . -B tmp
    cmake --build tmp
    cmake --install tmp

To run tests:

    cmake --build tmp --target all test

### Build Options

To build with the [AddressSanitizer][41] enabled, set the `ADDRESS-SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Set the `BUILD_TESTING` option to `OFF` to disable tests.  (It's `ON` by
default.)

    cmake -S . -B tmp -DBUILD_TESTING=OFF

To build for code coverage, set the `COVERAGE` option to `ON`.

    cmake -S . -B tmp -DCOVERAGE=ON

Set the `WALL` option to `ON` turns on [additional warnings][42] using the 
`-Wall` compiler flag and also treats warnings as errors.  `WALL` is off by
default but is recommended for development and integration builds.

    cmake -S . -B tmp -DWALL=ON

[41]: https://clang.llvm.org/docs/LeakSanitizer.html
[42]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


## License
`checks` is made available under a BSD-style license; see the [LICENSE][91] file
for details.

[91]: ./LICENSE
