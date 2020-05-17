
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define SCALE 4
#define SHEET "img/metroid.bmp"

class Sprite {
 private:

 public:
  Sprite(){}
  ~Sprite()
  {
    free();
  }

  void free();
  
  int draw(int loc_x, int loc_y);
};

// Sprite Texture
SDL_Surface *surface;
SDL_Texture *texture;
SDL_Renderer *renderer;


void Sprite::free()
{
  SDL_DestroyTexture(texture);
}

int Sprite::draw(int loc_x, int loc_y)
{
  //rects for drawing sprite
  SDL_Rect srcr;
  SDL_Rect dstr;

  // sprite location and size
  srcr.x = 128;
  srcr.y = 121;
  srcr.w = 16;
  srcr.h = 32;

  // dstr.x = (WINDOW_WIDTH - srcr.w) / 2;
  // dstr.y = (WINDOW_HEIGHT - srcr.h) / 2;

  dstr.x = loc_x;
  dstr.y = loc_y;

  dstr.w = srcr.w * SCALE;
  dstr.h = srcr.h * SCALE;

  surface = SDL_LoadBMP(SHEET);
  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    return 3;
  }

  SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x74, 0x74, 0x74 ) );
  
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    return 3;
  }
  SDL_FreeSurface(surface);


  // draw
  
  SDL_RenderCopy(renderer, texture, &srcr, &dstr);


  return 0;
}

int main(int argc, char *argv[])
{
 
  SDL_Window *window;
  SDL_Event event;

  Sprite samus;
  Sprite samus2;
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window nor renderer: %s", SDL_GetError());
    return 3;
  }

  // main loop
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	quit = true;
	break;
      }
    }

    // draw
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x0 );
    SDL_RenderClear( renderer );

    samus.draw(100, 100);
    samus2.draw(200, 200);

    SDL_RenderPresent(renderer);    
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
