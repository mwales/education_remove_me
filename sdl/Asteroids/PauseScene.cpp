#include "PauseScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

PauseScene::PauseScene(Graphics* g, Mixer* m, Scene* underneath):
   Scene(g, m),
   _underneath(underneath),
   _pauseText(g->GetWindowSize()),
   _nextScene(nullptr)
{
   _name = "Pause";

   SDL_Color textColor = { 0xff, 0xff, 0xff, 0xff };
   _pauseText.SetTextImageInfo("Game Paused", textColor, _renderer);

   XYPair textPosition = g->GetWindowSize();
   textPosition *= 0.5;
   _pauseText.SetPosition(textPosition);

   _entities.push_back(&_pauseText);
}

PauseScene::~PauseScene()
{
   _mixer->PauseMusic();
}

bool PauseScene::ProcessEvent(SDL_Event const & ev)
{
   switch(ev.type)
   {
      case SDL_KEYDOWN:
      {
        LOG_DEBUG() << "Keyboard down event";
         if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE)
         {
            LOG_DEBUG() << "Yeah, space bar";
            _nextScene = _underneath;
            return true;
         }
         break;
      }

      case SDL_JOYBUTTONDOWN:
         LOG_DEBUG() << "Joystick button" << ev.jbutton.button << "pressed";
         if (ev.jbutton.button == 7)
         {
            // User pressed A or Start on x360 controller
            _nextScene = _underneath;
            return true;
         }
         break;
   }

   return false;
}

Scene* PauseScene::GetNextState(bool* deleteMe)
{
   if (_nextScene)
   {
      *deleteMe = true;
   }
   else
   {
      *deleteMe = false;
   }
   return _nextScene;
}

void PauseScene::Draw()
{
   // Draw the scene underneath the pause screen
   _underneath->Draw();

   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      (*it)->Draw();
   }
}
