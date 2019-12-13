#include <iostream>
#include <vector>

#include "ElfComputer.h"

// screen data is x,y,enum

class ArcadeScreen
{

public:
   ArcadeScreen();

   void processScreenData(ElfCode const & sd);

   void printScreen();

   int getPixel(int x, int y);

   int getWidth() { return theWidth; }

   int getHeight() { return theHeight; }

protected:

   void determineScreenSize(ElfCode const & sd);

   void setPixel(int x, int y, int val);

   std::vector<int> thePixels;

   int theWidth;

   int theHeight;
};

ArcadeScreen::ArcadeScreen():
   theWidth(0),
   theHeight(0)
{
   // nothing
}

void ArcadeScreen::processScreenData(ElfCode const & sd)
{
   determineScreenSize(sd);

   thePixels.resize(theWidth * theHeight);

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
   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         int enumVal = getPixel(x,y);

         switch(enumVal)
         {
         case 0:
            // Empty tile
            std::cout << " ";
            break;

         case 1:
            // Wall tile
            std::cout << "#";
            break;

         case 2:
            // block tile
            std::cout << "@";
            break;

         case 3:
            // paddle tile
            std::cout << "=";
            break;

         case 4:
            // ball tile
            std::cout << "O";
         }
      }

      std::cout << std::endl;
   }

}

void ArcadeScreen::determineScreenSize(ElfCode const & sd)
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
}

void ArcadeScreen::setPixel(int x, int y, int val)
{
   if ( (x < 0) || (x > theWidth) || (y < 0) || (y > theHeight) )
   {
      std::cerr << "Screen draw failure: " << x << ", " << y << std::endl;
      exit(1);
   }

   thePixels[y * theWidth + x] = val;
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

int main(int, char**)
{
	std::string programData;
	std::cin >> programData;

	ElfCode pd = strListToInt(split(programData, ","));

	ElfComputer e(pd, "arcade");

	ElfCode inputQ;
	ElfCode outputQ;
	e.setInputDataQ(&inputQ);
	e.setOutputDataQ(&outputQ);

	while(!e.isHalted())
	{
		e.runIteration();
	}

   ArcadeScreen screen;
   screen.processScreenData(outputQ);
   screen.printScreen();

   // part 1, count the blocks
   int numBlocks = 0;
   for(int x = 0; x < screen.getWidth(); x++)
   {
      for(int y = 0; y < screen.getHeight(); y++)
      {
         if (screen.getPixel(x, y) == 2)
         {
            numBlocks++;
         }
      }
   }

   std::cout << "Num blocks = " << numBlocks << std::endl;

	return 0;
}
