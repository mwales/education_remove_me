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

   // Put it in free play mode
   pd[0] = 2;

	ElfComputer e(pd, "arcade");

   ElfCode outputQ;
	e.setInputDataQ(&inputQ);
	e.setOutputDataQ(&outputQ);

   ArcadeScreen screen;
   screen.setScreenSize(42,23);

   int numScreenDraws = 0;
   bool inputQFirstEmptied = false;

	while(!e.isHalted())
	{
		e.runIteration();

      // Do we have enough output to update the screen?
      if (outputQ.size() >= 3)
      {
         ElfCode drawCmd;

         for(int i = 0; i < 3; i++)
         {
            drawCmd.push_back(outputQ.at(0));
            outputQ.erase(outputQ.begin());
         }

         screen.processScreenData(drawCmd);

         if (numScreenDraws > 10)
         {
            numScreenDraws = 0;
            screen.printScreen();
         }
         else
         {
            numScreenDraws++;
         }
      }

      // If the input Q is empty, refill
      if (inputQFirstEmptied || (inputQ.size() == 0) )
      {
         inputQ.clear();
         inputQ.push_back(screen.getJoystickPosition());
         inputQFirstEmptied = true;
      }
	}

   screen.printScreen();

	return 0;
}
