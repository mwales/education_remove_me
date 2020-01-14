#include "Maze.h"

Maze::Maze(std::vector<std::string> data)
{
   theMazeData = data;

   theHeight = data.size();

   theWidth = data.front().size();

   std::cerr << "Maze is " << theWidth << "x" << theHeight << std::endl;

   theNumKeys = 0;
   for(int x = 0; x < theWidth; x++)
   {
      for(int y = 0; y < theHeight; y++)
      {
         char data = getPoint(x,y);

         if ( (data >= 'a') && (data <= 'z') )
         {
            theNumKeys++;
         }
      }
   }

   std::cerr << "Number of keys is " << theNumKeys << std::endl;
}

void Maze::findStartPoint(int & retValX, int & retValY)
{
   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         if (getPoint(x,y) == '@')
         {
            retValX = x;
            retValY = y;
            return;
         }
      }
   }

   retValX = -1;
   retValY = -1;
}

char Maze::getPoint(int x, int y)
{
   if ( (x < 0) || (x >= theWidth))
   {
      std::cerr << "getPoint(" << x << "," << y << ") x is invalid!" << std::endl;
      return '?';
   }

   if ( (y < 0) || (y >= theHeight))
   {
      std::cerr << "getPoint(" << x << "," << y << ") y is invalid!" << std::endl;
      return '?';
   }

   return theMazeData.at(y).at(x);
}
