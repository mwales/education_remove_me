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
   theBackground(g->GetWindowSize()),
   theRockSpawnCounter(10),
   theShip(g->GetWindowSize()),
   thePauseState(false),
   theNextState(nullptr),
   theCollisionMgr(g->GetWindowSize()[0], g->GetWindowSize()[1], g->GetWindowSize()[0] / 15),
   theDebugMode(false)
{
   theName = "Shooting";

   theBackground.SetImageInfo("assets/nebula_brown.png", theRenderer);
   theBackground.SetFullscreen(true);

   XYPair shipPosition = g->GetWindowSize();
   shipPosition *= 0.5;

   theShip.SetTiledImageInfo("assets/double_ship.png", theRenderer, 90, 90, 0,
                           TiledImage::PROVIDING_SINGLE_TILE_DIMENSIONS);
   theShip.SetPosition(shipPosition);
   theShip.SetFullscreen(false);
   theShip.SetAddDeleteLists(&theAdditionList, &theDeletionList);

   theEntities.push_back(&theBackground);
   theEntities.push_back(&theShip);

   theCollisionMgr.AddToB(&theShip);

   theKeyboardDownMappedCommands = theShip.GetKeyboardDownCallbacks();
   theKeyboardDownMappedCommands[SDL_SCANCODE_SPACE] = new PauseCommand(this);
   theKeyboardDownMappedCommands[SDL_SCANCODE_P] = new ToggleDebugCommand(this);

   theKeyboardUpMappedCommands = theShip.GetKeyboardUpCallbacks();

   g->GetJoystick()->RegisterCommand(&theShip, true);
   g->GetJoystick()->AddButtonDownHandler(7, new PauseCommand(this));

   //ImageCache::ImageCacheDebugDump();
}

ShootingScene::~ShootingScene()
{
   theGraphics->GetJoystick()->ClearRegisteredCommands();

   std::map<int, Command*>::const_iterator it;
   for (it = theKeyboardDownMappedCommands.begin(); it != theKeyboardDownMappedCommands.end(); it++)
   {
      delete it->second;
   }
   for (it = theKeyboardUpMappedCommands.begin(); it != theKeyboardUpMappedCommands.end(); it++)
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

         auto scanCmd = theKeyboardDownMappedCommands.find(scancode);
         if (scanCmd == theKeyboardDownMappedCommands.end())
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

         std::map<int, Command*>::iterator scanCmd = theKeyboardUpMappedCommands.find(scancode);
         if (scanCmd == theKeyboardUpMappedCommands.end())
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
         if (theGraphics->GetJoystick()->ProcessEvent(ev))
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
   if (theNextState)
   {
      // Is game just paused?
      if (thePauseState)
      {
         *deleteMe = false;
         Scene* retVal = theNextState;
         theNextState = nullptr;
         return retVal;
      }
      else
      {
         *deleteMe = true;
         return theNextState;
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

   thePauseState = true;
   theNextState = new PauseScene(theGraphics, theMixer, this);
}

void ShootingScene::SpawnRock()
{
   theRockSpawnCounter--;

   if (theRockSpawnCounter > 0)
   {
      return;
   }

   // Rock spawn counter hit zero, time to spawn a rock!

   // Set timer for the next attempted rock spawning
   theRockSpawnCounter = theUpdateRateHz / 2; //* 5;

   if (theBigRocks.size() >= MAX_ROCKS)
   {
      LOG_WARNING() << "Can't spawn a rock, too many already";
      return;
   }

   for(auto i = 0; i < 20; i++)
   {
      SpaceRock* rock = new SpaceRock(theGraphics->GetWindowSize(), theRenderer);
      rock->SetRandomLocation(theShip.GetPosition());
      rock->SetUpdateRate(theUpdateRateHz);
      theBigRocks.push_back(rock);
      theEntities.push_back(rock);
      theCollisionMgr.AddToA(rock);

      if (theDebugMode)
      {
         rock->DisplayCollisionArea(true);
      }
   }
}

void ShootingScene::Update()
{
   SpawnRock();

   theCollisionMgr.CheckForCollisions();

   std::vector<Collision> collisions = theCollisionMgr.GetCollisions();

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
      for(rockIt = theBigRocks.begin(); rockIt != theBigRocks.end(); rockIt++)
      {
         if ( (*it).first == *rockIt )
         {
            LOG_DEBUG() << "We found the rock that sploded: (" << (unsigned long) (*it).first << ")";

            //ImageCache::ImageCacheDebugDump();

            // We found the rock in our list
            SpaceRock* splodingRock = *rockIt;
            splodingRock->Explode(&theDeletionList, &theAdditionList);
            theCollisionMgr.RemoveFromA(splodingRock);
            rocksToDelete.insert(splodingRock);

            //ImageCache::ImageCacheDebugDump();
         }
      }


      if (&theShip == (*it).second)
      {
         LOG_DEBUG() << "Our ship explodes too!!";
      }
      else
      {
         LOG_DEBUG() << "Bullet shot a rock";
      }

   }

   // Need to remove from rock list, then delete!
   for (int i = theBigRocks.size() - 1; i >= 0; i--)
   {
      if (rocksToDelete.find(theBigRocks[i]) != rocksToDelete.end())
      {
         theBigRocks.erase(theBigRocks.begin() + i);
      }
   }

   theCollisionMgr.ClearCollisions();

   for(auto bullet : theShip.GetNewBullets())
   {
      theCollisionMgr.AddToB(bullet);
   }

   // Call parent implementation
   Scene::Update();
}

void ShootingScene::ManageEntityLifetimes()
{
   for(GameEntity* ge : theDeletionList)
   {
      theCollisionMgr.RemoveFromB( dynamic_cast<GraphicEntity*>(ge));
   }

   Scene::ManageEntityLifetimes();
}

void ShootingScene::ToggleDebug()
{
   theDebugMode = !theDebugMode;
   LOG_DEBUG() << "Toggling debug mode:" << (theDebugMode ? "On" : "Off");

   for(auto rock : theBigRocks)
   {
      rock->DisplayCollisionArea(theDebugMode);
      theShip.DisplayCollisionArea(theDebugMode);
   }
}

//*****************************************************************************
// PauseCommand
//*****************************************************************************


PauseCommand::PauseCommand(ShootingScene* scene):
   theScene(scene)
{
   // Empty
}

bool PauseCommand::Execute()
{
   theScene->PauseGame();
   return false;
}

ToggleDebugCommand::ToggleDebugCommand(ShootingScene* scene):
   theScene(scene)
{
   // Empty
}

bool ToggleDebugCommand::Execute()
{
   theScene->ToggleDebug();
   return false;
}
