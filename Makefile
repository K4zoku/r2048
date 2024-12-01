.DEFAULT_GOAL := default

include .make/project.mk

default: info clean build test

info: project_info platform_info

build: $(TARGET)

docs:
	doxide build

clean_target:
	$(RM) $(OBJ) $(TARGET)

clean_tests:
	$(RM) $(TEST_OBJ) $(TEST_BIN)

clean_docs:
	doxide clean

clean: clean_target clean_tests clean_coverage clean_docs

.PHONY: $(IMPLICIT_PHONY) default info build docs clean clean_tests clean_docs
