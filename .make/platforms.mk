# Define the platform and architecture if not already defined
# Allowing the user to override the platform and architecture
ifndef $(and,$(PLATFORM),$(ARCHITECTURE))
    ifeq ($(OS),Windows_NT)
        PLATFORM:=Windows
        ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
            ARCHITECTURE:=AMD64
        else
            ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
                ARCHITECTURE:=AMD64
            else ifeq ($(PROCESSOR_ARCHITECTURE),x86)
                ARCHITECTURE:=IA32
            else
                $(warning Could not determine architecture)
            endif
        endif
    else
        UNAME_S:=$(shell uname -s)
        ifeq ($(UNAME_S),Linux)
            PLATFORM:=Linux
        else ifeq ($(UNAME_S),Darwin)
            PLATFORM:=OSX
        else
            $(warning Could not determine platform)
        endif

        UNAME_P:=$(shell uname -p)
        ifeq ($(UNAME_P),x86_64)
            ARCHITECTURE:=AMD64
        else ifneq ($(filter %86,$(UNAME_P)),)
            ARCHITECTURE:=IA32
        else ifneq ($(filter arm%,$(UNAME_P)),)
            ARCHITECTURE:=ARM
        endif # ifeq ($(UNAME_P),x86_64)
    endif # ifeq ($(OS),Windows_NT)
endif # ifndef $(and,$(PLATFORM),$(ARCHITECTURE))

platform_info:
	@echo "  * Platform: $(or $(PLATFORM),Unknown)"
	@echo "  * Architecture: $(or $(ARCHITECTURE),Unknown)"

IMPLICIT_PHONY+=platform_info
