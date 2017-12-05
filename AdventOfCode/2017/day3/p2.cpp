#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

/*
As a stress test on the system, the programs here clear the grid and then store the value 1 in square 1. Then, in the same allocation order as shown above, they store the sum of the values in all adjacent squares, including diagonals.

So, the first few squares' values are chosen as follows:

Square 1 starts with the value 1.
Square 2 has only one adjacent filled square (with value 1), so it also stores 1.
Square 3 has both of the above squares as neighbors and stores the sum of their values, 2.
Square 4 has all three of the aforementioned squares as neighbors and stores the sum of their values, 4.
Square 5 only has the first and fourth squares as neighbors, so it gets the value 5.
Once a square is written, its value does not change. Therefore, the first few squares would receive the following values:

147  142  133  122   59
304    5    4    2   57
330   10    1    1   54
351   11   23   25   26
362  747  806--->   ...
What is the first value written that is larger than your puzzle input?
*/

int calcNumber(int x, int y, std::map< std::pair<int, int>, int > const & valueMap)
{
   printf("calcNumber(%d,%d)\n", x, y);

   // Special case at the center point
   if ( (x == 0) && (y == 0) )
   {
      return 1;
   }

   // Create a list of offsets from the current X,Y to try to add up
   std::vector< std::pair<int, int> > offsetList;
   offsetList.push_back(std::make_pair(1,0));
   offsetList.push_back(std::make_pair(1,1));
   offsetList.push_back(std::make_pair(0,1));
   offsetList.push_back(std::make_pair(-1,1));
   offsetList.push_back(std::make_pair(-1,0));
   offsetList.push_back(std::make_pair(-1,-1));
   offsetList.push_back(std::make_pair(0,-1));
   offsetList.push_back(std::make_pair(1,-1));

   int returnVal = 0;

   for(std::vector< std::pair<int, int> >::const_iterator it = offsetList.begin();
       it != offsetList.end();
       it++)
   {
      int testX = x + it->first;
      int testY = y + it->second;

      bool pointExists = valueMap.count(std::make_pair(testX, testY)) > 0;

      if (pointExists)
      {
         int surroundingVal = valueMap.at(std::make_pair(testX, testY));
         returnVal += surroundingVal;
         //printf("  At %d,%d, we found the value %d, sum now is %d\n",
         //       testX, testY, surroundingVal, returnVal);
      }
      else
      {
         //printf("  At %d,%d, we didn't find any value\n", testX, testY);
      }
   }



   return returnVal;
}

char const * directionToString(int dir)
{
   switch(dir % 4)
   {
   case 0:
      return "up";
   case 1:
      return "left";
   case 2:
      return "down";
   case 3:
      return "right";
   }
}

// The length of side of square
int lengthSidePerRing(int ringId)
{
   return ringId * 2 + 1;
}

int qtyPerRing(int ringId)
{
   if (ringId == 0)
   {
      return 1;
   }

   return lengthSidePerRing(ringId) * 4 - 4;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: %s number\n", argv[0]);
    return 1;
  }

  int userNumber = atoi(argv[1]);

  printf("User chose %d\n", userNumber);

  int posX = 0;
  int posY = 0;
  int ringId = 0;
  int curNumber = 1;
  std::map< std::pair<int, int>, int> valueMap;

  int curDirection = 0;

  while(curNumber <= userNumber)
  {
     int qtyThisRing = qtyPerRing(ringId);
     int sideLength = lengthSidePerRing(ringId);

     for(int perRingCounter = 0; ; perRingCounter++)
     {
        // Calculate the current number
        int val = calcNumber(posX, posY, valueMap);
        printf("Point at (%d, %d) is %d\n", posX, posY, val);
        valueMap[std::make_pair(posX, posY)] = val;

        if (val > userNumber)
        {
           printf("We are done!!!\n");
           return 0;
        }

        if (perRingCounter == qtyThisRing - 1)
        {
           // We are at the end of the ring
           posX += 1;
           ringId++;
           curDirection = 0;
           curNumber++;

           printf("End of ring, direction = %s\n", directionToString(curDirection));
           break;
        }
        else
        {
           // Time to move around in the ring

           // Is it time to turn?
           if ( (abs(posX) == (sideLength / 2)) && (abs(posY) == (sideLength / 2)) )
           {
              // It's time to turn!
              curDirection = (curDirection + 1) % 4;
              printf("Changing direction to %s\n", directionToString(curDirection));
           }

           switch(curDirection)
           {
           case 0: // UP
              posY++;
              break;
           case 1: // LEFT
              posX--;
              break;
           case 2: // DOWN
              posY--;
              break;
           case 3: // RIGHT
              posX++;
           }
        }


     }

  }






  return 0;
}
