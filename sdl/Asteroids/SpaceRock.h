#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "MovingEntity.h"
#include "GameMath.h"

class SpaceRock : public MovingEntity
{
public:
   SpaceRock(XYPair mapBounds, SDL_Renderer* r);

   void SetRandomLocation(XYPair shipPos);

protected:

   static bool _loadImagesOnce;

   static std::vector<ImageInfo*> _rockImages;

   static std::vector<ImageInfo*> _explosionImages;
};
