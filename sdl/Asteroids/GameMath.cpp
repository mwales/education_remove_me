#include "GameMath.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Logger.h"

XYPair GameMath::GetUnitVector(float angle)
{
   XYPair retVal;
   retVal[0] = cos(angle);
   retVal[1] = sin(angle);
   return retVal;
}

float GameMath::VectorMagnitude(XYPair v)
{
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

std::string GameMath::VectorToString(XYPair v)
{
   std::ostringstream oss;
   oss << v;
   return oss.str();
}

float Distance(XYPair a, XYPair b)
{
   float deltaX = (a[0] - b[0]);
   deltaX *= deltaX;

   float deltaY = (a[1] - b[1]);
   deltaY *= deltaY;

   return sqrt(deltaX + deltaY);
}
