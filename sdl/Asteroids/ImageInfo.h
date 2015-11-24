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

   ~ImageInfo();

   void SetAngle(float angleDeg);

   void Draw(XYPair dst);

   void Draw(XYPair dst, XYPair drawSize);

   void DrawFullScreen();

   XYPair GetSize();

   /// Possibly useful if using a single image from a tiled image set
   void SetSize(XYPair size);

protected:

   // Flag for run library initialization
   static bool _runOnce;

   SDL_Texture* _texture;

   SDL_Renderer* _renderer;

   float _angle;

   XYPair _size;
};
