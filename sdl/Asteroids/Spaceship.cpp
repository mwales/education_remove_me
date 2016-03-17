#include <SDL2/SDL.h>

#include "Spaceship.h"
#include "SpaceshipCommands.h"
#include "Logger.h"
#include "GameMath.h"
#include "FrictionlessGraphic.h"
#include "ILifetimeManager.h"
#include "ShootingSceneConstants.h"

static const float THRUST_ACCELERATION = 800.0;

Spaceship::Spaceship(XYPair mapBounds):
   GraphicEntity(mapBounds),
   theTurningDirection(0),
   theDisplayCollisionArea(false),
   theLifetimeMgr(nullptr)
{
   // empty
}

void Spaceship::SetLifetimeManager(ILifetimeManager* lm)
{
   theLifetimeMgr = lm;
}

void Spaceship::Fire(bool fireState)
{
   LOG_DEBUG() << "Fire";
   theFireBullet = fireState;
}

void Spaceship::FireBullet()
{
   LOG_DEBUG() << "FireBullet";
   theTicksSinceLastBullet++;

   if (theTicksSinceLastBullet < theFireDelayMap[theFireMode])
   {
      // Gun is in the cooldown period still
      return;
   }
   else
   {
      // Reseting cooldown period
      theTicksSinceLastBullet = 0;
   }

   if (theLifetimeMgr == nullptr)
   {
      LOG_WARNING() << "Can't fire because we never got spaceship doesn't have lifetime mgr";
      return;
   }

   FrictionlessGraphic* b = new FrictionlessGraphic("assets/shot2.png", theMapBounds, GetImageInfo()->GetRenderer());
   b->SetUpdateRate(theUpdateRateHz);
   b->SetLifetime(5.0, theLifetimeMgr, ShootingSceneLifetimeCodes::REMOVE_CODE_BULLET);
   b->SetAngle(theAngle);

   XYPair directionUnitVec = GameMath::GetUnitVector(theAngle);
   XYPair bulletVel = directionUnitVec;
   bulletVel *= 100.0;
   bulletVel += theVelocity;
   b->SetVelocity(bulletVel);

   static float bulletOffset = GetImageInfo()->GetSize()[0] / 2.0;
   XYPair bulletPos = thePosition + directionUnitVec * bulletOffset;
   b->SetPosition(bulletPos);

   theLifetimeMgr->AddEntity(b, ShootingSceneLifetimeCodes::ADD_CODE_BULLET);
}

void Spaceship::SetThrustState(bool state)
{
   LOG_DEBUG() << "Setting thrust state to" << state;
   theThrustOn = state;

   SetFrameNumber(theThrustOn ? 1 : 0);
}

void Spaceship::SetTurningDirection(int direction)
{
   theTurningDirection += direction;
   LOG_DEBUG() << "Setting turning direction to" << theTurningDirection;

   if (theTurningDirection < 0)
   {
      theRotAcceleration = -700.0;
   }
   else if (theTurningDirection > 0)
   {
      theRotAcceleration = +700.0;
   }
   else
   {
      theRotAcceleration = 0;
   }

}

std::map<int, Command*> Spaceship::GetKeyboardDownCallbacks()
{
   std::map<int, Command*> callbacks;
   callbacks[SDL_SCANCODE_LCTRL] = new FireCommand(this, true);

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
   callbacks[SDL_SCANCODE_LCTRL] = new FireCommand(this, false);

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
   callbacks[0] = new FireCommand(this, true);
   callbacks[5] = new ThrustCommand(this, true);
   return callbacks;
}

std::map<int, Command*> Spaceship::GetButtonUpHandlers()
{
   std::map<int, Command*> callbacks;
   callbacks[0] = new FireCommand(this, false);
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
   if (theThrustOn)
   {
      XYPair accelDir = GameMath::GetUnitVector(theAngle);
      theAcceleration = (accelDir * THRUST_ACCELERATION);
   }
   else
   {
      theAcceleration = XYPair(0.0, 0.0);
   }

   if (theFireBullet)
   {
      FireBullet();
   }

   // Call parent update
   GraphicEntity::Update();
}

void Spaceship::SetUpdateRate(int updateHz)
{
   theFireDelayMap.clear();
   theFireDelayMap[BulletFireMode::RAPID_SHOT] = updateHz / 15;
   theFireDelayMap[BulletFireMode::SPREAD_SHOT] = updateHz / 4;
   theFireDelayMap[BulletFireMode::HEAVY_SHOT] = updateHz;

   GraphicEntity::SetUpdateRate(updateHz);
}

void Spaceship::SetRotationalAcceleration(int rotAcc)
{
   LOG_DEBUG() << "Rotational acc=" << rotAcc * 9.0;
   theRotAcceleration = rotAcc * 9.0;
}

std::vector<SDL_Rect> Spaceship::GetCollisionBoxes() const
{
   // Basic collision box is the size of the image
   XYPair rectSize = theImage->GetSize() * 0.5;
   XYPair topLeftPoint = thePosition - rectSize * 0.5;

   SDL_Rect basicRectangle { (int) topLeftPoint[0], (int) topLeftPoint[1], (int) rectSize[0], (int) rectSize[1] };
   std::vector<SDL_Rect> retVal;
   retVal.push_back(basicRectangle);
   return retVal;
}

void Spaceship::Draw()
{
   GraphicEntity::Draw();

   if (theDisplayCollisionArea)
   {
      SDL_Renderer* r = theImage->GetRenderer();

      for(auto singleBox : GetCollisionBoxes())
      {

         SDL_RenderDrawRect(r, &singleBox);
      }
   }
}

void Spaceship::DisplayCollisionArea(bool display)
{
   theDisplayCollisionArea = display;
}
