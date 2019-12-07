#include <SDL.h>
#include <iostream>
#include <string.h>

#define SDL_CHECK(var) if (var) { std::cout << "Error on line" << __LINE__ << ":" << SDL_GetError(); }

int main(int argc, char** argv)
{
   std::cout << "Starting" << std::endl;

   SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));

   std::cout << "Creating window" << std::endl;

   SDL_Window* w;

   w = SDL_CreateWindow("EMU 6502", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640 , 640 ,0);

   if (w == NULL)
   {
      std::cout << "Create window failed" << std::endl;
      return 1;
   }

   std::cout << "Create renderer" << std::endl;

   SDL_Renderer* r;
   r = SDL_CreateRenderer(w, -1, SDL_RENDERER_SOFTWARE);
   if (r == NULL)
   {
      std::cout << "Create renderer failed" << std::endl;
      return 2;
   }

   SDL_CHECK(SDL_RenderSetLogicalSize(r, 32 , 32 ));
   SDL_CHECK(SDL_RenderSetLogicalSize(r, 32 , 32 ));
   SDL_CHECK(SDL_SetRenderDrawColor(r, 0x00 , 0x00 , 0x00 ,SDL_ALPHA_OPAQUE));
   SDL_CHECK(SDL_RenderClear(r));

   std::cout << "About to enter the loop" << std::endl;

   while(true)
   {
      std::cout << ".";

      SDL_RenderPresent(r);

      SDL_Event ev;
      memset(&ev, 0, sizeof(SDL_Event));

      int status = SDL_WaitEventTimeout(&ev,1000);


      if (status == 0)
      {
         // do nothing
         std::cout << "X";
         continue;
      }

      std::cout << "0" << std::endl;


      if (ev.type == SDL_QUIT)
      {
         std::cout << "Quit event" << std::endl;
         break;
      }
   }
   //[ 502/src/Display.cpp: 208]: SDL_RenderSetLogicalSize(pointer, 32 , 32 )
   //[ 502/src/Display.cpp: 229]: SDL_SetRenderDrawColor(pointer, 0x00 , 0x00 , 0x00 ,SDL_ALPHA_OPAQUE)
   //[ 502/src/Display.cpp: 243]: SDL_RenderClear(pointer)


   return 0;
}


//SDL_CreateWindow("EMU 6502", SDL_WINDOWPOS_CENTERED, SDLWINDOWPOS_CENTERED, 640 , 640 ,0)
//  [ 502/src/Display.cpp: 187]: SDL_CreateRenderer(pointer, -1, SDL_RENDERER_SOFTWARE)
//[ 502/src/Display.cpp: 208]: SDL_RenderSetLogicalSize(pointer, 32 , 32 )
//[ 502/src/Display.cpp: 229]: SDL_SetRenderDrawColor(pointer, 0x00 , 0x00 , 0x00 ,SDL_ALPHA_OPAQUE)
//[ 502/src/Display.cpp: 243]: SDL_RenderClear(pointer)
//[ 502/src/Display.cpp:  95]: SDL_WaitEventTimeout(pointer,1000)
//[ 502/src/Display.cpp:  95]: SDL_WaitEventTimeout(pointer,1000)
//[ 502/src/Display.cpp:  95]: SDL_WaitEventTimeout(pointer,1000)
