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

 Pie in the sky stuff to do:
 - Create a stop watch telemetry class.
  - How long does it take to Update?
  - to render?
  - to post process (new and deletes)
  - special events
  - timing graphs
 - Debug log receivers
   - tcp/ip
   - named pipe
   - cout



 */


#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include "GameMath.h"
#include "Logger.h"
#include "Graphics.h"
#include "ImageInfo.h"
#include "Mixer.h"
#include "TitleScene.h"
#include "TextImage.h"
#include "Scene.h"
#include "EventRecorder.h"

void sdlScopeMain()
{
   Graphics g;
   g.CreateWindow("Asteroids", 800, 600);
   g.DisplayRendererInfo();

   Mixer m;

   TextImage::LoadFont("assets/font/SuperMario256.ttf", 32);

   int updateRateHz = 60;
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

      currentScene->ManageEntityLifetimes();

      int tickCountEnd = SDL_GetTicks();
      int renderTime = tickCountEnd - tickCountStart;

      if ( renderTime < updatePeriodMs)
      {
         LOG_WARNING() << "Render time" << renderTime << ", period usage=" << (int) renderTime * 100 / updatePeriodMs << "%";
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
}

int main (int argc, char* argv[])
{
   // Initialize the random number generator
   srand(time(NULL));

   // Do everything SDL in another scope so that when we want to quit, we
   // know everything SDL related is really finished
   sdlScopeMain();

   // Delete the singleton EventRecorder
   EventRecorder::DestroyInstance();

   TextImage::UnloadAllFonts();

   if (TTF_WasInit())
   {
      LOG_DEBUG() << "Stopping TTF SDL";
      TTF_Quit();
   }
   else
   {
      LOG_WARNING() << "TTF SDL was never started / loaded";
   }

   LOG_DEBUG() << "Stopping Image SDL";
   IMG_Quit();

   LOG_DEBUG() << "Stopping Mixer / Audio SDL";
   Mix_Quit();

   return 0;
}
