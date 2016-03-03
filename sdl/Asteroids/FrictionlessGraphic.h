#pragma once

#include "GraphicEntity.h"
#include "ICollidable.h"
#include "GameMath.h"

class ImageInfo;

class FrictionlessGraphic : public GraphicEntity
{
public:
   FrictionlessGraphic(std::string filename, XYPair mapBounds, SDL_Renderer* r);

   virtual ~FrictionlessGraphic();

   virtual void Update() override;

   /**
    * Sets how long the object will travel before disappearing
    *
    * @note Set the updateRate before setting lifetime
    *
    * @param secs
    * @param deletionList
    */
   void SetLifetime(float secs, std::vector<GameEntity*>* deletionList);

protected:
   int theFramesToLive;

   std::vector<GameEntity*>* theDeletionList;
};
