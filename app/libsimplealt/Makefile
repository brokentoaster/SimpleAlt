TARGET=simplealt
TARGET_LIB_STATIC = lib$(TARGET).a
TARGET_LIB_DYNAMIC = lib$(TARGET).so

CC = gcc
CFLAGS = -g -Wall -fPIC
AR = ar
ARFLAGS = rcs

INSTALL_DIR = /usr/local/lib/


#detect OS
UNAME_S:= $(shell uname -s)

# Platform-specific configurations
LIBS = -lm -lserialport -Wl,-soname,$(TARGET_LIB_DYNAMIC) -lc
ifeq ($(UNAME_S),Linux)
else
 ifeq ($(UNAME_S),FreeBSD)
    LIBS += -lusb 
 endif
endif

.PHONY: default all clean install 
.PRECIOUS: $(TARGET) $(OBJECTS) $(TARGET_LIB_STATIC) $(TARGET_LIB_DYNAMIC)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)


all: default

default: $(TARGET_LIB_STATIC) $(TARGET_LIB_DYNAMIC)

clean:
	-rm -f *.o
	-rm -f $(TARGET_LIB_DYNAMIC)
	-rm -f $(TARGET_LIB_STATIC)    

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET_LIB_STATIC): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(TARGET_LIB_DYNAMIC): $(TARGET_LIB_STATIC)
	$(CC) $(OBJECTS) -Wall $(LIBS) -shared -o $@ 

# You probably need to be sudo to run the install target
install: $(TARGET_LIB_DYNAMIC)
	install --owner=root --mode=644 --group=root $(TARGET_LIB_DYNAMIC) $(INSTALL_DIR)
	ldconfig

uninstall:
	rm $(INSTALL_DIR)$(TARGET_LIB_DYNAMIC)
	ldconfig
