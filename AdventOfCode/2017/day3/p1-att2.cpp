#include <stdio.h>
#include <stdlib.h>


/*
Each square on the grid is allocated in a spiral pattern starting at a location marked 1 and then
counting up while spiraling outward. For example, the first few squares are allocated like this:

17  16  15  14  13
18   5   4   3  12
19   6   1   2  11
20   7   8   9  10
21  22  23---> ...

While this is very space-efficient (no squares are skipped), requested data must be carried back to
square 1 (the location of the only access port for this memory system) by programs that can only
move up, down, left, or right. They always take the shortest path: the Manhattan Distance between
the location of the data and square 1.

For example:

Data from square 1 is carried 0 steps, since it's at the access port.
Data from square 12 is carried 3 steps, such as: down, left, left.
Data from square 23 is carried only 2 steps: up twice.
Data from square 1024 must be carried 31 steps.

How many steps are required to carry the data from the square identified in your puzzle input all
the way to the access port?
*/

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

  int curDirection = 0;

  while(curNumber <= userNumber)
  {
     int qtyThisRing = qtyPerRing(ringId);
     int sideLength = lengthSidePerRing(ringId);

     for(int perRingCounter = 0; ; perRingCounter++)
     {
        printf("Number %d is at %d, %d (perRing = %d of %d)\n",
               curNumber, posX, posY, perRingCounter, qtyThisRing);

        if (curNumber == userNumber)
        {
           printf("We have arrived.  Dist = %d\n", abs(posX) + abs(posY));
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

        curNumber++;
     }

  }






  return 0;
}
