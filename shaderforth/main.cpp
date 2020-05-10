#ifndef _SDL_main_h
#define _SDL_main_h
#endif

#include <SDL.h>

#include <stdio.h>
#include <string.h>
#include <iostream>

#include "view.h"

int main(int /*argc*/, char** /*argv*/)
  {
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initilizated SDL Failed: %s", SDL_GetError());
    return -1;
    }


  std::string dummy_code = R"(u
v
t sin
1 + 2 /
)";

  {
  view my_view;

  my_view.set_shader_code(dummy_code);

  my_view.loop();
  }

  SDL_Quit();
  return 0;
  }