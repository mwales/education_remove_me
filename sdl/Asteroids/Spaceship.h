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

   // Event that tells us we should start/stop firing on the next update call
   void Fire(bool fireState);

   // Fires a bullet (call this during the update call)
   void FireBullet();

   void SetThrustState(bool state);

   void SetTurningDirection(int direction);

   void SetRotationalAcceleration(int rotAcc);

   void SetAddDeleteLists(std::vector<GameEntity*>* addList,
                          std::vector<GameEntity*>* delList);

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

   bool _fireBullet;

   // Need this so we can pass it on to bullets so they can end themselves
   std::vector<GameEntity*>* _additionList;
   std::vector<GameEntity*>* _deletionList;


};

