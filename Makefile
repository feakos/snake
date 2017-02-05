#változtók deklarálása
CC := gcc
CFLAGS := -Wall
SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)
CFLAGS += $(SDL_CFLAGS)
LDFLAGS += $(SDL_LDFLAGS)

BINARIES := snake

all: $(BINARIES)

snake: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LDFLAGS)

#$@ -> bit - target
#$^ -> bit.o - dependency lista, ami a : jobb oldala

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(BINARIES)

.PHONY: all clean
#ezek nem fájlok!