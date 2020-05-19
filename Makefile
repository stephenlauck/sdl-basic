
all:
	gcc -o bin/main.out src/main.c `sdl2-config --cflags --libs`

run: all
	./bin/main.out
