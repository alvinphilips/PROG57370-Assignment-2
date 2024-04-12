CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell sdl2-config --cflags)
INCLUDES=-I ../Engine/External-Libs/RakNet/Include -I ../Engine/External-Libs/SDL/include -I ../Engine/Engine -I ./GameProject -L ../Engine/External-Libs/RakNet/Lib -L ../Engine/Engine-Libs
LIBS=-lengine $(shell sdl2-config --static-libs) -lraknet -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2main -luuid

all: gameproject

gameproject:
	$(CXX) $(CPPFLAGS) $(INCLUDES) ./GameProject/*.cpp -o GameProject.elf $(LIBS)
	mv GameProject.elf ./Build/bin

full:
	cd ../Engine && make
	make

clean:
	rm ./*.elf
