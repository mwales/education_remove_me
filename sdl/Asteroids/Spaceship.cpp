#include <SDL2/SDL.h>

#include "Spaceship.h"
#include "SpaceshipCommands.h"
#include "Logger.h"
#include "GameMath.h"

static const float THRUST_ACCELERATION = 50.0;

Spaceship::Spaceship(XYPair mapBounds):
   MovingEntity(mapBounds),
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


}

void Spaceship::SetTurningDirection(int direction)
{
   _turningDirection += direction;
   LOG_DEBUG() << "Setting turning direction to" << _turningDirection;

   if (_turningDirection < 0)
   {
      _rotAcceleration = -90.0;
   }
   else if (_turningDirection > 0)
   {
      _rotAcceleration = +90.0;
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
   MovingEntity::Update();
}
