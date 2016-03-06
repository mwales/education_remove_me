#include "ImageInfo.h"
#include "ImageCache.h"

#include "Logger.h"

bool ImageInfo::theRunOnce = true;

ImageInfo::ImageInfo(char const * filename, SDL_Renderer* renderer):
   theCacheEntry(new ImageCache(filename, renderer))
 , theRenderer(renderer)
 , theAngle(0.0)
 , theOriginAtCenter(true)
 {
   if (theRunOnce)
   {
      // Initialize the library
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      theRunOnce = false;
   }

   theTexture = theCacheEntry->GetTexture();
   theUsingCachedTexture = true;

   if (theTexture == nullptr)
   {
      LOG_FATAL() << "Error loading image named" << filename;
      return;
   }

   theSize = theCacheEntry->GetSize();

   theSrc.w = theSize[0];
   theSrc.h = theSize[1];
   theSrc.x = 0;
   theSrc.y = 0;

   LOG_DEBUG() << "Loaded image:" << filename << "(" << theSize << ")";
}

ImageInfo::ImageInfo(SDL_Renderer* renderer):
   theCacheEntry(nullptr)
 , theRenderer(renderer)
 , theAngle(0.0)
 , theOriginAtCenter(true)
 {
   if (theRunOnce)
   {
      // Initialize the library
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      theRunOnce = false;
   }

   theTexture = nullptr;
   theUsingCachedTexture = false;

}

ImageInfo::~ImageInfo()
{
   if (theCacheEntry)
   {
      delete theCacheEntry;
      theCacheEntry = nullptr;
   }

   if (!theUsingCachedTexture && ( theTexture != nullptr) )
   {
      LOG_DEBUG() << "Deleting non-cached texture";
      SDL_DestroyTexture(theTexture);
      theTexture = nullptr;
   }
}

void ImageInfo::SetAngle(float angleDeg)
{
   theAngle = angleDeg;
}

void ImageInfo::Draw(XYPair dst)
{
   Draw(dst, theAngle);
}

void ImageInfo::Draw(XYPair dst, float angleDeg)
{
   theAngle = angleDeg;

   SDL_Rect pos;

   if (theOriginAtCenter)
   {
      pos.x = dst[0] - theSize[0]/2;
      pos.y = dst[1] - theSize[1]/2;
   }
   else
   {
      pos.x = dst[0];
      pos.y = dst[1];
   }
   pos.w = theSize[0];
   pos.h = theSize[1];

   if (0 != SDL_RenderCopyEx(theRenderer, theTexture, &theSrc, &pos, theAngle, nullptr, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::Draw(XYPair dst, XYPair drawSize)
{
   Draw(dst, drawSize, theAngle);
}

void ImageInfo::Draw(XYPair dst, XYPair drawSize, float angleDeg)
{
   theAngle = angleDeg;

   SDL_Rect pos;
   if (theOriginAtCenter)
   {
      pos.x = dst[0] - theSize[0]/2;
      pos.y = dst[1] - theSize[1]/2;
   }
   else
   {
      pos.x = dst[0];
      pos.y = dst[1];
   }
   pos.w = drawSize[0];
   pos.h = drawSize[1];

   if (0 != SDL_RenderCopyEx(theRenderer, theTexture, &theSrc, &pos, theAngle, nullptr, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::DrawFullScreen()
{
   if (0 != SDL_RenderCopy(theRenderer, theTexture, &theSrc, nullptr))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::SetOriginAtCenter(bool enable)
{
   theOriginAtCenter = enable;
}

void ImageInfo::SetFrameNumber(int frame)
{
   if (frame != 0)
   {
      LOG_WARNING() << "Setting non-zero frame number for a non-tiled image";
   }
}
