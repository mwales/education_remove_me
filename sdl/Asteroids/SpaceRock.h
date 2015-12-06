#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "GraphicEntity.h"
#include "GameMath.h"

class AnimationDriver;
class TiledImage;

class SpaceRock : public GraphicEntity
{
public:
   SpaceRock(XYPair mapBounds, SDL_Renderer* r);

   ~SpaceRock();

   void SetRandomLocation(XYPair shipPos);

   void Explode();

   virtual void Update();

protected:

   static bool _loadImagesOnce;

   static std::vector<ImageInfo*> _rockImages;

   static std::vector<TiledImage*> _explosionImages;

   AnimationDriver* _animator;
};
