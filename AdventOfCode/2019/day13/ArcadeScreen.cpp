#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>


#include "ElfComputer.h"
#include "ArcadeScreen.h"

ArcadeScreen::ArcadeScreen():
   theWidth(0),
   theHeight(0),
   theBallX(0),
   thePaddleX(0),
   theScore(0)
{
   // nothing
   initscr();

   int row, col;
   getmaxyx(stdscr, row, col);

   std::cout << "Max rows = " << row << " and max col = " << col << std::endl;
}

ArcadeScreen::~ArcadeScreen()
{
   endwin();

   std::cout << "Final Score: " << theScore << std::endl;
}

void ArcadeScreen::processScreenData(ElfCode const & sd)
{


   int numItems = sd.size() / 3;
   for(int i = 0; i < numItems; i++)
   {
      int curX = sd[i*3];
      int curY = sd[i * 3 + 1];
      int curEnum = sd[i*3+2];

      setPixel(curX, curY, curEnum);
   }
}

void ArcadeScreen::printScreen()
{
   erase();

   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         int enumVal = getPixel(x,y);

         switch(enumVal)
         {
         case 0:
            // Empty tile
            mvprintw(y,x," ");
            break;

         case 1:
            // Wall tile
            mvprintw(y,x,"#");
            break;

         case 2:
            // block tile
            mvprintw(y,x,"@");
            break;

         case 3:
            // paddle tile
            mvprintw(y,x,"=");
            break;

         case 4:
            // ball tile
            mvprintw(y,x,"O");
         }
      }

   }

   mvprintw(24,0,"SCORE = %d", theScore);
   refresh();

   usleep(1000);
}

void ArcadeScreen::detectScreenSize(ElfCode const & sd)
{
   int numItems = sd.size() / 3;

   theWidth = 0;
   theHeight = 0;

   for(int i = 0; i < numItems; i++)
   {
      int curX = sd[i*3];
      int curY = sd[i * 3 + 1];

      if (curX > theWidth)
      {
         theWidth = curX;
      }

      if (curY > theHeight)
      {
         theHeight = curY;
      }
   }

   // Add 1 to both of those numbers
   theWidth++;
   theHeight++;

   std::cout << "Screen is " << theWidth << " x " << theHeight << std::endl;
   thePixels.resize(theWidth * theHeight);
}

void ArcadeScreen::setPixel(int x, int y, int val)
{
   if ( (x == -1) && (y == 0) )
   {
      // Special case, update score display
      theScore = val;
      return;
   }

   if ( (x < 0) || (x > theWidth) || (y < 0) || (y > theHeight) )
   {
      std::cerr << "Screen draw failure: " << x << ", " << y << std::endl;
      exit(1);
   }

   if (val == 3)
   {
      // paddle tile
      thePaddleX = x;
   }

   if (val == 4)
   {
      // ball tile
      theBallX = x;
   }

   thePixels[y * theWidth + x] = val;

   printScreen();
}

int ArcadeScreen::getPixel(int x, int y)
{
   if ( (x < 0) || (x > theWidth) || (y < 0) || (y > theHeight) )
   {
      std::cerr << "Screen read failure: " << x << ", " << y << std::endl;
      exit(1);
   }

   return thePixels[y * theWidth + x];
}

void ArcadeScreen::setScreenSize(int w, int h)
{
   theWidth = w;
   theHeight = h;
   thePixels.resize(theWidth * theHeight);
}

int ArcadeScreen::getJoystickPosition()
{
   if (theBallX < thePaddleX)
   {
      return -1;
   }

   if (theBallX > thePaddleX)
   {
      return 1;
   }

   return 0;
}

