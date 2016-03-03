#include "Graphics.h"

#include "Logger.h"


Graphics::Graphics(uint32_t initFlags):
  theWindow(nullptr)
, theRenderer(nullptr)
{
   LOG_DEBUG() << "Starting graphics and SDL";

   if (0 != SDL_Init(initFlags))
   {
      LOG_FATAL() << "SDL Initialization failed:" << SDL_GetError();
   }

   theJoystick.AddJoystick();
}


void Graphics::CreateWindow(char const * title, int width, int height, uint32_t flags)
{
   theWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
   if (theWindow == nullptr)
   {
      LOG_FATAL() << "SDL Create Window failed:" << SDL_GetError();
      return;
   }

   theRenderer = SDL_CreateRenderer(theWindow, -1, SDL_RENDERER_ACCELERATED);
   if (theRenderer == nullptr)
   {
      LOG_FATAL() << "SDL Create Renderer failed:" << SDL_GetError();
      return;
   }

   // Set color
   if (0 != SDL_SetRenderDrawColor(theRenderer, 0xff, 0xff, 0xff, 0xff))
   {
      LOG_WARNING() << "Setting renderer draw color failed:" << SDL_GetError();
   }

   theWindowSize[0] = width;
   theWindowSize[1] = height;
}

Graphics::~Graphics()
{
   LOG_DEBUG() << "Closing graphics and SDL";

   theJoystick.CloseAllJoysticks();

   if (theRenderer)
   {
      SDL_DestroyRenderer(theRenderer);
   }

   if (theWindow)
   {
      SDL_DestroyWindow(theWindow);
   }

   SDL_Quit();
}

void Graphics::Clear()
{
   // No way we got in here with invalid pointers, bypassing null checks
   if (0 != SDL_RenderClear(theRenderer))
   {
      LOG_WARNING() << "Renderer clear failed:" << SDL_GetError();
   }

}

XYPair Graphics::GetWindowSize()
{
   return theWindowSize;
}

void Graphics::DisplayRendererInfo()
{
    SDL_RendererInfo info;
    if (0 != SDL_GetRendererInfo(theRenderer, &info))
    {
        LOG_WARNING() << "Error getting renderer info";
        return;
    }

    LOG_DEBUG() << "****************************************";
    LOG_DEBUG() << "* Renderer = " << info.name;
    LOG_DEBUG() << "* Flags:";

    if (info.flags & SDL_RENDERER_SOFTWARE)
    {
        LOG_DEBUG() << "*   Software Renderer";
    }

    if (info.flags & SDL_RENDERER_ACCELERATED)
    {
        LOG_DEBUG() << "*   Accelerated Renderer";
    }

    if (info.flags & SDL_RENDERER_PRESENTVSYNC)
    {
        LOG_DEBUG() << "*   Synced with refresh rate";
    }

    if (info.flags & SDL_RENDERER_TARGETTEXTURE)
    {
        LOG_DEBUG() << "*   Supports rendering to texture";
    }

    LOG_DEBUG() << "* Texture Formats: " << info.num_texture_formats;
    LOG_DEBUG() << "* Max Texture Size:" << info.max_texture_width << "x" << info.max_texture_height;

    LOG_DEBUG() << "****************************************";

}
