#pragma once

#include "GraphicEntity.h"
#include "ICollidable.h"
#include "GameMath.h"
#include "ILifetimeManager.h"

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
    * @param lm
    * @param code The code to pass to lifetime manager on end of life
    */
   void SetLifetime(float secs, ILifetimeManager* lm, int code);

protected:
   int theFramesToLive;

   ILifetimeManager* theLifetimeManager;

   int theEndOfLifeCode;
};
