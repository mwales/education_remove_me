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

   ImageInfo i("assets/splash.png", g.GetRenderer());

   Mixer m;
   m.LoadMusic("assets/cobra.ogg");
   m.PlayMusic(-1);


   int updateRateHz = 10;
   int updatePeriodMs = 1000 / updateRateHz;

   TitleScene ts(&g,&m);
   ts.SetUpdateRate(updateRateHz);

   bool stopEvent = false;
   while(!stopEvent)
   {
      int tickCountStart = SDL_GetTicks();

      g.Clear();

      ts.Update();
      ts.Draw();

      g.Render();



      int tickCountEnd = SDL_GetTicks();
      int renderTime = tickCountEnd - tickCountStart;

      if ( renderTime < updatePeriodMs)
      {
         LOG_DEBUG() << "Render time=" << renderTime << ", and render period remaining=" << (updatePeriodMs - renderTime) << "ms";
         SDL_Delay(updatePeriodMs - renderTime);
      }
      else
      {
         LOG_WARNING() << "Render time took too long" << renderTime;
      }

      SDL_Event ev;
      SDL_PollEvent(&ev);

      if (ev.type == SDL_QUIT)
      {
         LOG_DEBUG() << "Quit event received";
         stopEvent = true;
      }

   }

   LOG_DEBUG() << "Out of SDL loop";





   /// @todo Need a cleanup, need to call IMG_Uninit() or whatever the heck it is called
}
