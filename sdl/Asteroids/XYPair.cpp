#include <ostream>

#include "XYPair.h"
#include "Logger.h"


XYPair::XYPair():
   _x(0.0),
   _y(0.0)
{
   // Intentionally empty
}

XYPair::XYPair(XYPair const & rhs):
   _x(rhs._x),
   _y(rhs._y)
{
   // Intentionally empty
}

XYPair::XYPair(float x, float y):
   _x(x),
   _y(y)
{
   // Intentionally empty
}

XYPair & XYPair::operator=(XYPair const & rhs)
{
   if (this != &rhs)
   {
      _x = rhs._x;
      _y = rhs._y;
   }
   return *this;
}

XYPair XYPair::operator+(XYPair const & rhs) const
{
   XYPair result(*this);
   result += rhs;
   return result;
}

XYPair & XYPair::operator+=(XYPair const & rhs)
{
   _x += rhs._x;
   _y += rhs._y;
   return *this;
}

XYPair XYPair::operator-(XYPair const & rhs) const
{
   XYPair result(*this);
   result -= rhs;
   return result;
}

XYPair & XYPair::operator-=(XYPair const & rhs)
{
   _x -= rhs._x;
   _y -= rhs._y;
   return *this;
}

XYPair XYPair::operator*(float const & rhs) const
{
   XYPair result(*this);
   result *= rhs;
   return result;
}

XYPair & XYPair::operator*=(float const & rhs)
{
   _x *= rhs;
   _y *= rhs;
   return *this;
}

float& XYPair::operator[](std::size_t idx)
{
   if (idx > 1)
   {
      LOG_FATAL() << "XYPair index" << idx << "out of bounds";
      return _x;
   }

   if (idx == 0)
   {
      return _x;
   }
   else
   {
      return _y;
   }
}

float const & XYPair::operator[](std::size_t idx) const
{
   if (idx > 1)
   {
      LOG_FATAL() << "XYPair index" << idx << "out of bounds";
      return _x;
   }

   if (idx == 0)
   {
      return _x;
   }
   else
   {
      return _y;
   }
}

std::ostream& operator<<(std::ostream& os, const XYPair& obj)
{
  os << "(" << obj[0] << "," << obj[1] << ")";
  return os;
}
