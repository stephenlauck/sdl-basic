# g++ -o bin/main.out main.cpp `sdl2-config --cflags --libs` && ./bin/main.out

all:
	g++ -o bin/main.out src/main.cpp `sdl2-config --cflags --libs`

run: all
	./bin/main.out
