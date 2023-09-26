# Makefile for symnmf

# Compiler and flags
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

# Source files
SRCS = symnmf.c symnmfmodule.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = symnmf

# Build rule
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm -lpython3.x  # Replace '3.x' with your Python version

# Dependency generation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: clean