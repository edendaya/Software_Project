# Makefile for building symnmfC Python extension module

# Compiler and flags
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
# Python includes and library
PYTHON_INCLUDE = $(shell python3-config --cflags)
PYTHON_LIB = $(shell python3-config --ldflags)

# Extension module sources
SRC = symnmf.c symnmfmodule.c
# Extension module name
EXT_MODULE = symnmfC

# Build target
all: $(EXT_MODULE).so

# Compile the extension module
$(EXT_MODULE).so: $(SRC)
	$(CC) $(CFLAGS) $(PYTHON_INCLUDE) -shared -o $@ $(SRC) $(PYTHON_LIB)

# Clean the generated files
clean:
	rm -f $(EXT_MODULE).so

.PHONY: all clean
