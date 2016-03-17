#pragma once

#include <map>
#include "GraphicEntity.h"
#include "Command.h"
#include "GameMath.h"
#include "Joystick.h"

class ILifetimeManager;

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

   void SetLifetimeManager(ILifetimeManager* lm);

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

   int theTurningDirection;\

   bool theThrustOn { false };

   bool theFireBullet { false };

   int theTicksSinceLastBullet { 0 };

   BulletFireMode theFireMode { BulletFireMode::RAPID_SHOT };

   std::map<BulletFireMode, int> theFireDelayMap;

   std::vector<GraphicEntity*> theNewBullets;

   bool theDisplayCollisionArea;

   ILifetimeManager* theLifetimeMgr;
};

