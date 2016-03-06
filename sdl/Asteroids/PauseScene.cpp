#include "PauseScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

PauseScene::PauseScene(Graphics* g, Mixer* m, Scene* underneath):
   Scene(g, m),
   theBackScene(underneath),
   thePauseText(g->GetWindowSize()),
   theNextScene(nullptr)
{
   theName = "Pause";

   SDL_Color textColor = { 0xff, 0xff, 0xff, 0xff };
   thePauseText.SetTextImageInfo("Game Paused", textColor, theRenderer);

   XYPair textPosition = g->GetWindowSize();
   textPosition *= 0.5;
   thePauseText.SetPosition(textPosition);

   theEntities.push_back(&thePauseText);
}

PauseScene::~PauseScene()
{
   theMixer->PauseMusic();
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
            theNextScene = theBackScene;
            return true;
         }
         break;
      }

      case SDL_JOYBUTTONDOWN:
         LOG_DEBUG() << "Joystick button" << ev.jbutton.button << "pressed";
         if (ev.jbutton.button == 7)
         {
            // User pressed A or Start on x360 controller
            theNextScene = theBackScene;
            return true;
         }
         break;
   }

   return false;
}

Scene* PauseScene::GetNextState(bool* deleteMe)
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

void PauseScene::Draw()
{
   // Draw the scene underneath the pause screen
   theBackScene->Draw();

   for(std::vector<GameEntity*>::iterator it = theEntities.begin(); it != theEntities.end(); it++)
   {
      (*it)->Draw();
   }
}
