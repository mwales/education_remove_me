#include <iostream>
#include <vector>

#include "Maze.h"
#include "MazeSolver.h"

int main(int argc, char** argv)
{
   std::vector<std::string> mazeData;

   while(true)
   {
      std::string text;
      std::cin >> text;

      if (std::cin.eof())
      {
         break;
      }

      mazeData.push_back(text);
   }

   Maze m(mazeData);

   int x = 0;
   int y = 0;
   m.findStartPoint(x,y);

   MazeSolver solver(&m, x, y, 0);

   int pt1 = solver.numStepsToSolution();

   solver.printSolverStats();

   std::cout << "pt1 = " << pt1 << std::endl;

   return 0;
}
