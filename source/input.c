#include<stdio.h>
#include<stdlib.h>
#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
typedef struct{
  SDL_Renderer *renderer;
  SDL_Window *window;
  int running;
}GUI;
int handle_input(GUI* gui)
{
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  SDL_Event event;

  while(SDL_PollEvent(&event))
    if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
      gui->running = 0;
    else;
  if(keystate[SDL_SCANCODE_M] && keystate[SDL_SCANCODE_LCTRL])
      printf("move_state\n");
  if(keystate[SDL_SCANCODE_RIGHT])
    return 1;
  else if(keystate[SDL_SCANCODE_LEFT])
    return 2;
  else if(keystate[SDL_SCANCODE_DOWN])
    return 3;
  else if(keystate[SDL_SCANCODE_UP])
    return 4;
  else if(keystate[SDL_SCANCODE_RETURN])
    return 5;


    return 0;
}
