#include<stdio.h>
#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
typedef struct{
  SDL_Renderer *renderer;
  SDL_Window *window;
  int running;
}GUI;
SDL_Color BLACK = {0,0,0,0};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Texture* getText(GUI* gui, SDL_Color cl,TTF_Font* fnt,char *str)
{
  SDL_Texture *texture;
	SDL_Surface *textSurface = NULL;
	textSurface = TTF_RenderText_Blended(fnt, str, cl);
	texture = SDL_CreateTextureFromSurface(gui->renderer,textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}
void getDigitTexture(GUI* gui, SDL_Texture** digitTexture)
{
  TTF_Font* fc20 = TTF_OpenFont("firacode.ttf", 20);
  char string[][2] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};
  for(int i = 0;i<10;i++)
    digitTexture[i] = getText(gui, BLACK, fc20, string[i]);
  TTF_CloseFont(fc20);
}
void init(GUI* gui, SDL_Texture** digitTexture)
{
  //LOAD PREVIOUS WINDOW SIZE
  printf("loading save...\n");
  int wh[2];
  FILE* fp = fopen("./save.bin", "rb");
  if(fp)
    fread(wh, sizeof(wh), 1, fp);
  else
  {wh[0]=100;wh[1]=100;}
  fclose(fp);
  printf("finished.\n");
  //ADD GET WINDOW POSTITION

  //INIT SDL AND ERROR CHECK
  printf("initializing SDL...\n");
  if(!SDL_Init(SDL_INIT_EVERYTHING)) printf("finished.\n");
  else printf("SDL init error:%s\n", SDL_GetError());

  //INIT WINDOW
  printf("initializing window...\n");
  int Wpos = SDL_WINDOWPOS_UNDEFINED;
  int window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;// | SDL_WINDOW_BORDERLESS;
  gui->window = SDL_CreateWindow("title",Wpos,Wpos,wh[1],wh[0],window_flags);

  //ERROR CHECK WINDOW INITIALIZED
  if(gui->window) printf("finished.\n");
  else printf("Window Init Error:%s\n", SDL_GetError());

  //INIT RENDERER
  int render_flag = SDL_RENDERER_ACCELERATED;
  gui->renderer = SDL_CreateRenderer(gui->window, -1, render_flag);

  //ERROR CHECK RENDERER
  if(gui->renderer) printf("initializing renderer...\n");
  else printf("Renderer Init Error:%s", SDL_GetError());
  printf("finished.\n");

  //INIT TTF
  if(TTF_Init()!=0)
    printf("ttf_init failed\n");
  //LOAD FONT
  printf("loading digitTextures...\n");
  getDigitTexture(gui, digitTexture);
  printf("finished.\n");

}
