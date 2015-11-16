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
 * GameNode
 *   GraphicNode
 *     Ship
 *     Missle
 *     Rock
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



int main (int argc, char* argv[])
{

   LOG_WARNING() << "Here is some stuff";



   Graphics g;
   g.CreateWindow("Test", 1200, 1000);

   ImageInfo i("assets/splash.png", g.GetRenderer());



   for(int counter = 0; counter < 10; counter++)
   {
      g.Clear();
      i.DrawFullScreen();
      g.Render();

      SDL_Delay(1000);
   }






   /// @todo Need a cleanup, need to call IMG_Uninit() or whatever the heck it is called
}
