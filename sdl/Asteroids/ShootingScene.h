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

protected:

   bool ProcessEvent(SDL_Event const & ev);

   void SpawnRock();

   GraphicEntity _background;

   std::vector<SpaceRock*> _bigRocks;

   // When counter hits zero, consider spawning a rock
   int _rockSpawnCounter;

   Spaceship _ship;

   std::map<int, Command*> _keyboardDownMappedCommands;
   std::map<int, Command*> _keyboardUpMappedCommands;


   bool _pauseState;
   Scene* _nextState;

   // Everything collides with rocks, so put them in their own list
   //  list A = Rocks
   //  list B = Bullets and Ship
   CollisionManager _collisionMgr;
};



class PauseCommand: public Command
{
public:
   PauseCommand(ShootingScene* scene);

   virtual bool Execute();

protected:

   ShootingScene* _scene;
};
