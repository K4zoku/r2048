# Template

A template for C library project.

## Features

- [x] Recursive source file search.
- [x] Build profiles for debug and release.
- [x] Cross-platform support.
- [x] Unit test support. (Unit test framework not included)
- [x] Run tests in parallel with `make -j`.
- [x] Documentation support. (Doxide)
- [x] Experimental code coverage support. (Doxide with gcov/llvm-cov)
- [x] Dependency management. (Configuring the `LDFLAGS` and `LDLIBS` is enough for most cases)


## Project Structure

```
.make/           # Makefile templates
build/           # Build output directory
  bin/           # Executable output directory
  lib/           # Library output directory
  obj/           # Object file output directory
  test/          # Unit test output directory
docs/            # Documentation files
include/         # Public header files
src/             # Source files
test/            # Unit test files
```
