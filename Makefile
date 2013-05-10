C = cc

SOURCES = src/main.c

CFLAGS = -Wall -O2 
LDFLAGS = -lX11 


OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = snake

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(C) $(OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

cleanall: clean

clean:
	-rm $(EXECUTABLE) $(OBJECTS)

