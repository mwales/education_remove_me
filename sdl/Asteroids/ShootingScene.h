#pragma once

#include <map>

#include "Scene.h"
#include "GraphicEntity.h"
#include "Command.h"
#include "Spaceship.h"

#include "CollisionManager.h"

class SpaceRock;
class FrictionlessGraphic;

class ShootingScene : public Scene
{
public:
   ShootingScene(Graphics* g, Mixer* m);

   ~ShootingScene();

   Scene* GetNextState(bool* deleteMe);

   void PauseGame();

   virtual void Update() override;

   virtual void ManageEntityLifetimes() override;

   void ToggleDebug();

protected:

   bool ProcessEvent(SDL_Event const & ev);

   void SpawnRock();

   GraphicEntity theBackground;

   std::vector<SpaceRock*> theBigRocks;

   // When counter hits zero, consider spawning a rock
   int theRockSpawnCounter;

   Spaceship theShip;

   std::map<int, Command*> theKeyboardDownMappedCommands;
   std::map<int, Command*> theKeyboardUpMappedCommands;


   bool thePauseState;
   Scene* theNextState;

   // Everything collides with rocks, so put them in their own list
   //  list A = Rocks
   //  list B = Bullets and Ship
   CollisionManager theCollisionMgr;

   // Variable that can be toggled to enable special debug functions
   bool theDebugMode;
};



class PauseCommand: public Command
{
public:
   PauseCommand(ShootingScene* scene);

   virtual bool Execute();

protected:

   ShootingScene* theScene;
};

class ToggleDebugCommand: public Command
{
public:
   ToggleDebugCommand(ShootingScene* scene);

   virtual bool Execute();

protected:

   ShootingScene* theScene;
};
