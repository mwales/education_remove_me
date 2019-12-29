#include <iostream>
#include <vector>

#include "ElfComputer.h"
#include "ArcadeScreen.h"

int main(int, char**)
{
   std::string inputData;
   std::cin >> inputData;
   ElfCode inputQ = strListToInt(split(inputData, ","));

   std::string programData;
   std::cin >> programData;

   ElfCode pd = strListToInt(split(programData, ","));

   ElfComputer e(pd, "arcade");

   ElfCode outputQ;
   e.setInputDataQ(&inputQ);
   e.setOutputDataQ(&outputQ);

   while(!e.isHalted())
   {
      e.runIteration();
   }
   
   int numBlocks = 0;

   {
      ArcadeScreen screen;
      screen.detectScreenSize(outputQ);
      screen.processScreenData(outputQ);
      screen.printScreen();

      // part 1, count the blocks
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
   }
   std::cout << "Num blocks = " << numBlocks << std::endl;

   return 0;
}
