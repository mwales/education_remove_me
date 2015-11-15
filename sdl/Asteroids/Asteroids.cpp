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



int main (int argc, char* argv[])
{

   LOG_WARNING() << "Here is some stuff";

   XYPair gv = GameMath::GetUnitVector(2);

   std::string crap = "My uv=";
   crap += GameMath::VectorToString(gv);

   LOG_DEBUG() << crap << "and magnitude" << GameMath::VectorMagnitude(gv);

   Logger() << "I'm stupd";

   LOG_DEBUG() << "stuff" << 134 << " hoes";




   LOG_FATAL() << "doh, gonna die now";
   LOG_DEBUG() << "Never get here";


   /// @todo Need a cleanup, need to call IMG_Uninit() or whatever the heck it is called
}
