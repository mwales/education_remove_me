#pragma once

#include <stdint.h>
#include <string>

#include <SDL2/SDL.h>

#include "GameMath.h"
#include "Joystick.h"

class Graphics
{
public:
   Graphics(uint32_t initFlags = SDL_INIT_EVERYTHING);

   ~Graphics();

   void CreateWindow(char const * title, int width, int height, uint32_t flags = SDL_WINDOW_OPENGL);

   void Clear();

   void Render() { SDL_RenderPresent(theRenderer); }

   inline SDL_Renderer* GetRenderer() { return theRenderer; }

   inline SDL_Window* GetWindow() { return theWindow; }

   XYPair GetWindowSize();

   Joystick* GetJoystick() { return &theJoystick; }

   void DisplayRendererInfo();

protected:
   SDL_Window* theWindow;

   SDL_Renderer* theRenderer;

   XYPair theWindowSize;

   Joystick theJoystick;
};

