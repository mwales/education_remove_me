#include "FrictionlessGraphic.h"
#include "Logger.h"
#include "ShootingSceneConstants.h"

FrictionlessGraphic::FrictionlessGraphic(std::string filename, XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   theFramesToLive { 0 },
   theLifetimeManager { nullptr }
{
   theImage = new ImageInfo(filename.c_str(), r);
   SetImageInfo(theImage);
}

FrictionlessGraphic::~FrictionlessGraphic()
{
   LOG_DEBUG() << "~FrictionlessGraphic: this = " << theImage;
   delete theImage;
   theImage = nullptr;
}

void FrictionlessGraphic::Update()
{
   // Not using parent implementation of physics, Frictionless Graphics don't slow down or rotate
   thePosition += theVelocity * theUpdateRateScalar;

   // Wrap position around
   thePosition = GameMath::PositionModulus(thePosition, theMapBounds);

   // Update object lifetime if it has a fixed lifetime
   if (theFramesToLive > 0)
   {
      theFramesToLive--;
      if (theFramesToLive == 0)
      {
         // FrictionlessGraphic lifetime expired
         if (theLifetimeManager == nullptr)
         {
            LOG_FATAL() << "Bullet lifetime expired, but no lifetime manager set";
            return;
         }

         LOG_DEBUG() << "End of life reached" << (long) this;
         theLifetimeManager->DeleteEntity(this, theEndOfLifeCode);
      }
   }

}

void FrictionlessGraphic::SetLifetime(float secs, ILifetimeManager* lm, int endOfLifeCode)
{
   if (theUpdateRateHz == 0)
   {
      LOG_FATAL() << "Can't set FrictionlessGraphic lifetime before setting update rate";
      return;
   }

   theFramesToLive = theUpdateRateHz * secs;
   theLifetimeManager = lm;
   theEndOfLifeCode = endOfLifeCode;
}
