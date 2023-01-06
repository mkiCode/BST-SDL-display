#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
struct node{
  int val;
  struct node* left;
  struct node* right;
}node;

typedef struct{
  SDL_Renderer *renderer;
  SDL_Window *window;
  int running;
}GUI;

typedef struct{
  int num;
  int digitCount;
  SDL_Rect* digits;
  SDL_Texture** textures;
  SDL_Rect* box;
}numBox;
