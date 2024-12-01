
# ------------------------- #
# PROJECT BASIC INFORMATION #
# ------------------------- #

# Name of the project
NAME:=template

# Output type, either `bin` or `lib`
TYPE:=lib

# Build profile, either `DEBUG` or `RELEASE`
BUILD_PROFILE:=DEBUG

project_info:
	@echo "  * Project name: $(NAME)"
	@echo "  * Project type: $(TYPE)"
	@echo "  * Build profile: $(BUILD_PROFILE)"

# --------------------------- #
# PROJECT DIRECTORY STRUCTURE #
# --------------------------- #

# Directory containing source files
SOURCE_DIRECTORY:=src

# Directory containing header files
INCLUDE_DIRECTORY:=include

# Directory containing test files
TEST_DIRECTORY:=test

# Directory containing build files
BUILD_DIRECTORY:=build

# Directory containing output object files
OBJECT_DIRECTORY:=$(BUILD_DIRECTORY)/obj

# Directory containing output library files
LIBRARY_DIRECTORY:=$(BUILD_DIRECTORY)/lib

# Directory containing output binary files
BINARY_DIRECTORY:=$(BUILD_DIRECTORY)/bin

# Directory containing output test binary files
TEST_BINARY_DIRECTORY:=$(BUILD_DIRECTORY)/$(TEST_DIRECTORY)

# ---------------------- #
# COMPILER CONFIGURATION #
# ---------------------- #

# Compiler to use
CC:=clang

# Compiler flags
CFLAGS:=-ansi -pedantic

# Preprocessor flags
CPPFLAGS:=-Wall -Wextra

# Linker flags
LDFLAGS:=

# Libraries to link
LDLIBS:=

# ------------------- #
# DEBUG CONFIGURATION #
# ------------------- #
#
# This section is used to define the compiler flags, preprocessor flags, linker
#     flags, and libraries to link when building the project in DEBUG profile.
# Those flags will be appended to CFLAGS, CPPFLAGS, LDFLAGS, and LDLIBS.

# Debug compiler flags
DEBUG_CFLAGS:=-Og

# Debug preprocessor flags
DEBUG_CPPFLAGS:=

# Debug linker flags
DEBUG_LDFLAGS:=

# Debug libraries to link
DEBUG_LDLIBS:=

# --------------------- #
# RELEASE CONFIGURATION #
# --------------------- #
#
# This section is used to define the compiler flags, preprocessor flags, linker
#     flags, and libraries to link when building the project in RELEASE profile.
# Those flags will be appended to CFLAGS, CPPFLAGS, LDFLAGS, and LDLIBS.

# Release compiler flags
RELEASE_CFLAGS:=-O2

# Release preprocessor flags
RELEASE_CPPFLAGS:=

# Release linker flags
RELEASE_LDFLAGS:=

# Release libraries to link
RELEASE_LDLIBS:=

# ------------------ #
# TEST CONFIGURATION #
# ------------------ #
#
# This section is used to define the compiler flags, preprocessor flags, linker
#    flags, and libraries to link when building the test binary.
# Those flags is independent from the main project flags and build profile.

# Test compiler flags
TEST_CFLAGS:=$(DEBUG_CFLAGS)

# Test preprocessor flags
TEST_CPPFLAGS:=

# Test linker flags
TEST_LDFLAGS:=

# Test libraries to link
TEST_LDLIBS:=

# ---------------------- #
# COVERAGE CONFIGURATION #
# ---------------------- #

# EXPERIMENTAL: Enable coverage report, supported only in DEBUG profile, and
# compiler must be GCC or Clang
ENABLE_COVERAGE:=1

# Coverage target file
COVERAGE_TARGET:=$(BUILD_DIRECTORY)/coverage.gcov

# Coverage tool to use (only used when ENABLE_COVERAGE is set to 1)
# Supported coverage tools:
# - `gcov` if `gcc` is the compiler
# - `llvm-cov gcov` if `clang` is the compiler
COV:=llvm-cov gcov

# ---------------- #
# DON'T EDIT BELOW #
# ---------------- #
IMPLICIT_PHONY+=project_info
include .make/project.auto.mk
