#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

// 16:9 * 32 pixels
#define SCREEN_W 512
#define SCREEN_H 288
#define SPRITE_SCALE 4
#define ACCELERATION 10

struct Sprite {
  // location and size in sprite sheet
  SDL_Rect clip;
  // drawing location and size
  SDL_Rect draw;
};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

int system_info()
{
  // SDL Version
  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  printf("We compiled against SDL version %d.%d.%d ...\n",
	 compiled.major, compiled.minor, compiled.patch);
  printf("But we are linking against SDL version %d.%d.%d.\n\n",
	 linked.major, linked.minor, linked.patch);

  // SDL_image Info
  SDL_version compile_version;
  const SDL_version *link_version=IMG_Linked_Version();
  SDL_IMAGE_VERSION(&compile_version);
  printf("compiled with SDL_image version: %d.%d.%d\n", 
	 compile_version.major,
	 compile_version.minor,
	 compile_version.patch);
  printf("running with SDL_image version: %d.%d.%d\n\n", 
	 link_version->major,
	 link_version->minor,
	 link_version->patch);

  // https://wiki.libsdl.org/SDL_GameControllerMapping
  SDL_GameController *ctrl;
  int i;

  for (i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i)) {
      char *mapping;
      SDL_Log("Index \'%i\' is a compatible controller, named \'%s\'", i, SDL_GameControllerNameForIndex(i));
      ctrl = SDL_GameControllerOpen(i);
      mapping = SDL_GameControllerMapping(ctrl);
      SDL_Log("Controller %i is mapped as \"%s\".", i, mapping);
      SDL_free(mapping);
    } else {
      SDL_Log("Index \'%i\' is not a compatible controller.", i);
    }
  }
  return 0;
}

int setup()
{
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }
  	
  // load support for PNG image formats
  int flags=IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if((initted&flags) != flags) {
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());

    return 1;
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

  // show system info
  system_info();
  
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
int draw_sprite(int x, int y, int cx, int cy, int w, int h)
{
  
  SDL_Surface *surface;

  struct Sprite sprite = {
			  // clip
			  {cx, cy, w, h},
			  // draw
			  {x, y, w, h}
  };

  // scale sprite
  sprite.draw.w *= SPRITE_SCALE;
  sprite.draw.h *= SPRITE_SCALE;

  // center if -1
  if (x < 0){ sprite.draw.x = (SCREEN_W-sprite.draw.w)/2; };
  if (y < 0){ sprite.draw.y = (SCREEN_H-sprite.draw.h)/2; };
  
  surface = IMG_Load("data/metroid.png");

  if (surface == NULL) {
    SDL_Log("Unable to load PNG: %s", SDL_GetError());
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
  SDL_RenderCopy(renderer, texture, &sprite.clip, &sprite.draw);
  
  return 0;
}
int main(int argc, char* argv[])
{
  setup();

  int quit = 0;

  int sx = 0;
  int sy=0;
  
  while (!quit) {
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	quit = 1;
	break;
      case SDL_CONTROLLERBUTTONDOWN:
	switch (event.cbutton.button) {
	case SDL_CONTROLLER_BUTTON_BACK:
          quit = 1;
          break;
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
	  sy -= ACCELERATION;
	  break;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
	  sy += ACCELERATION;
	  break;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
	  sx -= ACCELERATION;
	  break;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
	  sx += ACCELERATION;
	  break;
	}	
      }
    }
    // draw
    draw_sprite(sx, sy, 128, 121, 16, 32);
    // render
    SDL_RenderPresent(renderer);
    //wait
  }

  teardown();

  return 0;
}
