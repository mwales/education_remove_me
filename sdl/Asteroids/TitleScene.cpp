#include "TitleScene.h"
#include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

TitleScene::TitleScene(Graphics* g, Mixer* m):
   Scene(g, m),
   _nextScene(NULL)
{
   _name = "Title";
   _background.SetImageInfo("assets/nebula_blue.png", _renderer);
   _splashImage.SetImageInfo("assets/splash.png", _renderer);

   _background.SetFullscreen(true);

   XYPair splashCenter = g->GetWindowSize();
   splashCenter *= 0.5;

   _splashImage.SetPosition(splashCenter);

   _entities.push_back(&_background);
   _entities.push_back(&_splashImage);

   _mixer->LoadMusic("assets/cobra.ogg");
   //_mixer->PlayMusic();
}

TitleScene::~TitleScene()
{
   _mixer->PauseMusic();
}

bool TitleScene::ProcessEvent(SDL_Event const & ev)
{
   switch(ev.type)
   {
      case SDL_KEYDOWN:
      {
        LOG_DEBUG() << "Keyboard down event";
         if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE)
         {
            LOG_DEBUG() << "Yeah, space bar";

            _nextScene = new ShootingScene(_graphics, _mixer);
            return true;
         }
         break;
      }

      case SDL_MOUSEBUTTONDOWN:
         LOG_DEBUG() << "Mouse button down";
         break;

      case SDL_JOYBUTTONDOWN:
         LOG_DEBUG() << "Joystick button" << ev.jbutton.button << "pressed";
         if ( (ev.jbutton.button == 0) || (ev.jbutton.button == 7) )
         {
            // User pressed A or Start on x360 controller
            _nextScene = new ShootingScene(_graphics, _mixer);
            return true;
         }
         break;
   }

   return false;
}

Scene* TitleScene::GetNextState(bool* deleteMe)
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
