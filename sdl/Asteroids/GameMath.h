#pragma once

#include <algorithm>
#include <string>
#include "XYPair.h"


class GameMath
{
public:
   static float Distance(XYPair a, XYPair b);

   static XYPair GetUnitVector(float angle);

   static float VectorMagnitude(XYPair v);

   static float VectorAngleDeg(XYPair v);

   static std::string VectorToString(XYPair v);

   static XYPair PositionModulus(XYPair pos, XYPair bounds);

   template <class T>
   static T Clamp(T min, T max, T val)
   {
      return std::max(std::min(val, max), min);
   }
};
