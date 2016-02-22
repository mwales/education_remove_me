#pragma once

#include "GraphicEntity.h"
#include "ICollidable.h"
#include "GameMath.h"

class ImageInfo;

class Bullet : public GraphicEntity
{
public:
   Bullet(XYPair mapBounds, SDL_Renderer* r);

   virtual ~Bullet();

   virtual void Update() override;

   /**
    * Sets how long the bullet will travel before disappearing
    * @param secs
    * @param deletionList
    */
   void SetLifetime(float secs, std::vector<GameEntity*>* deletionList);

protected:
   int _framesToLive;

   std::vector<GameEntity*>* _deletionList;
};
