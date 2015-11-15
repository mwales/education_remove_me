#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameMath.h"

/**
 * Draws simple non-animated images on the screen
 */
class ImageInfo
{
   ImageInfo(char* filename, SDL_Renderer* renderer);

   ~ImageInfo();

   void SetAngle(float angleDeg);

   void AddAngle(float angleDeg);

   void Draw(XYPair dst);

   void Draw(XYPair dst, XYPair drawSize);

   XYPair GetSize();

protected:

   // Flag for run library initialization
   static bool _runOnce;

   SDL_Texture* _texture;

   SDL_Renderer* _renderer;

   float _angle;

   XYPair _size;
};
