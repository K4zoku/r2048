
# Function to recursively find files matching a pattern, work just like `wildcard` but with recursive.
rwildcard=$(foreach d,$(wildcard $(1)/*),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
