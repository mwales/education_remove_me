#include "TextImage.h"

#include <SDL2/SDL_image.h>
#include <ostream>

#include "Logger.h"

bool TextImage::_runTtfInitOnce = true;
std::map<std::string, TTF_Font*> TextImage::_fonts;

TextImage::TextImage(std::string text, SDL_Color color, SDL_Renderer* renderer):
   ImageInfo(renderer)
{
   if (_runTtfInitOnce)
   {
      // Initialize the library
      if (TTF_Init() == -1)
      {
         LOG_FATAL() << "Error enabling TTF support:" << TTF_GetError();
      }
      _runTtfInitOnce = false;
   }

   // Since no font is specified, a font is required to be loaded already, use the first one
   if (_fonts.empty())
   {
      LOG_FATAL() << "TextImage constructor for default font called, but no fonts are loaded";
      return;
   }

   TTF_Font* f = _fonts.begin()->second;

   // Create surface with the text, and then create a texture with it
   SDL_Surface* surface = TTF_RenderText_Solid(f, text.c_str(), color);
   ProcessSurface(surface);

   LOG_DEBUG() << "Loaded texture with text (" << text << ") and size is (" << _size << ")";
}

TextImage::TextImage(std::string text, SDL_Color color, std::string font, int pt, SDL_Renderer* renderer):
   ImageInfo(renderer)
{
   if (_runTtfInitOnce)
   {
      // Initialize the library
      if (TTF_Init() == -1)
      {
         LOG_FATAL() << "Error enabling TTF support:" << TTF_GetError();
      }
      _runTtfInitOnce = false;
   }


   TTF_Font* f = GetFont(font, pt);
   if (f == nullptr)
   {
      // Fatal error already logged by LoadFont
      return;
   }

   // Create surface with the text, and then create a texture with it
   SDL_Surface* surface = TTF_RenderText_Solid(f, text.c_str(), color);
   ProcessSurface(surface);

   LOG_DEBUG() << "Loaded texture with text (" << text << ") and size is (" << _size << ")";
}

void TextImage::ProcessSurface(SDL_Surface* s)
{
   if (s == nullptr)
   {
      LOG_FATAL() << "Process surface passed null surface";
      return;
   }

   _size[0] = s->w;
   _size[1] = s->h;

   _src.w = s->w;
   _src.h = s->h;
   _src.x = 0;
   _src.y = 0;

   _texture = SDL_CreateTextureFromSurface(_renderer, s);

   if (_texture == nullptr)
   {
      LOG_FATAL() << "Error converting surface to texture in ProcessSurface:" << SDL_GetError();
      return;
   }

   SDL_FreeSurface(s);
}


bool TextImage::LoadFont(std::string font, int pt)
{
   if (_runTtfInitOnce)
   {
      // Initialize the library
      if (TTF_Init() == -1)
      {
         LOG_FATAL() << "Error enabling TTF support:" << TTF_GetError();
      }
      _runTtfInitOnce = false;
   }

   std::ostringstream oss;
   oss << font << "!" << pt;

   std::map<std::string, TTF_Font*>::iterator it = _fonts.find(oss.str());
   if (it == _fonts.end())
   {
      // We didn't have that font cached
      TTF_Font* f = TTF_OpenFont(font.c_str(), pt);
      if (f == nullptr)
      {
         LOG_WARNING() << "Error loading font (" << font << "," << pt << "):" << TTF_GetError();
         return false;
      }

      LOG_DEBUG() << "Font (" << font << "," << pt << ") added to font cache";
      _fonts[oss.str()] = f;
      return true;
   }
   else
   {
      LOG_DEBUG() << "Font (" << font << "," << pt << ") found in font cache";
      return true;
   }
}

void TextImage::UnloadAllFonts()
{
   if (_runTtfInitOnce)
   {
      // Nothing was ever loadedulils
      return;
   }

   std::map<std::string, TTF_Font*>::iterator it;
   for(it = _fonts.begin(); it != _fonts.end(); it++)
   {
      LOG_DEBUG() << "Unloading font" << it->first;
      TTF_CloseFont(it->second);
   }

   _fonts.clear();
}

TTF_Font* TextImage::GetFont(std::string font, int pt)
{
   std::ostringstream oss;
   oss << font << "!" << pt;

   std::map<std::string, TTF_Font*>::iterator it = _fonts.find(oss.str());
   if (it == _fonts.end())
   {
      // We didn't have that font cached
      TTF_Font* f = TTF_OpenFont(font.c_str(), pt);
      if (f == nullptr)
      {
         LOG_FATAL() << "Error loading font (" << font << "," << pt << "):" << TTF_GetError();
         return nullptr;
      }

      LOG_DEBUG() << "Font (" << font << "," << pt << ") added to font cache";
      _fonts[oss.str()] = f;
      return f;
   }
   else
   {
      LOG_DEBUG() << "Font (" << font << "," << pt << ") found in font cache";
      return it->second;
   }
}
