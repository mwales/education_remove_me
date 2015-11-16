#include "ImageInfo.h"
#include "Logger.h"

bool ImageInfo::_runOnce = true;

ImageInfo::ImageInfo(char const * filename, SDL_Renderer* renderer):
   _renderer(renderer)
 , _angle(0.0)
 , _size(2)
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

   _texture = SDL_CreateTextureFromSurface(_renderer, s);

   if (_texture == NULL)
   {
      LOG_FATAL() << "Error converting surface to texture for image named" << filename << ", error=" << SDL_GetError();
      return;
   }

   SDL_FreeSurface(s);

   LOG_DEBUG() << "Loaded image:" << filename;
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

void ImageInfo::AddAngle(float angleDeg)
{
   _angle += angleDeg;
}

void ImageInfo::Draw(XYPair dst)
{
   if (dst.size() != 2)
   {
      LOG_FATAL() << "Invalid XY pair, size =" << dst.size();
      return;
   }

   SDL_Rect pos;
   pos.x = dst[0];
   pos.y = dst[1];
   pos.w = _size[0];
   pos.h = _size[1];

   if (0 != SDL_RenderCopyEx(_renderer, _texture, NULL, &pos, _angle, NULL, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::Draw(XYPair dst, XYPair drawSize)
{
   if (dst.size() != 2)
   {
      LOG_FATAL() << "Invalid XY pair, dest size =" << dst.size();
      return;
   }

   if (drawSize.size() != 2)
   {
      LOG_FATAL() << "Invalid XY pair, drawSize size =" << drawSize.size();
      return;
   }

   SDL_Rect pos;
   pos.x = dst[0];
   pos.y = dst[1];
   pos.w = drawSize[0];
   pos.h = drawSize[1];

   if (0 != SDL_RenderCopyEx(_renderer, _texture, NULL, &pos, _angle, NULL, SDL_FLIP_NONE))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

void ImageInfo::DrawFullScreen()
{
   if (0 != SDL_RenderCopy(_renderer, _texture, NULL, NULL))
   {
      LOG_WARNING() << "Render call failed:" << SDL_GetError();
   }
}

XYPair ImageInfo::GetSize()
{
   return _size;
}
