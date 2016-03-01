#pragma once

#include <map>
#include "GraphicEntity.h"
#include "Command.h"
#include "GameMath.h"
#include "Joystick.h"


class Spaceship : public GraphicEntity, public JoystickRegistrationCallbacks
{
public:
   enum class BulletFireMode
   {
      RAPID_SHOT,
      SPREAD_SHOT,
      HEAVY_SHOT
   };

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

   virtual void Update() override;

   virtual void SetUpdateRate(int updateHz) override;

   std::vector<GraphicEntity*> GetNewBullets();

   // Joystick registration and callbacks

   virtual std::map<int, Command*> GetButtonDownHandlers();
   virtual std::map<int, Command*> GetButtonUpHandlers();
   virtual std::map<int, Command*> GetAxesHandlers();

   std::vector<SDL_Rect> GetCollisionBoxes() const override;

   virtual void Draw() override;

   void DisplayCollisionArea(bool display);

protected:

   int _turningDirection;\

   bool _thrustOn { false };

   bool _fireBullet { false };

   int _ticksSinceLastBullet { 0 };

   BulletFireMode _fireMode { BulletFireMode::RAPID_SHOT };

   std::map<BulletFireMode, int> _fireDelayMap;

   // Need this so we can pass it on to bullets so they can end themselves
   std::vector<GameEntity*>* _additionList;
   std::vector<GameEntity*>* _deletionList;

   std::vector<GraphicEntity*> _newBullets;

   bool _displayCollisionArea;
};

