#include <stdio.h>
#include <stdlib.h>

// Failed first attempt at this problem

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: %s number\n", argv[0]);
    return 1;
  }

  int userNumber = atoi(argv[1]);

  printf("User chose %d\n", userNumber);

  int ringNumber = 0;
  int ringStart = 1;
  int ringSideLen = 1;
  
  int lastRingStart = 0;
  int lastRingSideLen = 0;

  while(userNumber >= ringStart)
  {
    printf("Ring = %d, side len = %d, start = %d\n", ringNumber, ringSideLen, ringStart);
    
    lastRingStart = ringStart;
    lastRingSideLen = ringSideLen;

    int numElementsInRing = ringSideLen * 4 -4;
    printf("Num elements in ring = %d\n", numElementsInRing);

    ringStart += numElementsInRing + 1;
    ringSideLen += 2;
    ringNumber++;
  }

  printf("Last Ring Start = %d\n", lastRingStart);
  printf("Ring Len = %d\n", lastRingSideLen);

  // Distance caused by the ring level
  int ringDist = lastRingSideLen / 2;

  // Determine the distance  on the ring away from center
  int ringCenterPt = lastRingStart + (lastRingSideLen / 2 - 1);

  printf("ringDist = %d\n", ringDist);
  printf("ring Center point = %d\n", ringCenterPt);

  // Determine which side the point is on?

  // Determine the number in the corners
  int topRightCorner = lastRingStart + lastRingSideLen / 2;
  printf("Top right corner number = %d\n", topRightCorner);

  int topLeftCorner = topRightCorner + lastRingSideLen - 1;
  printf("Top left corner number = %d\n", topLeftCorner);

//  int bottomLeftCorner = topLeftCorner + lastRingSideLen - 1;
//  printf("Bottom left corner number = %d\n", bottomLeftCorner);

//  int topLeftCorner = topRightCorner + lastRingSideLen - 1;
//  printf("Top left corner number = %d\n", topLeftCorner);

  int xDist = 0;
  int yDist = 0;

  // Is the side on top
  if ( (userNumber >= topRightCorner) && (userNumber <= topRightCorner + lastRingSideLen))
  {
     printf("Number is on the top of the square\n");

     // Y distance is automatically the maximum
     yDist = lastRingSideLen / 2;
     xDist = abs( (userNumber - topRightCorner) % lastRingSideLen - lastRingSideLen / 2 );

  }


  printf("xDist = %d, yDist = %d, total dist = %d\n", xDist, yDist, xDist + yDist);


  


  return 0;
}
