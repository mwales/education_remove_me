#pragma once

#include <string>
#include "XYPair.h"

class GameMath
{
public:
   float Distance(XYPair a, XYPair b);

   static XYPair GetUnitVector(float angle);

   static float VectorMagnitude(XYPair v);

   static std::string VectorToString(XYPair v);
};
