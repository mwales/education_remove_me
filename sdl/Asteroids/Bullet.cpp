#include "Bullet.h"
#include "Logger.h"

Bullet::Bullet(XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   _deletionList(nullptr)
{
   _image = new ImageInfo("assets/shot2.png", r);
   SetImageInfo(_image);
}

Bullet::~Bullet()
{
   LOG_DEBUG() << "~Bullet: this = " << _image;
   delete _image;
   _image = nullptr;
}

void Bullet::Update()
{
   // Not using parent implementation of physics, bullets don't slow down or rotate
   _position += _velocity * _updateRateScalar;

   // Wrap position around
   _position = GameMath::PositionModulus(_position, _mapBounds);

   _framesToLive--;
   if (_framesToLive < 0)
   {
      // Bullet lifetime expired
      _deletionList->push_back(this);
   }
   else
   {
      LOG_DEBUG() << "Frames left to live" << _framesToLive;
   }
}

void Bullet::SetLifetime(float secs, std::vector<GameEntity*>* deletionList)
{
   if (_updateRate == 0)
   {
      LOG_FATAL() << "Can't set bullet lifetime before setting update rate";
      return;
   }

   _framesToLive = _updateRate * secs;
   _deletionList = deletionList;
}
