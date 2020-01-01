#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <ncurses.h>

#include "ElfComputer.h"
#include "MazeViewer.h"

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
      return 2; // south

   case 'k':
      v.onScreenDebug("Moving up!");
      return 1; // north

   case 'h':
      v.onScreenDebug("Moving left!");
      return 3; // west

   case 'l':
      v.onScreenDebug("Moving right!");
      return 4; // east

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
      case 1: // north
         newY = posY + 1;
         break;

      case 2: // south
         newY = posY - 1;
         break;

      case 3: // west
         newX = posX - 1;
         break;

      case 4: // east
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
      case 0: // Droid hit wall
         v.onScreenDebug("Hit wall");
         v.addPoint(newX, newY, MazeViewer::WALL);
         break;

      case 1: // Droid move success
         v.onScreenDebug("Move success");
         v.addPoint(newX, newY, MazeViewer::EMPTY);
         v.setDroidPosition(newX, newY);
         posX = newX;
         posY = newY;
         break;

      case 2: // Droid move success + goal found
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
      return 1;
   }

   std::string programData = readFile(argv[1]);

   // std::cout << "Elf Code:" << std::endl;
   // std::cout << programData << std::endl;
   ElfCode pd = strListToInt(split(programData, ","));

   if ( (argc == 3) && (strcmp(argv[2], "interactive") == 0))
   {
      runInteractiveMode(pd);
   }

//   ElfComputer e(pd, "maze");

//   ElfCode outputQ;
//   e.setInputDataQ(&inputQ);
//   e.setOutputDataQ(&outputQ);

//   while(!e.isHalted())
//   {
//      e.runIteration();
//   }
   
//   int numBlocks = 0;

//   {
//      ArcadeScreen screen;
//      screen.detectScreenSize(outputQ);
//      screen.processScreenData(outputQ);
//      screen.printScreen();

//      // part 1, count the blocks
//      for(int x = 0; x < screen.getWidth(); x++)
//      {
//         for(int y = 0; y < screen.getHeight(); y++)
//         {
//            if (screen.getPixel(x, y) == 2)
//            {
//               numBlocks++;
//            }
//         }
//      }
//   }
//   std::cout << "Num blocks = " << numBlocks << std::endl;

   return 0;
}
