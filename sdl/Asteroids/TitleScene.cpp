#include "TitleScene.h"
#include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

TitleScene::TitleScene(Graphics* g, Mixer* m):
   Scene(g, m),
   theBackground(g->GetWindowSize()),
   theSplashImage(g->GetWindowSize()),
   theAuthorName(g->GetWindowSize()),
   theNextScene(nullptr)
{
   theName = "Title";
   theBackground.SetImageInfo("assets/nebula_blue.png", theRenderer);
   theSplashImage.SetImageInfo("assets/splash.png", theRenderer);

   SDL_Color nameColor = { 0xff, 0xff, 0xff, 0xff };
   theAuthorName.SetTextImageInfo("Created by Michael Wales", nameColor, theRenderer);

   theBackground.SetFullscreen(true);

   XYPair namePosition = g->GetWindowSize();
   namePosition[1] *= 0.75;
   namePosition[0] *= 0.5;
   theAuthorName.SetPosition(namePosition);

   XYPair splashCenter = g->GetWindowSize();
   splashCenter *= 0.5;
   theSplashImage.SetPosition(splashCenter);

   theEntities.push_back(&theBackground);
   theEntities.push_back(&theSplashImage);
   theEntities.push_back(&theAuthorName);

   theMixer->LoadMusic("assets/cobra.ogg");
   //_mixer->PlayMusic();
}

TitleScene::~TitleScene()
{
   theMixer->PauseMusic();
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

            theNextScene = new ShootingScene(theGraphics, theMixer);
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
            theNextScene = new ShootingScene(theGraphics, theMixer);
            return true;
         }
         break;
   }

   return false;
}

Scene* TitleScene::GetNextState(bool* deleteMe)
{
   if (theNextScene)
   {
      *deleteMe = true;
   }
   else
   {
      *deleteMe = false;
   }
   return theNextScene;
}
