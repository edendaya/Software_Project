# Makefile for symnmf

# Compiler and flags
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

# Source files
SRCS = symnmf.c main.c  # Add more source files if needed

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = symnmf

# Build rule
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Dependency generation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: clean
