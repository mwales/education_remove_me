#include "ImageInfo.h"
#include "ImageCache.h"

#include "Logger.h"

bool ImageInfo::_runOnce = true;

ImageInfo::ImageInfo(char const * filename, SDL_Renderer* renderer):
   _cacheEntry(new ImageCache(filename, renderer))
 , _renderer(renderer)
 , _angle(0.0)
 , _originAtCenter(true)
 {
   if (_runOnce)
   {
      // Initialize the library
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      _runOnce = false;
   }

   _texture = _cacheEntry->GetTexture();
   _usingCachedTexture = true;

   if (_texture == NULL)
   {
      LOG_FATAL() << "Error loading image named" << filename;
      return;
   }

   _size = _cacheEntry->GetSize();

   _src.w = _size[0];
   _src.h = _size[1];
   _src.x = 0;
   _src.y = 0;

   LOG_DEBUG() << "Loaded image:" << filename << "(" << _size << ")";
}

ImageInfo::ImageInfo(SDL_Renderer* renderer):
   _cacheEntry(NULL)
 , _renderer(renderer)
 , _angle(0.0)
 , _originAtCenter(true)
 {
   if (_runOnce)
   {
      // Initialize the library
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      _runOnce = false;
   }

   _texture = NULL;
   _usingCachedTexture = false;

}

ImageInfo::~ImageInfo()
{
   if (_cacheEntry)
   {
      delete _cacheEntry;
      _cacheEntry = NULL;
   }

   if (!_usingCachedTexture && ( _texture != NULL) )
   {
      LOG_DEBUG() << "Deleting non-cached texture";
      SDL_DestroyTexture(_texture);
      _texture = NULL;
   }
}

void ImageInfo::SetAngle(float angleDeg)
{
   _angle = angleDeg;
}

void ImageInfo::Draw(XYPair dst)
{
   Draw(dst, _angle);
}

void ImageInfo::Draw(XYPair dst, float angleDeg)
{
   _angle = angleDeg;

   SDL_Rect pos;

   if (_originAtCenter)
   {
      pos.x = dst[0] - _size[0]/2;
      pos.y = dst[1] - _size[1]/2;
   }
   else
   {
      pos.x = dst[0];
      pos.y = dst[1];
   }
   pos.w = _size[0];
   pos.h = _size[1];

   if (0 != SDL_RenderCopyEx(_renderer, _texture, &_src, &pos, _angle, NULL, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::Draw(XYPair dst, XYPair drawSize)
{
   Draw(dst, drawSize, _angle);
}

void ImageInfo::Draw(XYPair dst, XYPair drawSize, float angleDeg)
{
   _angle = angleDeg;

   SDL_Rect pos;
   if (_originAtCenter)
   {
      pos.x = dst[0] - _size[0]/2;
      pos.y = dst[1] - _size[1]/2;
   }
   else
   {
      pos.x = dst[0];
      pos.y = dst[1];
   }
   pos.w = drawSize[0];
   pos.h = drawSize[1];

   if (0 != SDL_RenderCopyEx(_renderer, _texture, &_src, &pos, _angle, NULL, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::DrawFullScreen()
{
   if (0 != SDL_RenderCopy(_renderer, _texture, &_src, NULL))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::SetOriginAtCenter(bool enable)
{
   _originAtCenter = enable;
}

void ImageInfo::SetFrameNumber(int frame)
{
   if (frame != 0)
   {
      LOG_WARNING() << "Setting non-zero frame number for a non-tiled image";
   }
}
