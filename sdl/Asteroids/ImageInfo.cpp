#include "ImageInfo.h"
#include "Logger.h"

bool ImageInfo::_runOnce = true;

ImageInfo::ImageInfo(char const * filename, SDL_Renderer* renderer):
   _renderer(renderer)
 , _angle(0.0)
 , _originAtCenter(true)
 {
   if (_runOnce)
   {
      // Initialize the library
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      _runOnce = false;
   }

   SDL_Surface* s = NULL;
   _texture = NULL;

   s = IMG_Load(filename);
   if (s == NULL)
   {
      LOG_FATAL() << "Error loading image named" << filename << ", error=" << IMG_GetError();
      return;
   }

   _size[0] = s->w;
   _size[1] = s->h;

   _src.w = s->w;
   _src.h = s->h;
   _src.x = 0;
   _src.y = 0;

   _texture = SDL_CreateTextureFromSurface(_renderer, s);

   if (_texture == NULL)
   {
      LOG_FATAL() << "Error converting surface to texture for image named" << filename << ", error=" << SDL_GetError();
      return;
   }

   SDL_FreeSurface(s);

   LOG_DEBUG() << "Loaded image:" << filename << "(" << _size << ")";
}

ImageInfo::~ImageInfo()
{
   if (_texture)
   {
      SDL_DestroyTexture(_texture);
   }
}

void ImageInfo::SetAngle(float angleDeg)
{
   _angle = angleDeg;
}

void ImageInfo::Draw(XYPair dst)
{
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
