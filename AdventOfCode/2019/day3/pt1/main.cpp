#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <QSet>



std::vector<std::string> split(std::string data, std::string delimeter)
{
   std::vector<std::string> retVal;
   std::string::size_type pos= 0;
   while(true)
   {
      std::string::size_type delimPos = data.find(delimeter, pos);

      if (delimPos == std::string::npos)
      {
         // Add the last token to the list and return
         // std::cerr << "Token = " << data.substr(pos) << std::endl << std::endl;
         retVal.push_back(data.substr(pos));
         return retVal;
      }
      else
      {
         // We found a token and delimter
         // std::cerr << "Token = " << data.substr(pos, delimPos - pos) << std::endl;
         retVal.push_back(data.substr(pos, delimPos - pos));
         pos = delimPos + 1;
      }
   }

}

typedef struct path_point
{
   int x;
   int y;
} PathPoint;

std::string pathPointToString(PathPoint p)
{
   std::string retVal;
   retVal = "(";
   retVal += std::to_string(p.x);
   retVal += ",";
   retVal += std::to_string(p.y);
   retVal += ")";
   return retVal;
}

// Look at this, my first C++14 code, feels like some dirty python code
std::string pathToString(auto path)
{
   std::string retVal;
   bool isFirst = true;
   for(auto curLoc = path.begin(); curLoc != path.end(); curLoc++)
   {
      if (isFirst)
      {
         isFirst = false;
      }
      else
      {
         retVal += ", ";
      }

      retVal += pathPointToString(*curLoc);
   }

   return retVal;
}


int qHash(PathPoint const & pp)
{
   return (pp.x << 16) + pp.y;
}

bool operator==(PathPoint const & lhs, PathPoint const & rhs)
{
   return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

int distBetweenPathPoint(PathPoint a, PathPoint b)
{
   return abs(a.x - b.x) + abs(a.y - b.y);
}

int wireLength(std::vector<PathPoint> const & wire, PathPoint node)
{
   int retVal = 1;
   for(auto ppIt = wire.begin(); ppIt != wire.end(); ppIt++)
   {
      if (*ppIt == node)
      {
         return retVal;
      }

      retVal++;
   }

   return retVal;
}

std::vector<PathPoint> createPathPointList(std::vector<std::string> const & data)
{
   // std::cout << "Creating PathPoint list" << std::endl;

   std::vector<PathPoint> retVal;

   PathPoint curPoint = { 0, 0};

   for(auto pathIter = data.begin(); pathIter != data.end(); pathIter++)
   {
      PathPoint direction = { 0, 0};
      std::string curPath = *pathIter;

      if (curPath[0] == 'U')
      {
         direction.y = 1;
      }
      else if (curPath[0] == 'D')
      {
         direction.y = -1;
      }
      else if (curPath[0] == 'L')
      {
         direction.x = -1;
      }
      else if (curPath[0] == 'R')
      {
         direction.x = 1;
      }
      else
      {
         std::cerr << "Invalid direction : " << curPath << std::endl;
      }

      // Find the magnitude
      curPath = curPath.substr(1);
      int magnitude = atoi(curPath.c_str());

      for(int i = 0; i < magnitude; i++)
      {
         // Add the path points
         curPoint.x += direction.x;
         curPoint.y += direction.y;
         retVal.push_back(curPoint);
      }
   }

   return retVal;
}

QSet<PathPoint> convertPathPointList(std::vector<PathPoint> const & data)
{
   QSet<PathPoint> retVal;
   for(auto ppIt = data.begin(); ppIt != data.end(); ppIt++)
   {
      retVal.insert(*ppIt);
   }
   return retVal;
}

int main(int, char**)
{
   std::string inputData;
   while(true)
   {
      std::cin >> inputData;

      if (std::cin.eof())
      {
         break;
      }

      std::vector<std::string> path1 = split(inputData, ",");

      std::cin >> inputData;

      if (std::cin.eof())
      {
         break;
      }

      std::vector<std::string> path2 = split(inputData, ",");

      std::vector<PathPoint> wire1 = createPathPointList(path1);
      std::vector<PathPoint> wire2 = createPathPointList(path2);

      QSet<PathPoint> intersections = convertPathPointList(wire1);
      intersections.intersect(convertPathPointList(wire2));

      // std::cout << pathToString(wire1) << std::endl;
      // std::cout << pathToString(wire2) << std::endl;

      bool foundFirstPointer = false;
      int minDistPt1;
      int minDistPt2;
      PathPoint center = { 0, 0 };
      for(auto pt1 = intersections.begin(); pt1 != intersections.end(); pt1++)
      {

         // Found an intersection point
         std::cout << "Intersection @ " << pathPointToString(*pt1);

         int dist1 = distBetweenPathPoint(center, *pt1);
         int dist2 = wireLength(wire1, *pt1) + wireLength(wire2, *pt1);


         if (dist1 == 0)
         {
            std::cout << ", but it's the center" << std::endl;
            continue;
         }

         if (!foundFirstPointer)
         {
            std::cout << ", and it's first intersection found. dist = " << dist1 << std::endl;
            foundFirstPointer = true;
            minDistPt1 = dist1;
            minDistPt2 = dist2;
         }
         else
         {
            if (dist1 > minDistPt1)
            {
               std::cout << ", dist1 " << dist1 << " farther than best answer "
                         << minDistPt1 << std::endl;
            }
            else
            {
               std::cout << ", new best dist of " << dist1 << std::endl;
               minDistPt1 = dist1;
            }

            if (dist2 > minDistPt2)
            {
               std::cout << "  dist2 " << dist2 << " farther than best answer "
                         << minDistPt2 << std::endl;
            }
            else
            {
               std::cout << "  new best dist2 of " << dist2 << std::endl;
               minDistPt2 = dist2;
            }
         }

      }

      std::cout << "Best Distance = " << minDistPt1 << std::endl;
      std::cout << "Best Wire Length = " << minDistPt2 << std::endl;

   }

   return 0;
}





