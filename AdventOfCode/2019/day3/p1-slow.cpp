#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

std::vector<std::string> split(std::string data, std::string delimeter)
{
   std::vector<std::string> retVal;
   int pos= 0;
   while(true)
   {
      int delimPos = data.find(delimeter, pos);

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

std::string pathToString(std::vector<PathPoint> path)
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

int distBetweenPathPoint(PathPoint a, PathPoint b)
{
   return abs(a.x - b.x) + abs(a.y - b.y);
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




int main(int argc, char** arg)
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

      //std::cout << pathToString(wire1) << std::endl;
      //std::cout << pathToString(wire2) << std::endl;

      bool foundFirstPointer = false;
      int minDist;
      PathPoint center = { 0, 0 };
      for(auto pt1 = wire1.begin(); pt1 != wire1.end(); pt1++)
      {
         for(auto pt2 = wire2.begin(); pt2 != wire2.end(); pt2++)
         {
            if ( (pt1->x == pt2->x) && (pt1->y == pt2->y) )
            {
               // Found an intersection point
               std::cout << "Intersection @ " << pathPointToString(*pt1);

               int dist = distBetweenPathPoint(center, *pt1);

               if (dist == 0)
               {
                  std::cout << ", but it's the center" << std::endl;
                  continue;
               }

               if (!foundFirstPointer)
               {
                  std::cout << ", and it's first intersection found. dist = " << dist << std::endl;
                  foundFirstPointer = true;
                  minDist = dist;
               }
               else
               {
                  if (dist > minDist)
                  {
                     std::cout << ", dist " << dist << " farther than best answer "
                               << minDist << std::endl;
                  }
                  else
                  {
                     std::cout << ", new best dist of " << dist << std::endl;
                     minDist = dist;
                  }
               }
            }
         }
      }




   }





	return 0;
}
