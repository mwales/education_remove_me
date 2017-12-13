#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<stdlib.h>

/**
--- Day 11: Hex Ed ---

Crossing the bridge, you've barely reached the other side of the stream when a program comes up to
you, clearly in distress. "It's my child process," she says, "he's gotten lost in an infinite grid!"

Fortunately for her, you have plenty of experience with infinite grids.

Unfortunately for you, it's a hex grid.

The hexagons ("hexes") in this grid are aligned such that adjacent hexes can be found to the north,
northeast, southeast, south, southwest, and northwest:

  \ n  /
nw +--+ ne
  /    \
-+      +-
  \    /
sw +--+ se
  / s  \

You have the path the child process took. Starting where he started, you need to determine the
fewest number of steps required to reach him. (A "step" means to move from the hex you are in to
any adjacent hex.)

For example:

ne,ne,ne is 3 steps away.
ne,ne,sw,sw is 0 steps away (back where you started).
ne,ne,s,s is 2 steps away (se,se).
se,sw,se,sw,sw is 3 steps away (s,s,sw).


--- Part Two ---

How many steps away is the furthest he ever got from his starting position?
*/

std::vector<std::string> split(std::string const & s, char delimiter)
{
   std::vector<std::string> retVal;

   std::string part;
   for(auto it = s.begin(); it != s.end(); it++)
   {
      if (*it == delimiter)
      {
         retVal.push_back(part);
         part = "";
         continue;
      }

      part += *it;
   }

   if (!part.empty())
   {
      retVal.push_back(part);
   }

   return retVal;
}

void convertListToFreqMap(std::vector<std::string> const & list,
                          std::map<std::string, int> & freqMapOut)
{
   for(auto it = list.begin(); it != list.end(); it++)
   {
      if (freqMapOut.find(*it) != freqMapOut.end())
      {
         // This string already exists in the map
         freqMapOut[*it] += 1;
         // std::cout << "Adding another to " << *it << std::endl;
      }
      else
      {
         freqMapOut[*it]= 1;
         // std::cout << "Found a fresh " << *it << std::endl;
      }
   }
}

void dumpMap(std::map<std::string, int> const & m)
{
   for(auto it = m.begin(); it != m.end(); it++)
   {
      std::cout << it->first << " = " << it->second << std::endl;
   }
}

void cancelOpposites(std::map<std::string, int> & m, std::string a, std::string b)
{
   // If either A or B doesn't exist in map, exit out
   if (m.find(a) == m.end())
   {
      return;
   }

   if (m.find(b) == m.end())
   {
      return;
   }

   if (m[a] >= m[b])
   {
      m[a] -= m[b];
      m[b] = 0;
   }
   else
   {
      m[b] -= m[a];
      m[a] = 0;
   }
}

int calculateNumMoves(std::vector<std::string> moveList)
{
   std::map<std::string, int> dirMap;

   convertListToFreqMap(moveList, dirMap);

   // std::cout << "Map before:" << std::endl;
   // dumpMap(dirMap);

   cancelOpposites(dirMap, "n", "s");
   cancelOpposites(dirMap, "sw", "ne");
   cancelOpposites(dirMap, "se", "nw");

   // std::cout << std::endl << "Reduced Map:" << std::endl;
   // dumpMap(dirMap);

   // Moving diagonal is one unit W or E, and a 1/2 unit N or S
   // Moving N or S moves a full unit N or S

   // Rather than use half-units / double, we will track half-units of x and y
   // n or s will be y +/- 2
   // others will be y +/- 1, and x +/- 2
   int x = 0;
   int y = 0;
   for(auto it = dirMap.begin(); it != dirMap.end(); it++)
   {
      std::string direction = it->first;
      if (direction == "n")
      {
         y += (it->second * 2);
      }
      else if (direction == "s")
      {
         y -= (it->second * 2);
      }
      else if (direction == "ne")
      {
         y += it->second;
         x += (it->second * 2);
      }
      else if (direction == "nw")
      {
         y += it->second;
         x -= (it->second * 2);
      }
      else if (direction == "se")
      {
         y -= it->second;
         x += (it->second * 2);
      }
      else if (direction == "sw")
      {
         y -= it->second;
         x -= (it->second * 2);
      }

   }

   // std::cout << "x = " << x << ", y = " << y << std::endl;

   int xAbs = abs(x);
   int yAbs = abs(y);

   // This is the clever part of this, any moves in the E or W directions will also take care of
   // a half-unit of N or S displacement
   yAbs -= xAbs / 2;

   if (yAbs < 0)
      yAbs = 0;

   // Convert our half-unit counts back to normal unit counts
   xAbs /= 2;
   yAbs /= 2;

   // std::cout << "x moves = " << xAbs << ", y moves = " << yAbs << std::endl;
   // std::cout << "Total moves = " << xAbs + yAbs << std::endl;

   return xAbs + yAbs;
}



int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
      return 1;
   }

   std::ifstream f;
   f.open(argv[1], std::ios::in);
   if (!f.is_open())
   {
      std::cerr << "Error opening " << argv[1] << std::endl;
      return 1;
   }

   std::string directions;
   std::getline(f, directions);

   f.close();

   std::vector<std::string> dirList = split(directions, ',');\
   std::cout << "Split input into " << dirList.size() << " parts" << std::endl;

   int maxMoves = 0;
   int curNumMoves;
   std::vector<std::string> inProgressList;
   for(auto it = dirList.begin(); it != dirList.end(); it++)
   {
      inProgressList.push_back(*it);
      curNumMoves = calculateNumMoves(inProgressList);

      if (curNumMoves > maxMoves)
      {
         maxMoves = curNumMoves;
      }
   }

   std::cout << "Num Moves = " << curNumMoves << std::endl;
   std::cout << "Max Moves = " << maxMoves << std::endl;
   return 0;

}

