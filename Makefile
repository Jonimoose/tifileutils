CC=gcc
CFLAGS= -c -Wall -I/usr/include/tilp2
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

