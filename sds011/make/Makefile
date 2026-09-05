CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=sds011_driver.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sds011_driver

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm -rf $(EXECUTABLE) $(OBJECTS)
