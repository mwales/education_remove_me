#include "GameMath.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Logger.h"

XYPair GameMath::GetUnitVector(float angle)
{
   angle = angle * M_PI / 180.0;
   XYPair retVal;
   retVal[0] = cos(angle);
   retVal[1] = sin(angle);
   return retVal;
}

float GameMath::VectorMagnitude(XYPair v)
{
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float GameMath::VectorAngleDeg(XYPair v)
{
   float angRad = atan(v[1] / v[0]);
   float angDeg = angRad / M_PI * 180.0;

   if (v[0] < 0)
   {
      angDeg += 180.0;
   }
   else if (angDeg < 0)
   {
      angDeg += 360.0;
   }
   return angDeg;
}

std::string GameMath::VectorToString(XYPair v)
{
   std::ostringstream oss;
   oss << v;
   return oss.str();
}

float GameMath::Distance(XYPair a, XYPair b)
{
   float deltaX = (a[0] - b[0]);
   deltaX *= deltaX;

   float deltaY = (a[1] - b[1]);
   deltaY *= deltaY;

   return sqrt(deltaX + deltaY);
}

XYPair GameMath::PositionModulus(XYPair pos, XYPair bounds)
{
   if (pos[0] < 0)
   {
      pos[0] += bounds[0];
   }
   if (pos[1] < 0)
   {
      pos[1] += bounds[1];
   }

   if (pos[0] > bounds[0])
   {
      pos[0] = fmod(pos[0], bounds[0]);
   }
   if (pos[1] > bounds[1])
   {
      pos[1] = fmod(pos[1], bounds[1]);
   }

   return pos;
}
