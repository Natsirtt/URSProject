GCC=gcc
RM=rm -f
LIBS=-lGLU -lGL -lm
LIBS_SDL=-lSDL_image -lSDL_ttf `sdl-config --libs`
CFLAGS=--std=c99 -Wall
CFLAGS_SDL=`sdl-config --cflags` -Wall

all: sdlKeyUtils.o glUtils.o intList.o fblList.o camera.o

camera.o: camera.h camera.c
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

sdlKeyUtils.o: sdlKeyUtils.h sdlKeyUtils.c intList.h fblList.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

intList.o: intList.h intList.c
	$(GCC) -c $(CFLAGS) $<

fblList.o: fblList.h fblList.c
	$(GCC) -c $(CFLAGS) $<

glUtils.o: glUtils.h glUtils.c
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

clean:
	$(RM) *.{o,gch}
