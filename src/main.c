#include "SDL2/SDL.h"

// 16:9 * 32 pixels
#define SCREEN_W 512
#define SCREEN_H 288
#define SPRITE_SCALE 4

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

int setup()
{
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
  }

  window = SDL_CreateWindow(
			    "Basic",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    SCREEN_W,
			    SCREEN_H,
			    SDL_WINDOW_ALWAYS_ON_TOP
			    );

  if(window == NULL) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

  if(renderer == NULL) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    return 1;
  }
 
  return 0;
}

int teardown()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

// load sprite from sheet at cx, cy
// render sprite at x,y
int sprite(int x, int y, int cx, int cy, int w, int h)
{
  
  SDL_Surface *surface;
  SDL_Rect clip = {cx, cy, w, h};

  // scale sprite
  w *= SPRITE_SCALE;
  h *= SPRITE_SCALE;

  // center if -1
  if (x < 0){ x = (SCREEN_W-w)/2; };
  if (y < 0){ y = (SCREEN_H-h)/2; };
  
  SDL_Rect draw = {x, y, w, h};

  surface = SDL_LoadBMP("data/metroid.bmp");

  if (surface == NULL) {
    SDL_Log("Unable to load BMP: %s", SDL_GetError());
    return 1;
  }

  // set color key for sprite sheet
  SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x74, 0x74, 0x74 ) );
  
  texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (texture == NULL) {
    SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
    return 1;
  }

  SDL_FreeSurface(surface);

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, &clip, &draw);
  
  return 0;
}
int main(int argc, char* argv[])
{
  setup();

  int quit = 0;

  while (!quit) {
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	quit = 1;
	break;
      }
    }
    // draw
    sprite(-1, -1, 128, 121, 16, 32);
    // render
    SDL_RenderPresent(renderer);
    //wait
  }

  teardown();

  return 0;
}
