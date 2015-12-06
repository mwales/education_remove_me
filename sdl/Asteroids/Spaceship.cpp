#include <SDL2/SDL.h>

#include "Spaceship.h"
#include "SpaceshipCommands.h"
#include "Logger.h"
#include "GameMath.h"

static const float THRUST_ACCELERATION = 800.0;

Spaceship::Spaceship(XYPair mapBounds):
   GraphicEntity(mapBounds),
   _turningDirection(0),
   _thrustOn(false)
{
}

void Spaceship::Fire()
{
   LOG_DEBUG() << "Fire!";
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
   LOG_DEBUG() << "Spaceship update";

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

   // Call parent update
   GraphicEntity::Update();
}

void Spaceship::SetRotationalAcceleration(int rotAcc)
{
   LOG_DEBUG() << "Rotational acc=" << rotAcc * 9.0;
   _rotAcceleration = rotAcc * 9.0;
}
