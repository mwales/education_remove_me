#include <SDL2/SDL.h>

#include "Spaceship.h"
#include "SpaceshipCommands.h"
#include "Logger.h"
#include "GameMath.h"
#include "FrictionlessGraphic.h"

static const float THRUST_ACCELERATION = 800.0;

Spaceship::Spaceship(XYPair mapBounds):
   GraphicEntity(mapBounds),
   _turningDirection(0),
   _thrustOn(false),
   _fireBullet(false),
   _additionList(nullptr),
   _deletionList(nullptr)
{
}


void Spaceship::SetAddDeleteLists(std::vector<GameEntity*>* addList,
                                  std::vector<GameEntity*>* delList)
{
   _additionList = addList;
   _deletionList = delList;
}

void Spaceship::Fire()
{
   LOG_DEBUG() << "Fire";
   _fireBullet = true;
}

void Spaceship::FireBullet()
{
   LOG_DEBUG() << "FireBullet";

   if ( (_deletionList == nullptr) || (_additionList == nullptr) )
   {
      LOG_WARNING() << "Can't fire because we never got spaceship doesn't have add / del list";
      return;
   }

   FrictionlessGraphic* b = new FrictionlessGraphic("assets/shot2.png", _mapBounds, GetImageInfo()->GetRenderer());
   b->SetUpdateRate(_updateRate);
   b->SetLifetime(5.0, _deletionList);
   b->SetAngle(_angle);

   XYPair directionUnitVec = GameMath::GetUnitVector(_angle);
   XYPair bulletVel = directionUnitVec;
   bulletVel *= 100.0;
   bulletVel += _velocity;
   b->SetVelocity(bulletVel);

   static float bulletOffset = GetImageInfo()->GetSize()[0] / 2.0;
   XYPair bulletPos = _position + directionUnitVec * bulletOffset;
   b->SetPosition(bulletPos);


   _additionList->push_back(b);
   _fireBullet = false;
}


void Spaceship::SetThrustState(bool state)
{
   LOG_DEBUG() << "Setting thrust state to" << state;
   _thrustOn = state;

   SetFrameNumber(_thrustOn ? 1 : 0);
}

void Spaceship::SetTurningDirection(int direction)
{
   _turningDirection += direction;
   LOG_DEBUG() << "Setting turning direction to" << _turningDirection;

   if (_turningDirection < 0)
   {
      _rotAcceleration = -700.0;
   }
   else if (_turningDirection > 0)
   {
      _rotAcceleration = +700.0;
   }
   else
   {
      _rotAcceleration = 0;
   }

}

std::map<int, Command*> Spaceship::GetKeyboardDownCallbacks()
{
   std::map<int, Command*> callbacks;
   callbacks[SDL_SCANCODE_LCTRL] = new FireCommand(this);

   callbacks[SDL_SCANCODE_UP] = new ThrustCommand(this, true);
   callbacks[SDL_SCANCODE_W] = new ThrustCommand(this, true);

   callbacks[SDL_SCANCODE_LEFT] = new TurnCommand(this, -1);
   callbacks[SDL_SCANCODE_A] = new TurnCommand(this, -1);

   callbacks[SDL_SCANCODE_RIGHT] = new TurnCommand(this, 1);
   callbacks[SDL_SCANCODE_D] = new TurnCommand(this, 1);

   return callbacks;
}

std::map<int, Command*> Spaceship::GetKeyboardUpCallbacks()
{
   std::map<int, Command*> callbacks;
   callbacks[SDL_SCANCODE_UP] = new ThrustCommand(this, false);
   callbacks[SDL_SCANCODE_W] = new ThrustCommand(this, false);

   callbacks[SDL_SCANCODE_LEFT] = new TurnCommand(this, 1);
   callbacks[SDL_SCANCODE_A] = new TurnCommand(this, 1);

   callbacks[SDL_SCANCODE_RIGHT] = new TurnCommand(this, -1);
   callbacks[SDL_SCANCODE_D] = new TurnCommand(this, -1);

   return callbacks;
}

std::map<int, Command*> Spaceship::GetButtonDownHandlers()
{
   std::map<int, Command*> callbacks;
   callbacks[0] = new FireCommand(this);
   callbacks[5] = new ThrustCommand(this, true);
   return callbacks;
}

std::map<int, Command*> Spaceship::GetButtonUpHandlers()
{
   std::map<int, Command*> callbacks;
   callbacks[5] = new ThrustCommand(this, false);
   return callbacks;
}

std::map<int, Command*> Spaceship::GetAxesHandlers()
{
   std::map<int, Command*> callbacks;
   callbacks[0] = new TurnJSCommand(this);
   return callbacks;
}

void Spaceship::Update()
{
   LOG_DEBUG() << "Updating spaceship (" << (unsigned long) this << ")";

   // Reset thrust vector
   if (_thrustOn)
   {
      XYPair accelDir = GameMath::GetUnitVector(_angle);
      _acceleration = (accelDir * THRUST_ACCELERATION);
   }
   else
   {
      _acceleration = XYPair(0.0, 0.0);
   }

   if (_fireBullet)
   {
      FireBullet();
   }

   // Call parent update
   GraphicEntity::Update();
}

void Spaceship::SetRotationalAcceleration(int rotAcc)
{
   LOG_DEBUG() << "Rotational acc=" << rotAcc * 9.0;
   _rotAcceleration = rotAcc * 9.0;
}

std::vector<SDL_Rect> Spaceship::GetCollisionBoxes() const
{
   // Basic collision box is the size of the image
   XYPair rectSize = _image->GetSize() * 0.5;
   XYPair topLeftPoint = _position - rectSize * 0.5;

   SDL_Rect basicRectangle { (int) topLeftPoint[0], (int) topLeftPoint[1], (int) rectSize[0], (int) rectSize[1] };
   std::vector<SDL_Rect> retVal;
   retVal.push_back(basicRectangle);
   return retVal;
}
