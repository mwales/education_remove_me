#include "Maze.h"
#include <assert.h>

#define MAZE_DEBUG
#ifdef MAZE_DEBUG
   #define MAZEDEBUG std::cerr
#else
   #define MAZEDEBUG if(0) std::cerr
#endif

Maze::Maze(std::vector<std::string> data)
{
   theMazeData = data;

   theHeight = data.size();

   theWidth = data.front().size();

   MAZEDEBUG << "Maze is " << theWidth << "x" << theHeight << std::endl;

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

   MAZEDEBUG << "Number of keys is " << theNumKeys << std::endl;

   computeAllKeyDistances();
   computePathsToAllLocations();
   computeAllKeyDependencies();
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
            MAZEDEBUG << "Maze::findStartPoint() @ " << retValX << ","
                      << retValY << std::endl;
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

   char retVal = theMazeData.at(y).at(x);
   MAZEDEBUG << "Maze::getPoint(" << x << "," << y << ") = " << retVal << std::endl;
   return theMazeData.at(y).at(x);
}

std::vector<char> Maze::getPossibleKeys()
{
   std::vector<char> retVal;

   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         char curPoint = getPoint(x,y);

         if ( (curPoint >= 'a') && (curPoint <= 'z') )
         {
            retVal.push_back(curPoint);
         }
      }
   }

   return retVal;
}

std::set<char> Maze::getPossibleKeys2()
{
   std::set<char> retVal;
   std::vector<char> keyList = getPossibleKeys();

   for(auto it = keyList.begin(); it != keyList.end(); it++)
   {
      retVal.insert(*it);
   }
   return retVal;
}

std::set<Coord> Maze::getNeighbors(int x, int y)
{
   MAZEDEBUG << "Trace: getNeighbors(" << x << "," << y << ")" << std::endl;
   std::vector<Coord> possiblePoints;
   std::set<Coord> retVal;

   possiblePoints.push_back(std::make_pair(x, y+1));
   possiblePoints.push_back(std::make_pair(x, y-1));
   possiblePoints.push_back(std::make_pair(x+1, y));
   possiblePoints.push_back(std::make_pair(x-1, y));

   for(auto it = possiblePoints.begin(); it != possiblePoints.end(); it++)
   {
      if (!isPointOnMap(it->first, it->second))
      {
         continue;
      }
      retVal.insert(*it);
   }

   return retVal;
}

Coord Maze::getLocationOfKey(char key)
{
   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         char curPoint = getPoint(x,y);

         if (curPoint == key)
         {
            MAZEDEBUG << "Trace: getLocationOfKey(" << key << ") = " << x << ","
                      << y << std::endl;

            return std::make_pair(x, y);
         }
      }
   }

   // We didn't find the key
   MAZEDEBUG << "Trace: getLocationOfKey(" << key << " = FAILED" << std::endl;
   return std::make_pair(-1,-1);
}

void Maze::computeAllKeyDistances()
{
   std::vector<char> allKeys = getPossibleKeys();
   for(auto it = allKeys.begin(); it != allKeys.end(); it++)
   {
      // Don't even care about the return value, just let it fill internal cache
      getKeyDistances(*it);
   }

   getKeyDistances('@');
}

std::map<char, int> Maze::getKeyDistances(char key)
{
   // Have we already computed these key distances?
   if (theKeyDistances.find(key) != theKeyDistances.end())
   {
      // Return the data already computed
      return theKeyDistances[key];
   }

   Coord location = getLocationOfKey(key);
   std::map<char,int> retVal;
   if (location.first == -1)
   {
      // Return empty map?
      return retVal;
   }

   breadthFirstFill(location.first, location.second, retVal);

   // Cache the key distances for future lookup
   theKeyDistances[key] = retVal;

   MAZEDEBUG << "For key " << key << ", distances: ";
   for(auto it = retVal.begin(); it != retVal.end(); it++)
   {
      MAZEDEBUG << it->first << "=" << it->second << ", ";
   }
   MAZEDEBUG << std::endl;

   return retVal;
}

void Maze::breadthFirstFill(int x, int y, std::map<char, int> & keyDist)
{
   std::set<Coord> pointsVisited;
   pointsVisited.insert(std::make_pair(x, y));

   std::set<Coord> pointsToVisit = getNeighbors(x, y);

   int distance = 0;

   while(!pointsToVisit.empty())
   {
      distance++;

      MAZEDEBUG << "At distance " << distance << " from " << x << "," << y
                << ": ";
      for(auto it = pointsToVisit.begin(); it != pointsToVisit.end(); it++)
      {
         MAZEDEBUG << "(" << it->first << "," << it->second << "), ";
      }
      MAZEDEBUG << std::endl;

      std::set<Coord> pointsSameDist;
      pointsSameDist = pointsToVisit;
      pointsToVisit.clear();

      for(auto it = pointsSameDist.begin(); it != pointsSameDist.end(); it++)
      {
         pointsVisited.insert(*it);
         char data = getPoint(it->first, it->second);

         // Is it a key?
         if ( (data >= 'a') && (data <= 'z') )
         {
            MAZEDEBUG << "Dist from " << x << "," << y << " to " << data
                      << " = " << distance << std::endl;
            keyDist[data] = distance;
         }

         // Now add neighbors if not a wall
         if (data == '#')
         {
            continue;
         }

         std::set<Coord> neighs = getNeighbors(it->first, it->second);
         for(auto nIt = neighs.begin(); nIt != neighs.end(); nIt++)
         {
            if ( (pointsVisited.find(*nIt) == pointsVisited.end()) &&
                 (pointsToVisit.find(*nIt) == pointsToVisit.end()) )
            {
               // We have not visited this neighbor
               pointsToVisit.insert(*nIt);
            }
         }
      }
   }
}

void Maze::computePathsToAllLocations()
{
   MAZEDEBUG << "Trace: computePathsToAllLocations()" << std::endl;
   std::set<Coord > pointsVisited;
   int startX = 0;
   int startY = 0;
   findStartPoint(startX, startY);
   Coord startPoint = std::make_pair(startX, startY);

   pointsVisited.insert(startPoint);

   std::vector<std::pair<int, int> > curPath;
   thePathToEachPoint[startPoint] = curPath; // empty path for the start point

   std::set<Coord> pointsToVisit = mapNeighbors(startPoint);

   int distance = 0;

   while(!pointsToVisit.empty())
   {
      distance++;

      MAZEDEBUG << "Distance to all of the following points is "
                << distance << std::endl;

      std::set<Coord> pointsSameDist;
      pointsSameDist = pointsToVisit;
      pointsToVisit.clear();

      MAZEDEBUG << "Points to visit on this round = " << pointsSameDist.size() << std::endl;

      for(auto it = pointsSameDist.begin(); it != pointsSameDist.end(); it++)
      {
         MAZEDEBUG << "Same dist loop @ point " << it->first << "," << it->second
                   << std::endl;

         char data = getPoint(it->first, it->second);

         // Now add neighbors if not a wall
         if (data == '#')
         {
            continue;
         }

         std::set<Coord > newneighs = mapNeighbors(*it);
         for(auto nIt = newneighs.begin(); nIt != newneighs.end(); nIt++)
         {
            if ( (pointsVisited.find(*nIt) == pointsVisited.end()) &&
                 (pointsToVisit.find(*nIt) == pointsToVisit.end()) )
            {
               // We have not visited this neighbor
               MAZEDEBUG << "Add " << nIt->first << "," << nIt->second
                         << " to our list of points to visit at next distance"
                         << std::endl;
               pointsToVisit.insert(*nIt);
            }
         }

         pointsVisited.insert(*it);
      }
   }

   MAZEDEBUG << "Trace: computePathsToAllLocations() complete" << std::endl;
}

std::set<Coord> Maze::mapNeighbors(Coord coord)
{
   MAZEDEBUG << "Maze::mapNeighbors(" << coord.first << "," << coord.second
             << ")" << std::endl;

   std::set<Coord > retVal;

   Path curPath = getPath(coord.first, coord.second);


   Coord neighborCoord;

   // North
   neighborCoord = std::make_pair(coord.first, coord.second+1);
   if (isPointOnMap(neighborCoord.first, neighborCoord.second))
   {
      retVal.insert(neighborCoord);
      Path neighPath = curPath;
      neighPath.push_back(neighborCoord);
      thePathToEachPoint[neighborCoord] = neighPath;
   }

   // South
   neighborCoord = std::make_pair(coord.first, coord.second-1);
   if (isPointOnMap(neighborCoord.first, neighborCoord.second))
   {
      retVal.insert(neighborCoord);
      Path neighPath = curPath;
      neighPath.push_back(neighborCoord);
      thePathToEachPoint[neighborCoord] = neighPath;
   }

   // East
   neighborCoord = std::make_pair(coord.first+1, coord.second);
   if (isPointOnMap(neighborCoord.first, neighborCoord.second))
   {
      retVal.insert(neighborCoord);
      Path neighPath = curPath;
      neighPath.push_back(neighborCoord);
      thePathToEachPoint[neighborCoord] = neighPath;
   }

   // West
   neighborCoord = std::make_pair(coord.first-1, coord.second);
   if (isPointOnMap(neighborCoord.first, neighborCoord.second))
   {
      retVal.insert(neighborCoord);
      Path neighPath = curPath;
      neighPath.push_back(neighborCoord);
      thePathToEachPoint[neighborCoord] = neighPath;
   }

   return retVal;
}

bool Maze::isPointOnMap(int x, int y)
{
   if ( (x < 0) || (x >= theWidth) )
   {
      return false;
   }

   if ( (y < 0) || (y >= theHeight) )
   {
      return false;
   }

   return true;
}

Path Maze::getPath(int x, int y)
{
   MAZEDEBUG << "getPath(" << x << "," << y << ")" << std::endl;
   Coord coord = std::make_pair(x, y);
   if (thePathToEachPoint.find(coord) == thePathToEachPoint.end())
   {
      std::cerr << "Asked to get the path to a point that has no path @ " << x
                << "," << y << std::endl;
      assert(false);
      exit(1);
   }

   return thePathToEachPoint[coord];
}

void Maze::computeAllKeyDependencies()
{
   std::vector<char> allKeys = getPossibleKeys();

   for(auto curKeyIt = allKeys.begin(); curKeyIt != allKeys.end(); curKeyIt++)
   {
      // Follow the path to each key and see what doors need to be traversed
      Coord curKeyCoord = getLocationOfKey(*curKeyIt);

      Path keyPath = getPath(curKeyCoord.first, curKeyCoord.second);
      std::set<char> keysReqd;
      std::set<char> keysFound;

      for(auto it = keyPath.begin(); it != keyPath.end(); it++)
      {
         char curPoint = getPoint(it->first, it->second);

         if ( (curPoint >= 'A') && (curPoint <= 'Z') )
         {
            // We found a door on the way to the key
            char keyRequired = curPoint - 'A' + 'a';
            keysReqd.insert(keyRequired);
         }

         if ( (curPoint >= 'a') && (curPoint <= 'z') )
         {
            // We found a door on the way to the key
            keysFound.insert(curPoint);
         }
      }

      theKeyDependencies[*curKeyIt] = keysReqd;
      theKeysAutomatic[*curKeyIt] = keysFound;

      std::cout << "Key " << *curKeyIt << " requires keys (" << charListToString(keysReqd)
                << "), automatic keys (" << charListToString(keysFound) << ")" << std::endl;
   }
}

std::string Maze::charListToString(std::set<char> const & data)
{
   bool first = true;
   std::string retVal;
   for(auto it = data.begin(); it != data.end(); it++)
   {
      if (first)
      {
         first = false;
      }
      else
      {
         retVal += ",";
      }

      retVal += *it;
   }

   return retVal;
}

std::string Maze::charListToString(std::vector<char> const & data)
{
   bool first = true;
   std::string retVal;
   for(auto it = data.begin(); it != data.end(); it++)
   {
      if (first)
      {
         first = false;
      }
      else
      {
         retVal += ",";
      }

      retVal += *it;
   }

   return retVal;
}

void Maze::printState()
{
   MAZEDEBUG << "The Maze = " << theWidth << "x" << theHeight << " with "
             << theNumKeys << " keys" << std::endl;

   for(auto it = theMazeData.begin(); it != theMazeData.end(); it++)
   {
      MAZEDEBUG << *it << std::endl;
   }

   MAZEDEBUG << "Key Distances: " << theKeyDistances.size() << std::endl;
   for(auto sIt = theKeyDistances.begin(); sIt != theKeyDistances.end(); sIt++)
   {
      MAZEDEBUG << "The following are the key distnaces for " << sIt->first << std::endl;

      for(auto kIt = sIt->second.begin(); kIt != sIt->second.end(); kIt++)
      {
         MAZEDEBUG << sIt->first << " to " << kIt->first << " = " << kIt->second
                   << std::endl;
      }

      MAZEDEBUG << "   Reqs: " << charListToString(theKeyDependencies[sIt->first])
            << "  Auto: " << charListToString(theKeysAutomatic[sIt->first])
            << std::endl;


   }


}

bool Maze::canIGetKey(char key, std::vector<char> keysVec)
{
   std::set<char> keysPossessed;
   for(auto it = keysVec.begin(); it != keysVec.end(); it++)
   {
      keysPossessed.insert(*it);
   }

   std::set<char> keysReqd = theKeyDependencies.at(key);

   for(auto it = keysReqd.begin(); it != keysReqd.end(); it++)
   {
      // Make sure they have each key they require
      if (keysPossessed.find(*it) == keysPossessed.end())
      {
         return false;
      }
   }

   // We had all the reqd keys
   return true;
}
