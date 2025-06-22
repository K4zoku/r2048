.DEFAULT_GOAL := default

include .make/project.mk

all: info clean $(EXTERN_DIR)/libraylib.a build

default: all

.PHONY: $(IMPLICIT_PHONY) \
	all default
