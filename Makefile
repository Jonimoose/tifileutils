CC=gcc
CFLAGS= -c -O2 -Wall -I/usr/include/tilp2  -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
LDFLAGS= -ltifiles2
SOURCES=tigrp.c timod.c
OBJECTS=$(SOURCES:.c=.o)
programs = tigrp \
	   timod
EXECUTABLE=$(SOURCES:.c=)

all: $(programs)

clean:
	rm $(programs)
	rm  $(OBJECTS)
	
$(programs): $(OBJECTS) 
	$(CC) $(LDFLAGS) $@.o -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

