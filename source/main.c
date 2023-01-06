#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define SDL_MAIN_HANDLED
#include<structs.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

#define DIGIT_WIDTH  24
#define DIGIT_HEIGHT 48
#define SPACEING 10

void init(GUI* gui, SDL_Texture** digitTexture);
int handle_input(GUI* gui);
void close(GUI* gui, int exit_code);

numBox* getBox(int num, SDL_Texture** digitTexture)
{
  int negative;
  if(nums < 0) negative = 1;
  else negative = 0;
  num = abs(num);
  int power = 1;
  int digitCount = 1;
  int* digits;
  while((power*10)<=num){power*=10;digitCount++;}
  digits = malloc(sizeof(int)*digitCount);

  //INITIALIZE BOX
  numBox* box = malloc(sizeof(numBox));
          box->num        = num;
          box->box        = malloc(sizeof(SDL_Rect));
          box->digits     = malloc(sizeof(SDL_Rect)*digitCount);
          box->textures   = malloc(sizeof(SDL_Texture*)*digitCount);
          box->digitCount = digitCount;

  //GET DIGITS
  for(int i = 0; i < digitCount; i++)
  {
    digits[i] = num/power;
    num%=power;
    power/=10;
  }

  //SIZE BOX
  box->box->w = SPACEING;
  box->box->h = DIGIT_HEIGHT + SPACEING;
  for(int ii = 0; ii < digitCount; ii++)
  {
    //printf("digits[ii]:%d\n", digits[ii]);
    box->textures[ii] = digitTexture[digits[ii]];
    box->digits[ii].w = DIGIT_WIDTH;
    box->digits[ii].h = DIGIT_HEIGHT;
    box->box->w += DIGIT_WIDTH;
  }
  return box;
}
void boxPos(numBox* box, int x, int y)
{
  box->box->x = x;
  box->box->y = y;
  int digitPos = x+(SPACEING/2);
  for(int i  = 0; i < box->digitCount; i++)
  {
    box->digits[i].y = y + (SPACEING/2);
    box->digits[i].x = digitPos;
    digitPos+=DIGIT_WIDTH;
  }
}
void drawBox(GUI* gui, numBox* box)
{
    SDL_SetRenderDrawColor(gui->renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(gui->renderer, box->box);
    for(int i  = 0; i < box->digitCount; i++)
      SDL_RenderCopy(gui->renderer,box->textures[i],NULL,&box->digits[i]);
}
int* loadArr(int* size)
{
   FILE *fp;
   int end;
   int *arr;
   fp = fopen("arr.bin", "rb");
   fseek(fp, 0L, SEEK_END);
   end = ftell(fp);
   fseek(fp, 0L, SEEK_SET);
   *size = end/sizeof(int);
   arr = malloc(*size * sizeof(int));
   fread(arr, sizeof(int), *size, fp);
   fclose(fp);
   return arr;
}

//MAKE NODE FUNCTION.
void fillNode(struct node* n, int* val)
{
  if(n->val==*val);
  else if(n->val>*val)
    if (n->left)
      fillNode(n->left, val);
    else
    {
      n->left =  malloc(sizeof(node));
      n->left->val = *val;
      n->left->left = NULL;
      n->left->right = NULL;
    }
  else if(n->val<*val)
    if (n->right)
      fillNode(n->right, val);
    else
    {
      n->right =  malloc(sizeof(node));
      n->right->val = *val;
      n->right->left = NULL;
      n->right->right = NULL;
    }
  else;
}
struct node* makeTree(int* arr, int* size)
{
  struct node* n = malloc(sizeof(node));
  n->left = NULL;
  n->right = NULL;
  n->val = *arr;
  while(--(*size))
  {
    arr++;
    fillNode(n, arr);
  }
  return n;
}
void getDepth(struct node* n, int depth, int* finalDepth)
{
  depth++;
  //printf("depth:%d\n", depth);
  if(n->left)
    getDepth(n->left, depth, finalDepth);
  if(n->right)
    getDepth(n->right, depth, finalDepth);

  if((!n->left) && (!n->right))
    *finalDepth = (depth>*finalDepth?depth:*finalDepth);
}
int renderTree(
  GUI* gui,
  struct node* node,
  int pos, int width, int depth,
  SDL_Texture** digitTexture,
  int descend,
  int cornerX, int cornerY)
{
  numBox* box;
  box = getBox(node->val, digitTexture);
  boxPos(box, (width/2)+pos, depth);
  drawBox(gui, box);
  SDL_RenderDrawLine(gui->renderer,cornerX, cornerY,
  box->box->x+(box->box->w/2), box->box->y);
  if(descend)
  {
    if(node->left)
      renderTree(gui, node->left, pos, width/2,
                 depth+70, digitTexture, descend-1,
                 box->box->x, box->box->y+box->box->h);
    if(node->right)
      renderTree(gui, node->right, pos+(width/2),
                 width/2, depth+70, digitTexture,
                 descend-1,
                 box->box->x+box->box->w, box->box->y+box->box->h);
  }
}
void run(GUI* gui, SDL_Texture** digitTexture)
{
  int arrSize;
  int* arr = loadArr(&arrSize);
  struct node* tree =  makeTree(arr, &arrSize);
  int depth = 0;
  getDepth(tree, 0, &depth);
  int setDepth = 3;
  int width = 1;//SHOULD DO THIS WITHIN DEPTH FUNCTION
  for(int i = 0; i < setDepth; i++)
    width*=2;

  int x = 0;
  int y = 0;
  int i = 0;
  while(gui->running)
  {
    SDL_SetRenderDrawColor(gui->renderer, 100, 100, 100, 255);
    SDL_RenderClear(gui->renderer);
    renderTree(gui, tree, x, width*100, y, digitTexture, setDepth, ((width*100)/2)+x, y);
    i=i==100?0:i+1;//if i is 100 reset to 0 else iterate by 1
    switch(handle_input(gui))
    {
      case 1:x-=100;break;
      case 2:x+=100;break;
      case 3:y-=100;break;
      case 4:y+=100;break;
      case 5:setDepth++;width*=2;break;
      default:break;
    }
    SDL_RenderPresent(gui->renderer);
    SDL_Delay(100);
  }
  close(gui, EXIT_SUCCESS);
}
int main()
{
  GUI gui = {.running=1};
  SDL_Texture** digitTextures = malloc(sizeof(SDL_Texture*)*10);

  init(&gui, digitTextures);
  run(&gui, digitTextures);
  SDL_Quit();

  return 0;
}
