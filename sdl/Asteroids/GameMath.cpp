#include "GameMath.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Logger.h"


XYPair::XYPair():
   boost::numeric::ublas::vector<float>(2)
{
   // Intentionally empty
}

XYPair::XYPair(XYPair const & rhs)
{
   // Copy constructor
   (*this) = rhs;
}

XYPair::XYPair(float x, float y):
   boost::numeric::ublas::vector<float>(2)
{
   (*this)[0] = x;
   (*this)[1] = y;
}

XYPair GameMath::GetUnitVector(float angle)
{
   XYPair retVal;
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
