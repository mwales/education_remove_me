#include "GameMath.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Logger.h"


XYPair GameMath::GetUnitVector(float angle)
{
   XYPair retVal(2);
   retVal[0] = cos(angle);
   retVal[1] = sin(angle);
   return retVal;
}

float GameMath::VectorMagnitude(XYPair v)
{
   if (v.size() != 2)
   {
      LOG_FATAL() << "GameVector provided is not 2 dimenstional";
   }

   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

std::string GameMath::VectorToString(XYPair v)
{
   std::ostringstream oss;
   oss << v;
   return oss.str();
}
