#include "ImageCache.h"
#include "Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cassert>

// Static variables
std::map<SDL_Texture*, struct ImageCacheData> ImageCache::_textureData;
std::map<std::string, SDL_Texture*> ImageCache::_textureLookup;
std::vector<SDL_Texture*> ImageCache::_unusedTextures;

ImageCache::ImageCache(char const * filename, SDL_Renderer* renderer)
{
   SDL_Surface* s = NULL;

   _texture = LookupTexture(filename);
   if (_texture)
   {
      // Texture was already cached
      LOG_DEBUG() << "Found texture for " << filename << " in cache!";
      _textureData[_texture]._userCount++;
      return;
   }

   s = IMG_Load(filename);
   if (s == NULL)
   {
      LOG_FATAL() << "Error loading image named" << filename << ", error=" << IMG_GetError();
      return;
   }

   ConvertToTexture(filename, s, renderer);

   LOG_DEBUG() << "Loaded texture from disk:" << filename;
}


ImageCache::~ImageCache()
{
   if (_texture)
   {
      // Drop the user count 1 - texture is always in the list
      std::map<SDL_Texture*, ImageCacheData>::iterator it;
      it = _textureData.find(_texture);

      // Texture should always be in the map
      assert(it != _textureData.end());

      it->second._userCount--;

      if (it->second._userCount == 0)
      {
         LOG_DEBUG() << "Added image to unused image list";
         ImageCacheDebugDump();

         // We were the last user, add to the list of unused textures
         _unusedTextures.push_back(_texture);

         if (_unusedTextures.size() > NUM_CACHED_UNUSED_IMAGES_MAX)
         {
            LOG_DEBUG() << "Too many old unused images, removing one";

            // Delete the oldest unused texture on the unused textures list
            SDL_Texture* t = _unusedTextures.back();
            DeleteFromCache(t);
         }
      }
   }
   else
   {
      LOG_WARNING() << "Deleted a ImageCache object that never fully initialized";
   }
}

SDL_Texture* ImageCache::GetTexture() const
{
   return _texture;
}

XYPair ImageCache::GetSize() const
{
   if (_texture)
   {
      return _textureData[_texture]._size;
   }
   else
   {
      LOG_FATAL() << "Failed trying to get the size of a ImageCache that never loaded an image";
   }
}

void ImageCache::ConvertToTexture(std::string const & filename, SDL_Surface* s, SDL_Renderer* renderer)
{
   assert(s != NULL);
   assert(renderer != NULL);

   ImageCacheData cacheEntry;

   cacheEntry._size[0] = s->w;
   cacheEntry._size[1] = s->h;

   _texture = SDL_CreateTextureFromSurface(renderer, s);

   if (_texture == NULL)
   {
      LOG_FATAL() << "Error converting surface to texture in ProcessSurface:" << SDL_GetError();
      return;
   }

   SDL_FreeSurface(s);

   cacheEntry._userCount = 1;
   cacheEntry._filename = filename;
   _textureData[_texture] = cacheEntry;
   _textureLookup[filename] = _texture;

   LOG_DEBUG() << "Successfully converted surface to texture";
}

/// Clears the image cache (useful for running the application with Valgrind so it doesn't leak memory)
void ImageCache::ClearCache()
{
   while (!_textureLookup.empty())
   {
      std::map<std::string, SDL_Texture*>::const_iterator it = _textureLookup.begin();
      DeleteFromCache(it->second);
   }
}

SDL_Texture* ImageCache::LookupTexture(std::string const & imageFile) const
{
   std::map<std::string, SDL_Texture*>::const_iterator it;
   it = _textureLookup.find(imageFile);

   ImageCacheDebugDump();

   if (it != _textureLookup.end())
   {
      LOG_DEBUG() << "Texture " << imageFile << " found in cache";
      return it->second;
   }
   else
   {
      LOG_DEBUG() << "Texture " << imageFile << " not in cache";
      return NULL;
   }
}

void ImageCache::DeleteFromCache(SDL_Texture* t)
{
   std::map<SDL_Texture*, ImageCacheData>::iterator cacheDataIt;
   cacheDataIt = _textureData.find(t);

   if (cacheDataIt != _textureData.end())
   {
      _textureData.erase(cacheDataIt);
   }
   else
   {
      LOG_WARNING() << "Attempting to delete a texture from cache, but couldn't find texture cache data";
   }

   std::vector<SDL_Texture*>::iterator unusedIt;
   unusedIt = std::find(_unusedTextures.begin(), _unusedTextures.end(), t);
   if (unusedIt != _unusedTextures.end())
   {
      _unusedTextures.erase(unusedIt);
   }
   else
   {
      LOG_WARNING() << "Attempting to delete a texture from cache, but couldn't find texture in unused list";
   }

   std::map<std::string, SDL_Texture*>::iterator it;
   for(it = _textureLookup.begin(); it != _textureLookup.end(); it++)
   {
      if (it->second == t)
      {
         LOG_DEBUG() << "Deleting " << it->first << " from cache";
         SDL_DestroyTexture(t);
         _textureLookup.erase(it);
         return;
      }
   }

   LOG_WARNING() << "Attempting to delete a texture from cache, but can't find texture in lookup data";
}

void ImageCache::ImageCacheDebugDump()
{
   LOG_DEBUG() << "****************************************";
   LOG_DEBUG() << "* ImageCacheDebugDump";
   LOG_DEBUG() << "*";

   LOG_DEBUG() << "* _textureLookup:";
   std::map<std::string, SDL_Texture*>::const_iterator lookupIt;
   for(lookupIt = _textureLookup.begin(); lookupIt != _textureLookup.end(); lookupIt++)
   {
      LOG_DEBUG() << "*   " << lookupIt->first << " -> " << (unsigned long) lookupIt->second;
   }

   LOG_DEBUG() << "* _textureData:";
   std::map<SDL_Texture*, ImageCacheData>::const_iterator dataIt;
   for(dataIt = _textureData.begin(); dataIt != _textureData.end(); dataIt++)
   {
      LOG_DEBUG() << "*   " << (unsigned long) dataIt->first << " -> ("
                  << dataIt->second._filename << ", "
                  << dataIt->second._size << ", "
                  << dataIt->second._userCount << ")";
   }

   LOG_DEBUG() << "* _unusedTextures:";
   std::vector<SDL_Texture*>::const_iterator unusedIt;
   for(unusedIt = _unusedTextures.begin(); unusedIt != _unusedTextures.end(); unusedIt++)
   {
      LOG_DEBUG() << "*   " << (unsigned long) (*unusedIt);
   }
}

