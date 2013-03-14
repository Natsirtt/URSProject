GCC=gcc
RM=rm -f
LIBS=-lGLU -lGL -lm
LIBS_SDL=-lSDL_image `sdl-config --libs`
CFLAGS=--std=c99 -Wall
CFLAGS_SDL=`sdl-config --cflags` -Wall

all: urs

urs: urs.o planet.o sun.o space.o ship.o guLib/sdlKeyUtils.o guLib/glUtils.o guLib/intList.o guLib/fblList.o guLib/camera.o
	$(GCC) -o $@ $^ $(LIBS_SDL) $(LIBS)

tests.o: tests.c guLib/sdlKeyUtils.h guLib/glUtils.h guLib/camera.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

planet.o: planet.c planet.h guLib/camera.h guLib/glUtils.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

sun.o: sun.c sun.h guLib/glUtils.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

ship.o: ship.c ship.h guLib/glUtils.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

space.o: space.c space.h guLib/glUtils.h
	$(GCC) -c $(CFLAGS) $(CFLAGS_SDL) $<

camera.o:
	make --directory=guLib

sdlKeyUtils.o:
	make --directory=guLib

glUtils.o:
	make --directory=guLib

intList.o:
	make --directory=guLib

fblUtils.o:
	make --directory=guLib

clean:
	$(RM) *.{o,gch} & make --directory=guLib clean
