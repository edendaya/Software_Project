CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -fPIC -I/usr/include/python3.6m/
LDFLAGS = -lm -shared
TARGET = symnmf
SHARED_OBJECT = symnmfC.so
SOURCES = symnmf.c 

all: $(TARGET) $(SHARED_OBJECT)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

$(SHARED_OBJECT): $(SOURCES)
	$(CC) $(CFLAGS) -shared $(SOURCES) -o $(SHARED_OBJECT) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(SHARED_OBJECT)