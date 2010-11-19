CC=gcc
CFLAGS= -c -Wall -I/usr/include/tilp2
LDFLAGS= -ltifiles2
SOURCES=tigroup.c timod.c
OBJECTS=$(SOURCES:.c=.o)
programs = tigroup \
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

