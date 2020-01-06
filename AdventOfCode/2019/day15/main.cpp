#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <ncurses.h>

#include "ElfComputer.h"
#include "MazeCommon.h"
#include "MazeViewer.h"
#include "MazeSolver.h"

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

int getNextInput(MazeViewer & v)
{
   char c = getch();

   switch(c)
   {
   case 'j':
      v.onScreenDebug("Moving down!");
      return MAZE_SOUTH;

   case 'k':
      v.onScreenDebug("Moving up!");
      return MAZE_NORTH;

   case 'h':
      v.onScreenDebug("Moving left!");
      return MAZE_WEST;

   case 'l':
      v.onScreenDebug("Moving right!");
      return MAZE_EAST;

   case 'q':
      v.onScreenDebug("Quitting...");
      return -1;

   default:
      return getNextInput(v);

   }


}

void runInteractiveMode(ElfCode pd)
{
   ElfComputer emu(pd, "interactivemaze");
   std::vector<int64_t> inputQ;
   std::vector<int64_t> outputQ;
   emu.setInputDataQ(&inputQ);
   emu.setOutputDataQ(&outputQ);

   MazeViewer v;
   int posX = 0;
   int posY = 0;
   v.setDroidPosition(posX, posY);

   v.onScreenDebug("Starting up!");

   int moveDirection = getNextInput(v);

   while( (moveDirection != -1) && !emu.isHalted() )
   {
      inputQ.push_back(moveDirection);

      int newX = posX;
      int newY = posY;
      switch(moveDirection)
      {
      case MAZE_NORTH:
         newY = posY + 1;
         break;

      case MAZE_SOUTH:
         newY = posY - 1;
         break;

      case MAZE_WEST:
         newX = posX - 1;
         break;

      case MAZE_EAST:
         newX = posX + 1;
         break;
      }

      int numIterations = 0;
      while(!emu.isHalted() && outputQ.empty())
      {
         emu.runIteration();
         numIterations++;
      }

      std::string debugMsg;
      debugMsg = "Number of iterations to move =";
      debugMsg += std::to_string(numIterations);
      v.onScreenDebug(debugMsg);
      //sleep(2);

      int moveResult = outputQ[0];
      outputQ.clear();

      switch(moveResult)
      {
      case MAZE_WALL: // Droid hit wall
         v.onScreenDebug("Hit wall");
         v.addPoint(newX, newY, MazeViewer::WALL);
         break;

      case MAZE_EMPTY: // Droid move success
         v.onScreenDebug("Move success");
         v.addPoint(newX, newY, MazeViewer::EMPTY);
         v.setDroidPosition(newX, newY);
         posX = newX;
         posY = newY;
         break;

      case MAZE_GOAL: // Droid move success + goal found
         v.onScreenDebug("Goal Found!");
         v.addPoint(newX, newY, MazeViewer::GOAL);
         v.setDroidPosition(newX, newY);
         break;

      default:
         v.onScreenDebug("WTF Result");
      }

      v.printScreen();

      moveDirection = getNextInput(v);
   }

   v.printScreen();

   sleep(3);

}

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      std::cout << "You need to provide the filename of the maze code" << std::endl;
      std::cout << "For interactive mode: " << argv[0] << " mazecode.txt interactive"
                << std::endl;
      return 1;
   }

   std::string programData = readFile(argv[1]);

   // std::cout << "Elf Code:" << std::endl;
   // std::cout << programData << std::endl;
   ElfCode pd = strListToInt(split(programData, ","));

   if ( (argc == 3) && (strcmp(argv[2], "interactive") == 0))
   {
      runInteractiveMode(pd);
      return 0;
   }

   std::vector<int> finalPath;
   int longestPath;
   Coord topLeft, bottomRight;
   {
      MazeSolver solver(pd);
      finalPath = solver.findPathToExit(true);

      solver.reset();
      sleep(5);

      solver.findPathToExit(false);
      longestPath = solver.longestPath();
      solver.getMapLimits(topLeft, bottomRight);
      solver.printMap();
   }

   std::cout << "Pt 1: Number of steps " << finalPath.size() << std::endl;
   std::cout << "Pt 2: Longest path " << longestPath << std::endl;

   // std::cout << "Top Left: " << topLeft.toString() << std::endl;
   // std::cout << "Bottom Right: " << bottomRight.toString() << std::endl;

   return 0;
}
