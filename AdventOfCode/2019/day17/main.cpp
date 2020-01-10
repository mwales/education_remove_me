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



int main(int argc, char** argv)
{

   std::string programData;
   std::cin >> programData;

   std::vector<int64_t> inputQ;
   std::vector<int64_t> outputQ;

   ElfComputer emu(strListToInt(split(programData, ",")), "emu");
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

   return 0;
}
