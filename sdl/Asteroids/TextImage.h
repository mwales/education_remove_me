#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ImageInfo.h"

/**
 * Renders text messages into textures for display
 *
 * Fonts are cached so we don't hit the disk everytime we write text on the screen
 */
class TextImage : public ImageInfo
{
public:
   TextImage(std::string text, SDL_Color color, std::string font, int pt, SDL_Renderer* renderer);

   TextImage(std::string text, SDL_Color color, SDL_Renderer* renderer);

   static bool LoadFont(std::string font, int pt);

   static void UnloadAllFonts();

protected:

   void ProcessSurface(SDL_Surface* surface);

   TTF_Font* GetFont(std::string font, int pt);

   static bool _runTtfInitOnce;

   static std::map<std::string, TTF_Font*> _fonts;
};

