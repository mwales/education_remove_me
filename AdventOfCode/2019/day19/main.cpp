#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <ncurses.h>

#include "ElfComputer.h"
#include "BeamDetector.h"



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

   std::vector<int64_t> pd = strListToInt(split(programData, ","));

   BeamDetector bd(pd);

   Coord tl = std::make_pair(0,0);
   Coord br = std::make_pair(49,49);

   int beamDensity = bd.quantifyBeam(tl, br);

   std::cout << "Beam Density = " << beamDensity << std::endl;

   bd.printData(tl, br);

   std::cout << "Moving on to part 2:" << std::endl;

   Coord squarePoint = bd.firstSquareOfSize(100);
   std::cout << "Beam square of 100 @ " << bd.coordToString(squarePoint) << std::endl;

   int part2Sol = squarePoint.first * 10000 + squarePoint.second - 99;
   std::cout << "Part 2 = " << part2Sol << std::endl;

   return 0;
}
