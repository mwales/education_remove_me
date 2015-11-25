#pragma once

#include <map>
#include "MovingEntity.h"
#include "Command.h"
#include "GameMath.h"



class Spaceship : public MovingEntity
{
public:
   Spaceship(XYPair mapBounds);

   void Fire();

   void SetThrustState(bool state);

   void SetTurningDirection(int direction);

   std::map<int, Command*> GetKeyboardDownCallbacks();

   std::map<int, Command*> GetKeyboardUpCallbacks();

   virtual void Update();

protected:

   int _turningDirection;\

   bool _thrustOn;


};

