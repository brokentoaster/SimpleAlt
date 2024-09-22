TARGET = simple
CC = gcc
CFLAGS = -g -Wall

LIBS = -lm -lsimplealt -L../libsimplealt 
INC = -I../libsimplealt

#detect OS
UNAME_S:= $(shell uname -s)


# Platform-specific configurations
ifeq ($(UNAME_S),Linux)
else
 ifeq ($(UNAME_S),FreeBSD)
 endif
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC)  -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
