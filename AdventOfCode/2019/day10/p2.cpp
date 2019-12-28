#include <iostream>
#include <vector>
#include <map>
#include <set>

#include <cmath>

void reduceRatio(int & x, int & y)
{

   int minimumVal = abs(x);
   if (abs(y) < abs(x))
   {
      minimumVal = abs(y);
   }

   if (minimumVal == 0)
   {
      // Special case, horizontal or vertical line
      if (x == 0)
      {
         y /= abs(y);
      }
      else
      {
         x /= abs(x);
      }

      return;

   }

   if (minimumVal <= 1)
   {
      // This is the minimum ratio if one of the numbers is 1
      return;
   }

   for(int i = 2; i <= minimumVal; i++)
   {
      if ( (x % i == 0) && (y % i == 0) )
      {
         x /= i;
         y /= i;
         return reduceRatio(x, y);
      }
   }

}


class AsteroidMap
{
public:
   AsteroidMap(std::vector<std::string> data);

   void printMap();

   bool isAsteroid(int x,int y);

   int numAsteroids();

   void scanSystem();

   std::pair<int, int> printBestLocation(void);

   std::vector<std::pair<int, int> > getVaporizationOrder(std::pair<int, int> baseLoc);



protected:

   void identifyHiddenPlanets(std::pair<int, int> curCoord,
                              std::pair<int, int> otherCoord,
                              std::set<std::pair<int, int> > & hiddenPlanetList);

   std::pair<int, int> nextToVaporize(std::pair<int, int> baseLoc,
                                      std::map<std::pair<int, int>, double> & searchMe,
                                      std::map<std::pair<int, int>, double> & nextLoopList);

   int findDistSquared(std::pair<int, int> const & a, std::pair<int, int> const & b);

   std::map<std::pair<int, int>, int> theMap;

   int theWidth;

   int theHeight;


};

AsteroidMap::AsteroidMap(std::vector<std::string> inputData):
   theWidth(-1),
   theHeight(-1)
{
   theHeight = inputData.size();

   if (theHeight == 0)
   {
      std::cerr << "No rows in the map, fail" << std::endl;
      exit(1);
   }

   theWidth = inputData.at(0).size();

   int y = 0;
   for(auto it = inputData.begin(); it != inputData.end(); it++)
   {
      if (theWidth != it->size())
      {
         std::cerr << "The rows are not all the same width!" << std::endl;
         exit(1);
      }

      for(int x = 0; x < theWidth; x++)
      {
         if ( it->at(x) == '#' )
         {
            // Add this asteroid to our map
            std::pair<int, int> coord = std::make_pair(x, y);
            theMap[coord] = -1;
         }
      }

      y++;
   }





}

void AsteroidMap::printMap()
{
   for(int y = 0; y < theHeight; y++)
   {
      for(int x = 0; x < theWidth; x++)
      {
         if (isAsteroid(x,y))
         {
            std::cout << "#";
         }
         else
         {
            std::cout << ".";
         }
      }

      std::cout << std::endl;
   }
}

bool AsteroidMap::isAsteroid(int x, int y)
{
   std::pair<int, int> coord = std::make_pair(x,y);
   if (theMap.find(coord) == theMap.end())
   {
      return false;
   }
   else
   {
      return true;
   }
}

int AsteroidMap::numAsteroids()
{
   return theMap.size();
}

void AsteroidMap::scanSystem()
{
   for(auto aIt = theMap.begin(); aIt != theMap.end(); aIt++)
   {
      std::pair<int, int> curCoord = aIt->first;
      std::set<std::pair<int, int> > hiddenPlanets;

      std::cout << "Scanning " << curCoord.first << "," << curCoord.second << std::endl;

      for(auto otherIt = theMap.begin(); otherIt != theMap.end(); otherIt++)
      {
         std::pair<int, int> otherCoord = otherIt->first;

         if (otherCoord == curCoord)
         {
            // Dont investigate ourselves
            continue;
         }

         identifyHiddenPlanets(curCoord, otherCoord, hiddenPlanets);

      }

      int numAsteroidsDetected = numAsteroids() - hiddenPlanets.size() - 1;
      std::cout << "The planet @ " << curCoord.first << "," << curCoord.second << " can see " << numAsteroidsDetected << std::endl;

      theMap[curCoord] = numAsteroidsDetected;
   }
}

std::pair<int, int> AsteroidMap::printBestLocation(void)
{
   std::pair<int, int> bestCoord;

   int numDetections = -1;
   for(auto it = theMap.begin(); it != theMap.end(); it++)
   {
      if (numDetections < it->second)
      {
         bestCoord = it->first;
         numDetections = it->second;
      }
   }

   std::cout << "Best location @ " << bestCoord.first << "," << bestCoord.second << " with "
             << numDetections << " detections" << std::endl;

   return bestCoord;

}

void AsteroidMap::identifyHiddenPlanets(std::pair<int, int> curCoord,
                                        std::pair<int, int> otherCoord,
                                        std::set<std::pair<int, int> > & hiddenPlanetList)
{
   int deltaX = otherCoord.first - curCoord.first;
   int deltaY = otherCoord.second - curCoord.second;
   reduceRatio(deltaX, deltaY);

   int xCheck = otherCoord.first;
   int yCheck = otherCoord.second;
   while(true)
   {
      xCheck += deltaX;
      yCheck += deltaY;

      // Is this still inbounds?
      if ( (xCheck < 0) || (xCheck > theWidth) )
      {
         // We are out of bounds
         return;
      }

      if ( (yCheck < 0) || (yCheck > theHeight ) )
      {
         return;
      }

      std::pair<int, int> checkCoord = std::make_pair(xCheck, yCheck);

      if (theMap.find(checkCoord) != theMap.end())
      {
         // We found an asteroid at the location!  It will be hidden by otherCoord
         hiddenPlanetList.emplace(checkCoord);

         std::cout << "Asteroid @ " << xCheck << "," << yCheck
                   << " will be obscured by " << otherCoord.first << ","
                   << otherCoord.second << std::endl;
      }

   }

}

std::vector<std::pair<int, int> > AsteroidMap::getVaporizationOrder(std::pair<int, int> baseLoc)
{
   std::vector<std::pair<int, int> > retVal;

   std::map<std::pair<int, int>, double> angleTable;
   std::map<std::pair<int, int>, double> nextLaserLoop;

   for(auto it = theMap.begin(); it != theMap.end(); it++)
   {
      // Is this current location the same as baseLoc?
      if (baseLoc == it->first)
      {
         // Don't add this one to the list
         continue;
      }

      int deltaY = baseLoc.second - it->first.second;  // non standard cause our axes upside down
      int deltaX = it->first.first - baseLoc.first;

      if (deltaX == 0)
      {
         // Special case!  Slope is infinity
         if (deltaY > 0)
         {
            angleTable[it->first] = 90;
         }
         else
         {
            angleTable[it->first] = 270;
         }

         continue;
      }

      double OA = (double) deltaY / (double) deltaX;
      double angle = atan(OA) / 3.14159 * 180.0; // this will be -90 to 90

      if (deltaX < 0)
         angle += 180.0;

      angleTable[it->first] = angle;
   }

   std::map<std::pair<int, int>, double> remapTable = angleTable;
   for(auto it = angleTable.begin(); it != angleTable.end(); it++)
   {
      double remapAng = -1.0 * (it->second - 90.0);

      if (remapAng < -0.001)
      {
         remapAng += 360.0;
      }

      if (remapAng > 360.001)
      {
         remapAng -= 360.0;
      }

      std::cout << "Remapping " << it->first.first << "," << it->first.second
                << " from " << it->second << " to " << remapAng << std::endl;

      remapTable[it->first] = remapAng;
   }

   // Now we will spin in circles and identify closest asteriod at each angle
   // that needs to be vaporized (hidden ones will be copied to hidden table for
   // the next loop

   while(true)
   {
      while(remapTable.size() > 0)
      {
         retVal.push_back(nextToVaporize(baseLoc, remapTable, nextLaserLoop));
      }

      remapTable = nextLaserLoop;
      nextLaserLoop.clear();

      if (remapTable.size() == 0)
      {
         break;
      }
   }

   return retVal;
}

std::pair<int, int> AsteroidMap::nextToVaporize(std::pair<int, int> baseLoc,
                                   std::map<std::pair<int, int>, double> & searchMe,
                                   std::map<std::pair<int, int>, double> & nextLoopList)
{
   std::vector<std::pair<int, int> > onAngleList;
   double smallestAngleFound = searchMe.begin()->second;

   for(auto it = searchMe.begin(); it != searchMe.end(); it++)
   {
      // Is this coord on angle?
      if (fabs(it->second - smallestAngleFound) < 0.01)
      {
         // This coord on same angle
         onAngleList.push_back(it->first);
         continue;
      }

      // Is this coord at smaller angle?
      if (it->second < smallestAngleFound)
      {
         onAngleList.clear();
         onAngleList.push_back(it->first);
         smallestAngleFound = it->second;
      }

      // This coord at larger angle, do nothing..
   }

   int closestDist = findDistSquared(baseLoc, *(onAngleList.begin()) );
   std::pair<int, int> closestOnAngle = *(onAngleList.begin());

   std::cout << "Smallest angle found: " << smallestAngleFound << std::endl;
   std::cout << "Coords: ";
   for(auto it = onAngleList.begin(); it != onAngleList.end(); it++)
   {
      std::cout << it->first << "," << it->second << "  ";

      int curDist = findDistSquared(baseLoc, *it);
      if (curDist < closestDist)
      {
         // This is the newest closest point!
         closestDist = curDist;
         closestOnAngle = *it;
      }
   }
   std::cout << std::endl;

   std::cout << "Closest on angle @ " << closestOnAngle.first << "," << closestOnAngle.second << std::endl;

   // Search main list for all points on angle
   for(auto it = onAngleList.begin(); it != onAngleList.end(); it++)
   {
      // remove from the search me list (it's eitehr hidden or vaporized)
      auto itToRemove = searchMe.find(*it);

      // Is this the closest point?
      if (*it != closestOnAngle)
      {
          // Add to the list of hidden asteroids
         nextLoopList[itToRemove->first] = itToRemove->second;
      }

      // Remove from the list that are being searched on current laser loop
      searchMe.erase(itToRemove);
   }

   std::cout << "After search, searchMe has " << searchMe.size() << " items in it, and nextLoop has "
             << nextLoopList.size() << " items" << std::endl;

   return closestOnAngle;
}


int AsteroidMap::findDistSquared(std::pair<int, int> const & a, std::pair<int, int> const & b)
{
   int deltaX = abs(a.first - b.first);
   int deltaY = abs(a.second - b.second);

   return deltaX * deltaX + deltaY * deltaY;
}



int main(int, char**)
{
   std::vector<std::string> mapData;

   while(true)
   {
      std::string singleLine;
      std::cin >> singleLine;

      if (std::cin.eof())
      {
         break;
      }

      mapData.push_back(singleLine);
   }

   std::cout << "Number of lines = " << mapData.size() << std::endl;

   AsteroidMap am(mapData);
   am.printMap();

   std::cout << "Number of asteroids = " << am.numAsteroids() << std::endl;

   am.scanSystem();

   std::pair<int, int> baseLoc = am.printBestLocation();

   am.printMap();

   std::vector<std::pair<int, int> > vapeOrder = am.getVaporizationOrder(baseLoc);

   for(int i = 0; i < vapeOrder.size(); i++)
   {
      std::cout << i+1 << ") " << vapeOrder[i].first << "," << vapeOrder[i].second << std::endl;
   }

   return 0;
}
