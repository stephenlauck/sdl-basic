
all:
	gcc -o bin/main.out src/main.c `sdl2-config --cflags --libs` -lSDL2_image

run: all
	./bin/main.out
