#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameMath.h"

/**
 * Draws simple non-animated images on the screen
 */
class ImageInfo
{
public:

   ImageInfo(char const * filename, SDL_Renderer* renderer);

   virtual ~ImageInfo();

   virtual void SetAngle(float angleDeg);

   virtual void Draw(XYPair dst);

   virtual void Draw(XYPair dst, float _angleDeg);

   virtual void Draw(XYPair dst, XYPair drawSize);

   virtual void Draw(XYPair dst, XYPair drawSize, float _angleDeg);

   virtual void DrawFullScreen();

   XYPair GetSize() { return _size; }

   void SetOriginAtCenter(bool enable);

   virtual int GetNumberOfFrames() { return 0; }

   virtual void SetFrameNumber(int frame);

protected:

   ImageInfo(SDL_Renderer* renderer);

   void ProcessSurface(SDL_Surface* surface);

   // Flag for run library initialization
   static bool _runOnce;

   SDL_Texture* _texture;

   SDL_Renderer* _renderer;

   float _angle;

   XYPair _size;

   /// True if image is tracked at center, or false at top-left corner
   bool _originAtCenter;

   /// Box of the source image
   SDL_Rect _src;
};
