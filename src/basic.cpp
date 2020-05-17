// Basic SDL Demo
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCALE 3
#define SHEET "img/metroid.bmp"
#define SPEED 100

// PROTOTYPE:
// SpriteSheet metroid ("img/metroid");
// Sprite samus ( clip_x, clip_y, clip_w, clip_h );
// samus.move((SCREEN_WIDTH - samus.w)/2, (SCREEN_HEIGHT - samus.h)/2);
// samus.draw();

class Sprite {
 private:
  SDL_Rect clip_rect;
  SDL_Rect draw_rect;

  float x_pos, y_pos;
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
  x_pos = (SCREEN_WIDTH - clip_w) / 2;
  y_pos = (SCREEN_HEIGHT - clip_h) / 2;

  clip_rect = { clip_x, clip_y, clip_w, clip_h };
  draw_rect = { (int) x_pos, (int) y_pos, clip_w*SCALE, clip_h*SCALE };
}

Sprite::~Sprite()
{
  free();
}

void Sprite::free()
{
  SDL_DestroyTexture(texture);
}

void Sprite::move(int x_vel, int y_vel)
{
  // update positions
  x_pos += x_vel / 60;
  y_pos += y_vel / 60;

  // collision detection
  if (x_pos <= 0) x_pos = 0;
  if (y_pos <= 0) y_pos = 0;
  if (x_pos >= SCREEN_WIDTH - draw_rect.w) x_pos = SCREEN_WIDTH - draw_rect.w;
  if (y_pos >= SCREEN_HEIGHT - draw_rect.h) y_pos = SCREEN_HEIGHT - draw_rect.h;
  
  draw_rect.x = (int) x_pos;
  draw_rect.y = (int) y_pos;
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

void sdl_info()
{

  // show SDL2 Version
  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  printf("We compiled against SDL version %d.%d.%d ...\n",
        compiled.major, compiled.minor, compiled.patch);
  printf("But we are linking against SDL version %d.%d.%d.\n",
        linked.major, linked.minor, linked.patch);

  // Show Controller Info
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
}

int main(int argc, char *argv[])
{
 
  SDL_Window *window;
  SDL_Event event;

  loadSheet();
  Sprite samus (128, 121, 16, 32);
  Sprite rocket (277, 172, 16, 8);
  
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer) <0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window nor renderer: %s", SDL_GetError());
    return 3;
  }

  sdl_info();
  
  // main loop
  bool quit = false;

  int up, down, left, right = 0;
  float x_vel, y_vel = 0;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	quit = true;
	break;
      case SDL_CONTROLLERBUTTONDOWN:
        switch ( event.cbutton.button ) {
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
          up = 1;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
          down = 1;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
          left = 1;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
          right = 1;
          break;
        }
	break;
      case SDL_CONTROLLERBUTTONUP:
        switch ( event.cbutton.button ) {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
          up = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
          down = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
          left = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
          right = 0;
          break;
        }
	break;
      }
    }

    // determine velocity
    if (up && !down) y_vel = -SPEED;
    if (down && !up) y_vel = SPEED;
    if (left && !right) x_vel = -SPEED;
    if (right && !left) x_vel = SPEED;
    
    // samus.move(100, 100);
    samus.move(x_vel, y_vel);
    
    // draw
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );

    samus.draw();
    rocket.draw();
    
    SDL_RenderPresent(renderer);    
  }


  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
