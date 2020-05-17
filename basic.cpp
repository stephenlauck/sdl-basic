// Basic SDL Demo
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCALE 3
#define SHEET "img/metroid.bmp"

// PROTOTYPE:
// SpriteSheet metroid ("img/metroid");
// Sprite samus ( clip_x, clip_y, clip_w, clip_h );
// samus.move((SCREEN_WIDTH - samus.w)/2, (SCREEN_HEIGHT - samus.h)/2);
// samus.draw();

class Sprite {
 private:
  SDL_Rect clip_rect;
  SDL_Rect draw_rect;
  
 public:
  Sprite(int clip_x, int clip_y, int clip_w, int clip_h);
  ~Sprite();

  void free();
  void move(int x, int y);
  int draw();
};

// Sprite Texture
SDL_Surface *surface;
SDL_Texture *texture;
SDL_Renderer *renderer;

Sprite::Sprite(int clip_x, int clip_y, int clip_w, int clip_h)
{
  clip_rect = { clip_x, clip_y, clip_w, clip_h };
  draw_rect = { (SCREEN_WIDTH - clip_w)/2, (SCREEN_HEIGHT - clip_h)/2, clip_w*SCALE, clip_h*SCALE };
}

Sprite::~Sprite()
{
  free();
}

void Sprite::free()
{
  SDL_DestroyTexture(texture);
}

void Sprite::move(int x, int y)
{
  draw_rect.x = x;
  draw_rect.y = y;
}

int Sprite::draw()
{
  SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x74, 0x74, 0x74 ) );
  
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    return 3;
  }
  
  SDL_RenderCopy(renderer, texture, &clip_rect, &draw_rect);

  return 0;
}

int loadSheet()
{
  surface = SDL_LoadBMP(SHEET);
  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    return 3;
  }

  return 0;
}

int main(int argc, char *argv[])
{
 
  SDL_Window *window;
  SDL_Event event;

  loadSheet();
  Sprite samus (128, 121, 16, 32);
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
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

    // samus.move(100, 100);
    samus.draw();
    
    SDL_RenderPresent(renderer);    
  }


  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
