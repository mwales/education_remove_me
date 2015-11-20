/**
 *
 * Classes in the original python version:
 *
 * ImageInfo
 * Sprite
 * Ship
 *
 * Convert to:
 * Scene
 *   TitleScreen
 *   Game
 *   GameOver
 *   Credits
 * GameEntity
 *   GraphicEntity
 *     MovingEntity
 *       Ship
 *       Missle
 *       Rock
 *   Music Node
 *     TitleMusic
 *     GameMusic
 * Image
 *   AnimatedImage

 */


#include <iostream>
#include <stdio.h>

#include <sstream>
#include <string>




#include "GameMath.h"
#include "Logger.h"
#include "Graphics.h"
#include "ImageInfo.h"
#include "Mixer.h"
#include "TitleScene.h"



int main (int argc, char* argv[])
{
   Graphics g;
   g.CreateWindow("Test", 800, 600);

   Mixer m;

   int updateRateHz = 10;
   int updatePeriodMs = 1000 / updateRateHz;

   Scene* currentScene = new TitleScene(&g,&m);
   currentScene->SetUpdateRate(updateRateHz);

   bool stopFlag = false;
   while(!stopFlag)
   {
      int tickCountStart = SDL_GetTicks();

      g.Clear();

      currentScene->Update();
      currentScene->Draw();

      g.Render();

      int tickCountEnd = SDL_GetTicks();
      int renderTime = tickCountEnd - tickCountStart;

      if ( renderTime < updatePeriodMs)
      {
         stopFlag = currentScene->PollInputs(updatePeriodMs - renderTime);
      }
      else
      {
         LOG_WARNING() << "Render time took too long" << renderTime;
         stopFlag = currentScene->PollInputs(0);
      }

      // Should we scene transition?
      bool deleteScene;
      Scene* nextScene = currentScene->GetNextState(&deleteScene);

      if (nextScene)
      {
         LOG_DEBUG() << "Scene transition from" << currentScene->GetSceneName() << "to" << nextScene->GetSceneName();

         if (deleteScene)
         {
            delete currentScene;
         }

         currentScene = nextScene;
         currentScene->SetUpdateRate(updateRateHz);
      }

   }

   LOG_DEBUG() << "Out of SDL loop";

   delete currentScene;
   /// @todo Need a cleanup, need to call IMG_Uninit() or whatever the heck it is called
}
