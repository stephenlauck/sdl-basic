// Basic SDL Demo
#include <SDL2/SDL.h>
#include <iostream>

#include "system.h"
#include "input.h"

using namespace std;


int main () {

  System system;
  Input input;
  
  while(!input.quit) {

    input.GetInput();
    
    SDL_SetRenderDrawColor(system.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(system.renderer);
    SDL_RenderPresent(system.renderer);
  }

  system.Cleanup();
  
  return 0;
}
