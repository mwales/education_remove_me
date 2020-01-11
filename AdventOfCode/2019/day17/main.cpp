#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <ncurses.h>

#include "ElfComputer.h"
#include "Scaffold.h"



std::string readFile(std::string filename)
{
   std::ifstream f;
   f.open(filename);
   std::string retVal;

   while(true)
   {
      std::string nextLine;
      std::getline(f, nextLine);

      if (f.eof())
      {
         break;
      }

      retVal += nextLine;
   }

   return retVal;
}

std::vector<int64_t> stringToData(std::string input)
{
   std::vector<int64_t> retVal;

   for(auto it = input.begin(); it != input.end(); it++)
   {
      retVal.push_back((int) *it);
   }

   return retVal;
}

void clearTheScreen()
{
   // Found this on stack overflow
   std::cout << "\033[2J\033[1;1H";
}

void outputScreen(std::vector<int64_t> const & q)
{
   clearTheScreen();

   for(auto it = q.begin(); it != q.end(); it++)
   {
      std::cout << (char) *it;
   }

   std::cout << std::endl;

   usleep(100000);
}

int main(int argc, char** argv)
{

   std::string programData;
   std::cin >> programData;

   std::vector<int64_t> inputQ;
   std::vector<int64_t> outputQ;

   std::vector<int64_t> pd = strListToInt(split(programData, ","));

   ElfComputer emu(pd, "emu");
   emu.setInputDataQ(&inputQ);
   emu.setOutputDataQ(&outputQ);

   int numIterations = 0;
   while(!emu.isHalted())
   {
      emu.runIteration();
      numIterations++;
   }

   // Print the map!
   for(auto it = outputQ.begin(); it != outputQ.end(); it++)
   {
      std::cout << (char) *it;
   }

   std::cout << std::endl;

   Scaffold s(outputQ);

   int sumAlign = s.getSumAlignmentParameters();

   std::cout << "Sum of align params: " << sumAlign << std::endl;

   sleep(5);

   std::string pt2Program, funcA, funcB, funcC, liveViewOption;
   std::cin >> pt2Program;
   std::cin >> funcA;
   std::cin >> funcB;
   std::cin >> funcC;
   std::cin >> liveViewOption;

   pd[0] = 2; // Manual control mode
   ElfComputer emuControl(pd, "control");
   inputQ.clear();
   outputQ.clear();

   std::string theWholeThing;
   theWholeThing = pt2Program;
   theWholeThing += "\n";
   theWholeThing += funcA;
   theWholeThing += "\n";
   theWholeThing += funcB;
   theWholeThing += "\n";
   theWholeThing += funcC;
   theWholeThing += "\n";
   theWholeThing += liveViewOption;
   theWholeThing += "\n";

   printIntList(stringToData(theWholeThing));

   inputQ = stringToData(theWholeThing);
   emuControl.setInputDataQ(&inputQ);
   emuControl.setOutputDataQ(&outputQ);

   numIterations = 0;

   // When we see 2 consecutive new lines, clear the screen, output Q, delay
   int prevQSize = 0;
   int consecutiveNewLines = 0;

   while(!emuControl.isHalted())
   {
      emuControl.runIteration();
      numIterations++;

      // Are the last two items in the output Q \n\n?
      if (outputQ.size() != prevQSize)
      {
         if (outputQ.back() == '\n')
         {
            consecutiveNewLines++;
         }
         else
         {
            consecutiveNewLines = 0;
         }

         if (consecutiveNewLines == 2)
         {
            // Output the screen and clear the Q
            outputScreen(outputQ);
            outputQ.clear();
            consecutiveNewLines = 0;
            prevQSize = 0;
         }
         else
         {
            prevQSize = outputQ.size();
         }
      }
   }

   std::cout << "Dust collected: ";
   printIntList(outputQ);


   return 0;
}
