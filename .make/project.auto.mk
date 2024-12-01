include .make/functions.mk
include .make/platforms.mk

ifneq ($(ARCHITECTURE),)
    CFLAGS:=-D $(ARCHITECTURE) $(CFLAGS)
endif
ifneq ($(PLATFORM),)
    CFLAGS:=-D $(PLATFORM) $(CFLAGS)
endif
CPPFLAGS:=-I$(INCLUDE_DIRECTORY) $(CPPFLAGS)

ifeq ($(PLATFORM),Windows)
    LIB_EXT:=dll
    BIN_EXT:=exe
else
    LIB_EXT:=so
endif

ifeq ($(TYPE),lib)
    TARGET:=$(LIBRARY_DIRECTORY)/lib$(NAME).$(LIB_EXT)
    LDFLAGS+=-shared
    ifeq ($(PLATFORM),Windows)
        LDFLAGS+=-Wl,--out-implib,$(LIBRARY_DIRECTORY)/$(NAME).lib
    else
        CFLAFS+=-fPIC
        ifeq ($(PLATFORM),Linux)
            TEST_LDFLAGS+=-Wl,-rpath=$(LIBRARY_DIRECTORY)
        endif
    endif
else ifeq ($(TYPE),bin)
    TARGET:=$(NAME)
    ifdef BIN_EXT
        TARGET:=$(TARGET).$(BIN_EXT)
    endif
else
    $(error Invalid target type)
endif

ifeq ($(BUILD_PROFILE),RELEASE)
    CFLAGS+=-D NDEBUG
else ifeq ($(BUILD_PROFILE),DEBUG)
    CFLAGS+=-g
    ifeq ($(ENABLE_COVERAGE),1)
        CFLAGS+=--coverage -O0 -fno-inline
        TEST_CFLAGS+=--coverage -O0 -fno-inline
    endif
else
    $(error Invalid build profile. Expected `DEBUG` or `RELEASE`, but got `$(BUILD_PROFILE)`)
endif

CFLAGS+=$($(BUILD_PROFILE)_CFLAGS)
CPPFLAGS+=$($(BUILD_PROFILE)_CPPFLAGS)
LDFLAGS+=$($(BUILD_PROFILE)_LDFLAGS)
LDLIBS+=$($(BUILD_PROFILE)_LDLIBS)

SRC:=$(call rwildcard,$(SOURCE_DIRECTORY),*.c)
OBJ:=$(patsubst $(SOURCE_DIRECTORY)/%.c,$(OBJECT_DIRECTORY)/%.o,$(SRC))

TEST_SRC:=$(call rwildcard,$(TEST_DIRECTORY),*.c)
TEST_OBJ:=$(patsubst $(TEST_DIRECTORY)/%.c,$(OBJECT_DIRECTORY)/%.o,$(TEST_SRC))
TEST_BIN:=$(patsubst $(OBJECT_DIRECTORY)/%.o,$(TEST_BINARY_DIRECTORY)/%,$(TEST_OBJ))

ifeq ($(TYPE),bin)
    TEST_OBJ:=$(OBJ) $(TEST_OBJ)
endif

# Automatically link the library to the test binary if the target is a library
ifeq ($(TYPE),lib)
    ifeq ($(TEST_CPPFLAGS),)
        TEST_CPPFLAGS:=-I$(INCLUDE_DIRECTORY)
    else
        TEST_CPPFLAGS:=-I$(INCLUDE_DIRECTORY) $(TEST_CPPFLAGS)
    endif
    TEST_LDFLAGS:=-L$(LIBRARY_DIRECTORY) $(TEST_LDFLAGS)
    TEST_LDLIBS:=-l$(NAME) $(TEST_LDLIBS)
endif

$(OBJ): $(SRC)
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJECT_DIRECTORY)/%.o: $(TEST_DIRECTORY)/%.c
	@mkdir -p $(@D)
	$(CC) $(TEST_CPPFLAGS) $(TEST_CFLAGS) -c -o $@ $<


$(TEST_BINARY_DIRECTORY)/%: $(OBJECT_DIRECTORY)/%.o
	@mkdir -p $(@D)
	@echo "Building test '$(notdir $<)'..."
	$(CC) $(TEST_CPPFLAGS) $(TEST_CFLAGS) $(TEST_LDFLAGS) -o $@ $(OBJECT_DIRECTORY)/$*.o $(TEST_LDLIBS)

ifeq ($(TYPE),bin)
run: $(TARGET)
	@echo "Executing '$(notdir $<)'..."
	@$(abspath $<)

IMPLICIT_PHONY+=run
endif

# Build tests binary
tests: $(TEST_OBJ) $(TEST_BIN)

IMPLICIT_PHONY+=tests

test_%: $(TEST_BINARY_DIRECTORY)/%
	@echo "Running test '$(notdir $<)'..."
	@$(abspath $<)
	@echo "Test '$(notdir $<)' passed."

test: $(patsubst $(TEST_BINARY_DIRECTORY)/%,test_%,$(TEST_BIN))

IMPLICIT_PHONY+=test

ifeq ($(ENABLE_COVERAGE),1)
# Collate coverage report
GCNO:=$(OBJ:.o=.gcno) $(TEST_OBJ:.o=.gcno)
GCDA:=$(patsubst %.gcno,%.gcda,$(GCNO))

coverage: $(GCDA)
	$(COV) --stdout $(GCDA) > $(COVERAGE_TARGET)

$(GCDA): $(GCNO)

$(GCNO): $(OBJ) $(TEST_OBJ)

clean_coverage:
	$(RM) $(GCNO) $(GCDA)

IMPLICIT_PHONY+=coverage clean_coverage

endif
