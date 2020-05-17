class Input
{
 public:
  SDL_Event event;
  bool quit;
  
  void GetInput();
};

void Input::GetInput()
{
  quit = false;
  
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      quit = true;
      break;
    }
  }
}
