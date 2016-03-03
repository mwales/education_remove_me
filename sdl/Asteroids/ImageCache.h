#pragma once

#include <map>
#include <string>
#include <vector>

#include "GameMath.h"

class SDL_Texture;
class SDL_Surface;
class SDL_Renderer;

#define NUM_CACHED_UNUSED_IMAGES_MAX 10

struct ImageCacheData
{
   std::string theFilename;

   XYPair theSize;

   int theUserCount;
};

/**
 * Loads and manages caches for textures for the requested images
 */
class ImageCache
{
public:

   /**
    * Loads the requested image from file into a texture, and the texture will be added to texture cache so
    * consecutive requests don't have to load from disk
    * @param filename Filename of image
    * @param renderer Graphics renderer that will render texture
    */
   ImageCache(char const * filename, SDL_Renderer* renderer);

   /// When the ImageCache object is deleted, the texture loaded with this instance will have user count decremented
   virtual ~ImageCache();

   SDL_Texture* GetTexture() const;

   XYPair GetSize() const;

   /// Clears the image cache (useful for running the application with Valgrind so it doesn't leak memory)
   static void ClearCache();

   static void ImageCacheDebugDump();

protected:

   void ConvertToTexture(std::string const & filename, SDL_Surface* surface, SDL_Renderer* renderer);

   SDL_Texture* LookupTexture(std::string const &) const;

   static void DeleteFromCache(SDL_Texture* t);

   SDL_Texture* theTexture;

   /// Stores the number of users of a cached texture
   static std::map<SDL_Texture*, ImageCacheData> theTextureData;

   /// Maps image filename to textures in cache
   static std::map<std::string, SDL_Texture*> theTextureLookup;

   /// List of currently unused textures, if this list count() > NUM_CACHED_UNUSED_IMAGES_MAX, then old images will be
   /// deleted
   static std::vector<SDL_Texture*> theUnusedTextures;

};
