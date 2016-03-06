#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameMath.h"

class ImageCache;

/**
 * Draws simple non-animated images on the screen
 */
class ImageInfo
{
public:

   ImageInfo(char const * filename, SDL_Renderer* renderer);

   virtual ~ImageInfo();

   virtual void SetAngle(float angleDeg);

   float GetAngle() { return theAngle; }

   virtual void Draw(XYPair dst);

   virtual void Draw(XYPair dst, float _angleDeg);

   virtual void Draw(XYPair dst, XYPair drawSize);

   virtual void Draw(XYPair dst, XYPair drawSize, float _angleDeg);

   virtual void DrawFullScreen();

   XYPair GetSize() { return theSize; }

   void SetOriginAtCenter(bool enable);

   virtual int GetNumberOfFrames() { return 0; }

   virtual void SetFrameNumber(int frame);

   SDL_Renderer* GetRenderer() { return theRenderer; }

protected:

   ImageInfo(SDL_Renderer* renderer);

   // Flag for run library initialization
   static bool theRunOnce;

   ImageCache* theCacheEntry;

   // Set to true if texture was loaded using ImageCache
   bool theUsingCachedTexture;

   SDL_Texture* theTexture;

   SDL_Renderer* theRenderer;

   float theAngle;

   XYPair theSize;

   /// True if image is tracked at center, or false at top-left corner
   bool theOriginAtCenter;

   /// Box of the source image
   SDL_Rect theSrc;
};
