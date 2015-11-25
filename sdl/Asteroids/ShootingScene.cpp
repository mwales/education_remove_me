  #include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

ShootingScene::ShootingScene(Graphics* g, Mixer* m):
   Scene(g, m),
   _ship(g->GetWindowSize())
{
   _name = "Shooting";

   _background.SetImageInfo("assets/nebula_brown.png", _renderer);
   _background.SetFullscreen(true);

   XYPair shipPosition = g->GetWindowSize();
   shipPosition *= 0.5;

   _ship.SetImageInfo("assets/double_ship.png", _renderer);
   _ship.SetPosition(shipPosition);
   _ship.SetFullscreen(false);

   _entities.push_back(&_background);
   _entities.push_back(&_ship);

   _keyboardDownMappedCommands = _ship.GetKeyboardDownCallbacks();
   _keyboardUpMappedCommands = _ship.GetKeyboardUpCallbacks();
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
            scanCmd->second->Execute();
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
            scanCmd->second->Execute();
         }
         break;
      }

   }

   return false;
}

Scene* ShootingScene::GetNextState(bool* deleteMe)
{
   *deleteMe = false;
   return NULL;
}
