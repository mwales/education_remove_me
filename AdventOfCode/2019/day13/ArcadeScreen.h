#ifndef ARCADE_SCREEN_H
#define ARCADE_SCREEN_H

#include <iostream>
#include <vector>

#include "ElfComputer.h"

// screen data is x,y,enum
// score data is -1, 0, score

class ArcadeScreen
{

public:
   ArcadeScreen();
   ~ArcadeScreen();

   void processScreenData(ElfCode const & sd);

   void printScreen();

   int getPixel(int x, int y);

   int getWidth() { return theWidth; }

   int getHeight() { return theHeight; }

   void setScreenSize(int w, int h);

   void detectScreenSize(ElfCode const & sd);

   int getJoystickPosition();

   int getScore() { return theScore; }

protected:

   void setPixel(int x, int y, int val);

   std::vector<int> thePixels;

   int theWidth;

   int theHeight;

   int theBallX;

   int thePaddleX;

   int theScore;
};

#endif
