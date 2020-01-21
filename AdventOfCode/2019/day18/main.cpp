#include <iostream>
#include <vector>

#include "Maze.h"
#include "MazeSolver2.h"

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
   m.printState();

   MazeSolver2 ms2(&m);
   std::set<char> keysToGet = m.getPossibleKeys2();
   std::vector<char> keysHave;
   std::vector<MazePath> res = ms2.distToGetFollowingKeys(keysToGet, keysHave, '@');


   bool first = true;
   MazePath best;
   for(auto it = res.begin(); it != res.end(); it++)
   {
      std::cout << "Path: " << mazePathToString(*it) << std::endl;

      if (first)
      {
         first = false;
         best = *it;
      }
      else
      {
         if (best.theLength > it->theLength)
         {
            best = *it;
         }
      }
   }

   std::cout << "\n\nBest Path!\n" << mazePathToString(best) << std::endl;

   return 0;
}
