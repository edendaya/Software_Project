CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm  
TARGET = symnmf
SOURCES = symnmf.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
