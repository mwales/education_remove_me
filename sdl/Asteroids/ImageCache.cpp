#include "ImageCache.h"
#include "Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cassert>

// Static variables
std::map<SDL_Texture*, struct ImageCacheData> ImageCache::theTextureData;
std::map<std::string, SDL_Texture*> ImageCache::theTextureLookup;
std::vector<SDL_Texture*> ImageCache::theUnusedTextures;

ImageCache::ImageCache(char const * filename, SDL_Renderer* renderer)
{
   SDL_Surface* s = nullptr;

   theTexture = LookupTexture(filename);
   if (theTexture)
   {
      // Texture was already cached
      LOG_DEBUG() << "Found texture for " << filename << " in cache!";
      theTextureData[theTexture].theUserCount++;
      return;
   }

   s = IMG_Load(filename);
   if (s == nullptr)
   {
      LOG_FATAL() << "Error loading image named" << filename << ", error=" << IMG_GetError();
      return;
   }

   ConvertToTexture(filename, s, renderer);

   LOG_DEBUG() << "Loaded texture from disk:" << filename;
}


ImageCache::~ImageCache()
{
   if (theTexture)
   {
      // Drop the user count 1 - texture is always in the list
      std::map<SDL_Texture*, ImageCacheData>::iterator it;
      it = theTextureData.find(theTexture);

      // Texture should always be in the map
      assert(it != theTextureData.end());

      it->second.theUserCount--;

      if (it->second.theUserCount == 0)
      {
         LOG_DEBUG() << "Added image to unused image list";
         // ImageCacheDebugDump();

         // We were the last user, add to the list of unused textures
         theUnusedTextures.push_back(theTexture);

         if (theUnusedTextures.size() > NUM_CACHED_UNUSED_IMAGES_MAX)
         {
            LOG_DEBUG() << "Too many old unused images, removing one";

            // Delete the oldest unused texture on the unused textures list
            SDL_Texture* t = theUnusedTextures.back();
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
   return theTexture;
}

XYPair ImageCache::GetSize() const
{
   if (theTexture)
   {
      return theTextureData[theTexture].theSize;
   }
   else
   {
      LOG_FATAL() << "Failed trying to get the size of a ImageCache that never loaded an image";
      return XYPair();
   }
}

void ImageCache::ConvertToTexture(std::string const & filename, SDL_Surface* s, SDL_Renderer* renderer)
{
   assert(s != nullptr);
   assert(renderer != nullptr);

   ImageCacheData cacheEntry;

   cacheEntry.theSize[0] = s->w;
   cacheEntry.theSize[1] = s->h;

   theTexture = SDL_CreateTextureFromSurface(renderer, s);

   if (theTexture == nullptr)
   {
      LOG_FATAL() << "Error converting surface to texture in ProcessSurface:" << SDL_GetError();
      return;
   }

   SDL_FreeSurface(s);

   cacheEntry.theUserCount = 1;
   cacheEntry.theFilename = filename;
   theTextureData[theTexture] = cacheEntry;
   theTextureLookup[filename] = theTexture;

   LOG_DEBUG() << "Successfully converted surface to texture";
}

/// Clears the image cache (useful for running the application with Valgrind so it doesn't leak memory)
void ImageCache::ClearCache()
{
   while (!theTextureLookup.empty())
   {
      std::map<std::string, SDL_Texture*>::const_iterator it = theTextureLookup.begin();
      DeleteFromCache(it->second);
   }
}

SDL_Texture* ImageCache::LookupTexture(std::string const & imageFile) const
{
   std::map<std::string, SDL_Texture*>::const_iterator it;
   it = theTextureLookup.find(imageFile);

   if (it != theTextureLookup.end())
   {
      return it->second;
   }
   else
   {
      return nullptr;
   }
}

void ImageCache::DeleteFromCache(SDL_Texture* t)
{
   std::map<SDL_Texture*, ImageCacheData>::iterator cacheDataIt;
   cacheDataIt = theTextureData.find(t);

   if (cacheDataIt != theTextureData.end())
   {
      theTextureData.erase(cacheDataIt);
   }
   else
   {
      LOG_WARNING() << "Attempting to delete a texture from cache, but couldn't find texture cache data";
   }

   auto unusedIt = std::find(theUnusedTextures.begin(), theUnusedTextures.end(), t);
   if (unusedIt != theUnusedTextures.end())
   {
      theUnusedTextures.erase(unusedIt);
   }
   else
   {
      LOG_WARNING() << "Attempting to delete a texture from cache, but couldn't find texture in unused list";
   }

   std::map<std::string, SDL_Texture*>::iterator it;
   for(it = theTextureLookup.begin(); it != theTextureLookup.end(); it++)
   {
      if (it->second == t)
      {
         LOG_DEBUG() << "Deleting " << it->first << " from cache";
         SDL_DestroyTexture(t);
         theTextureLookup.erase(it);
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
   for(auto const & curTexture : theTextureLookup)
   {
      LOG_DEBUG() << "*   " << curTexture.first << " -> " << (unsigned long) curTexture.second;
   }

   LOG_DEBUG() << "* _textureData:";
   for(auto const & curData : theTextureData)
   {
      LOG_DEBUG() << "*   " << (unsigned long) curData.first << " -> ("
                  << curData.second.theFilename << ", "
                  << curData.second.theSize << ", "
                  << curData.second.theUserCount << ")";
   }

   LOG_DEBUG() << "* _unusedTextures:";
   for(auto const & curUnused : theUnusedTextures)
   {
      LOG_DEBUG() << "*   " << (unsigned long) curUnused;
   }
   LOG_DEBUG() << "****************************************";

}

