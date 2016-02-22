#include "FrictionlessGraphic.h"
#include "Logger.h"

FrictionlessGraphic::FrictionlessGraphic(std::string filename, XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   _framesToLive { 0 },
   _deletionList { nullptr }
{
   _image = new ImageInfo(filename.c_str(), r);
   SetImageInfo(_image);
}

FrictionlessGraphic::~FrictionlessGraphic()
{
   LOG_DEBUG() << "~FrictionlessGraphic: this = " << _image;
   delete _image;
   _image = nullptr;
}

void FrictionlessGraphic::Update()
{
   // Not using parent implementation of physics, Frictionless Graphics don't slow down or rotate
   _position += _velocity * _updateRateScalar;

   // Wrap position around
   _position = GameMath::PositionModulus(_position, _mapBounds);

   // Update object lifetime if it has a fixed lifetime
   if (_framesToLive > 0)
   {
      _framesToLive--;
      if (_framesToLive == 0)
      {
         // FrictionlessGraphic lifetime expired
         _deletionList->push_back(this);
      }
   }

}

void FrictionlessGraphic::SetLifetime(float secs, std::vector<GameEntity*>* deletionList)
{
   if (_updateRate == 0)
   {
      LOG_FATAL() << "Can't set FrictionlessGraphic lifetime before setting update rate";
      return;
   }

   _framesToLive = _updateRate * secs;
   _deletionList = deletionList;
}
