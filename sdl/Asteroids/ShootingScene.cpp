#include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "PauseScene.h"


ShootingScene::ShootingScene(Graphics* g, Mixer* m):
   Scene(g, m),
   _ship(g->GetWindowSize()),
   _pauseState(false),
   _nextState(NULL)
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

   _entities.push_back(&_background);
   _entities.push_back(&_ship);

   _keyboardDownMappedCommands = _ship.GetKeyboardDownCallbacks();
   _keyboardDownMappedCommands[SDL_SCANCODE_SPACE] = new PauseCommand(this);

   _keyboardUpMappedCommands = _ship.GetKeyboardUpCallbacks();

   g->GetJoystick()->RegisterCommand(&_ship, true);
   g->GetJoystick()->AddButtonDownHandler(7, new PauseCommand(this));
}

ShootingScene::~ShootingScene()
{
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

         std::map<int, Command*>::iterator scanCmd = _keyboardDownMappedCommands.find(scancode);
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
         _nextState = NULL;
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
      return NULL;
   }

}

void ShootingScene::PauseGame()
{
   LOG_DEBUG() << "Pausing game";

   _pauseState = true;
   _nextState = new PauseScene(_graphics, _mixer, this);
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
