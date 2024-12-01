.DEFAULT_GOAL := default

include .make/project.mk

default: info clean build test

.PHONY: $(IMPLICIT_PHONY) \
	default
