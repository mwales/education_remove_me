#include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "PauseScene.h"
#include "SpaceRock.h"
#include "ImageCache.h"

const int MAX_ROCKS = 100;

ShootingScene::ShootingScene(Graphics* g, Mixer* m):
   Scene(g, m),
   _background(g->GetWindowSize()),
   _rockSpawnCounter(10),
   _ship(g->GetWindowSize()),
   _pauseState(false),
   _nextState(nullptr),
   _collisionMgr(g->GetWindowSize()[0], g->GetWindowSize()[1], g->GetWindowSize()[0] / 15),
   _debugMode(false)
{
   _name = "Shooting";

   _background.SetImageInfo("assets/nebula_brown.png", _renderer);
   _background.SetFullscreen(true);

   XYPair shipPosition = g->GetWindowSize();
   shipPosition *= 0.5;

   _ship.SetTiledImageInfo("assets/double_ship.png", _renderer, 90, 90, 0,
                           TiledImage::PROVIDING_SINGLE_TILE_DIMENSIONS);
   _ship.SetPosition(shipPosition);
   _ship.SetFullscreen(false);
   _ship.SetAddDeleteLists(&_additionList, &_deletionList);

   _entities.push_back(&_background);
   _entities.push_back(&_ship);

   _collisionMgr.AddToB(&_ship);

   _keyboardDownMappedCommands = _ship.GetKeyboardDownCallbacks();
   _keyboardDownMappedCommands[SDL_SCANCODE_SPACE] = new PauseCommand(this);
   _keyboardDownMappedCommands[SDL_SCANCODE_P] = new ToggleDebugCommand(this);

   _keyboardUpMappedCommands = _ship.GetKeyboardUpCallbacks();

   g->GetJoystick()->RegisterCommand(&_ship, true);
   g->GetJoystick()->AddButtonDownHandler(7, new PauseCommand(this));

   //ImageCache::ImageCacheDebugDump();
}

ShootingScene::~ShootingScene()
{
   _graphics->GetJoystick()->ClearRegisteredCommands();

   std::map<int, Command*>::const_iterator it;
   for (it = _keyboardDownMappedCommands.begin(); it != _keyboardDownMappedCommands.end(); it++)
   {
      delete it->second;
   }
   for (it = _keyboardUpMappedCommands.begin(); it != _keyboardUpMappedCommands.end(); it++)
   {
      delete it->second;
   }
}

bool ShootingScene::ProcessEvent(SDL_Event const & ev)
{

   switch (ev.type)
   {
      case SDL_KEYDOWN:
      {
         if (ev.key.repeat)
         {
            // Ignore repeating keys
            break;
         }

         int scancode = ev.key.keysym.scancode;

         auto scanCmd = _keyboardDownMappedCommands.find(scancode);
         if (scanCmd == _keyboardDownMappedCommands.end())
         {
            LOG_DEBUG() << "Keystroke down not mapped to command";
         }
         else
         {
            if (scanCmd->second->Execute())
            {
               // Scene will change now
               return true;
            }
         }
         break;
      }
      case SDL_KEYUP:
      {
         if (ev.key.repeat)
         {
            // Ignore repeating keys
            break;
         }
         int scancode = ev.key.keysym.scancode;

         std::map<int, Command*>::iterator scanCmd = _keyboardUpMappedCommands.find(scancode);
         if (scanCmd == _keyboardUpMappedCommands.end())
         {
            LOG_DEBUG() << "Keystroke up not mapped to command";
         }
         else
         {
            if (scanCmd->second->Execute())
            {
               // Scene will change now
               return true;
            }
         }
         break;
      }
      case SDL_JOYAXISMOTION:
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
      {
         if (_graphics->GetJoystick()->ProcessEvent(ev))
         {
            // Scene will change now
            return true;
         }
         break;
      }
   }

   return false;
}

Scene* ShootingScene::GetNextState(bool* deleteMe)
{
   if (_nextState)
   {
      // Is game just paused?
      if (_pauseState)
      {
         *deleteMe = false;
         Scene* retVal = _nextState;
         _nextState = nullptr;
         return retVal;
      }
      else
      {
         *deleteMe = true;
         return _nextState;
      }
   }
   else
   {
      *deleteMe = false;
      return nullptr;
   }

}

void ShootingScene::PauseGame()
{
   LOG_DEBUG() << "Pausing game";

   _pauseState = true;
   _nextState = new PauseScene(_graphics, _mixer, this);
}

void ShootingScene::SpawnRock()
{
   _rockSpawnCounter--;

   if (_rockSpawnCounter > 0)
   {
      return;
   }

   // Rock spawn counter hit zero, time to spawn a rock!

   // Set timer for the next attempted rock spawning
   _rockSpawnCounter = _updateRateHz / 2; //* 5;

   if (_bigRocks.size() >= MAX_ROCKS)
   {
      LOG_WARNING() << "Can't spawn a rock, too many already";
      return;
   }

   for(auto i = 0; i < 20; i++)
   {
      SpaceRock* rock = new SpaceRock(_graphics->GetWindowSize(), _renderer);
      rock->SetRandomLocation(_ship.GetPosition());
      rock->SetUpdateRate(_updateRateHz);
      _bigRocks.push_back(rock);
      _entities.push_back(rock);
      _collisionMgr.AddToA(rock);

      if (_debugMode)
      {
         rock->DisplayCollisionArea(true);
      }
   }
}

void ShootingScene::Update()
{
   SpawnRock();

   _collisionMgr.CheckForCollisions();

   std::vector<Collision> collisions = _collisionMgr.GetCollisions();

   /// @todo More collision management code needed!!!
   ///       What if ship hits 2 rocks at once?
   ///       Bullet vs Rock
   ///       Add exploding ship
   ///
   std::set<SpaceRock*> rocksToDelete;

   std::vector<Collision>::iterator it;
   for(it = collisions.begin(); it != collisions.end(); it++)
   {
      // Find the rock in the list of rocks, and explode it, and delete it from collision manager
      std::vector<SpaceRock*>::iterator rockIt;
      for(rockIt = _bigRocks.begin(); rockIt != _bigRocks.end(); rockIt++)
      {
         if ( (*it).first == *rockIt )
         {
            LOG_DEBUG() << "We found the rock that sploded: (" << (unsigned long) (*it).first << ")";

            //ImageCache::ImageCacheDebugDump();

            // We found the rock in our list
            SpaceRock* splodingRock = *rockIt;
            splodingRock->Explode(&_deletionList, &_additionList);
            _collisionMgr.RemoveFromA(splodingRock);
            rocksToDelete.insert(splodingRock);

            //ImageCache::ImageCacheDebugDump();
         }
      }


      if (&_ship == (*it).second)
      {
         LOG_DEBUG() << "Our ship explodes too!!";
      }
      else
      {
         LOG_DEBUG() << "Bullet shot a rock";
      }

   }

   // Need to remove from rock list, then delete!
   for (int i = _bigRocks.size() - 1; i >= 0; i--)
   {
      if (rocksToDelete.find(_bigRocks[i]) != rocksToDelete.end())
      {
         _bigRocks.erase(_bigRocks.begin() + i);
      }
   }

   _collisionMgr.ClearCollisions();

   for(auto bullet : _ship.GetNewBullets())
   {
      _collisionMgr.AddToB(bullet);
   }

   // Call parent implementation
   Scene::Update();
}

void ShootingScene::ManageEntityLifetimes()
{
   for(GameEntity* ge : _deletionList)
   {
      _collisionMgr.RemoveFromB( dynamic_cast<GraphicEntity*>(ge));
   }

   Scene::ManageEntityLifetimes();
}

void ShootingScene::ToggleDebug()
{
   _debugMode = !_debugMode;
   LOG_DEBUG() << "Toggling debug mode:" << (_debugMode ? "On" : "Off");

   for(auto rock : _bigRocks)
   {
      rock->DisplayCollisionArea(_debugMode);
      _ship.DisplayCollisionArea(_debugMode);
   }
}

//*****************************************************************************
// PauseCommand
//*****************************************************************************


PauseCommand::PauseCommand(ShootingScene* scene):
   _scene(scene)
{
   // Empty
}

bool PauseCommand::Execute()
{
   _scene->PauseGame();
   return false;
}

ToggleDebugCommand::ToggleDebugCommand(ShootingScene* scene):
   _scene(scene)
{
   // Empty
}

bool ToggleDebugCommand::Execute()
{
   _scene->ToggleDebug();
   return false;
}
