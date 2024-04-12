CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -c $(shell sdl2-config --cflags)
LDFLAGS=-static
INCLUDES=-I ./External-Libs/RakNet/Include -I ./External-Libs/SDL/include -I ./Engine -L ./External-Libs/RakNet/Lib
LIBS=$(shell sdl2-config --libs) -lraknet -lSDL2_image -lSDL2_ttf -lSDL2_mixer

all: engine

engine:
	$(CXX) $(CPPFLAGS) $(INCLUDES) ./Engine/*.cpp $(LIBS)
	ar rcs libengine.a *.o
	mv libengine.a ./Engine-Libs
	mv *.o ./Intermediate

clean:
	$(RM) ./*.o
	$(RM) ./Intermediate/*
	$(RM) ./Engine-Libs/libengine.a
