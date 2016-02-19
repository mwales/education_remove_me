#pragma once

#include <map>
#include "GraphicEntity.h"
#include "Command.h"
#include "GameMath.h"
#include "Joystick.h"


class Spaceship : public GraphicEntity, public JoystickRegistrationCallbacks
{
public:
   Spaceship(XYPair mapBounds);

   void Fire();

   void SetThrustState(bool state);

   void SetTurningDirection(int direction);

   void SetRotationalAcceleration(int rotAcc);

   std::map<int, Command*> GetKeyboardDownCallbacks();

   std::map<int, Command*> GetKeyboardUpCallbacks();

   virtual void Update();

   // Joystick registration and callbacks

   virtual std::map<int, Command*> GetButtonDownHandlers();
   virtual std::map<int, Command*> GetButtonUpHandlers();
   virtual std::map<int, Command*> GetAxesHandlers();

   std::vector<SDL_Rect> GetCollisionBoxes() const override;

protected:

   int _turningDirection;\

   bool _thrustOn;


};

