#include "Graphics.h"

#include "Logger.h"


Graphics::Graphics(uint32_t initFlags):
  _window(NULL)
, _renderer(NULL)
{
   LOG_DEBUG() << "Starting graphics and SDL";

   if (0 != SDL_Init(initFlags))
   {
      LOG_FATAL() << "SDL Initialization failed:" << SDL_GetError();
   }

   _joystick.AddJoystick();
}


void Graphics::CreateWindow(char const * title, int width, int height, uint32_t flags)
{
   _window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
   if (_window == NULL)
   {
      LOG_FATAL() << "SDL Create Window failed:" << SDL_GetError();
      return;
   }

   _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
   if (_renderer == NULL)
   {
      LOG_FATAL() << "SDL Create Renderer failed:" << SDL_GetError();
      return;
   }

   // Set color
   if (0 != SDL_SetRenderDrawColor(_renderer, 0xff, 0xff, 0xff, 0xff))
   {
      LOG_WARNING() << "Setting renderer draw color failed:" << SDL_GetError();
   }

   _windowSize[0] = width;
   _windowSize[1] = height;
}

Graphics::~Graphics()
{
   LOG_DEBUG() << "Closing graphics and SDL";

   _joystick.CloseAllJoysticks();

   if (_renderer)
   {
      SDL_DestroyRenderer(_renderer);
   }

   if (_window)
   {
      SDL_DestroyWindow(_window);
   }

   SDL_Quit();
}

void Graphics::Clear()
{
   // No way we got in here with invalid pointers, bypassing null checks
   if (0 != SDL_RenderClear(_renderer))
   {
      LOG_WARNING() << "Renderer clear failed:" << SDL_GetError();
   }

}

XYPair Graphics::GetWindowSize()
{
   return _windowSize;
}

void Graphics::DisplayRendererInfo()
{
    SDL_RendererInfo info;
    if (0 != SDL_GetRendererInfo(_renderer, &info))
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
