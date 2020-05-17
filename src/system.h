#define SCREEN_W 640
#define SCREEN_H 480

class System
{

 public:
  SDL_Window *window;
  SDL_Renderer *renderer;  

  int screenW, screenH;

  System();
  ~System(){};

  void Setup();
  void Info();
  void Cleanup();

};

System::System()
{
  screenW = SCREEN_W;
  screenH = SCREEN_H;

  Setup();
  Info();
}

void System::Setup()
{
  // sdl init
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER);

  // create window
  window = SDL_CreateWindow("Basic Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_ALWAYS_ON_TOP);

  // create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

}

void System::Info()
{
  // controller info
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

void System::Cleanup()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
