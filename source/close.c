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
void close(GUI* gui, int exit_code)
{
  int width = 0;
  int height = 0;
  SDL_GetWindowSize(gui->window, &width, &height);
  FILE *fp;
  fp = fopen("./save.bin", "wb");
  fwrite(&height, sizeof(height), 1, fp);
  fwrite(&width, sizeof(width), 1, fp);
  fclose(fp);

  if(gui->renderer)
    SDL_DestroyRenderer(gui->renderer);
  if(gui->window)
    SDL_DestroyWindow(gui->window);
  SDL_Quit();
  exit(exit_code);
}
