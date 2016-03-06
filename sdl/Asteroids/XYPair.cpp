#include <ostream>

#include "XYPair.h"
#include "Logger.h"


XYPair::XYPair():
   theX(0.0),
   theY(0.0)
{
   // Intentionally empty
}

XYPair::XYPair(XYPair const & rhs):
   theX(rhs.theX),
   theY(rhs.theY)
{
   // Intentionally empty
}

XYPair::XYPair(float x, float y):
   theX(x),
   theY(y)
{
   // Intentionally empty
}

XYPair & XYPair::operator=(XYPair const & rhs)
{
   if (this != &rhs)
   {
      theX = rhs.theX;
      theY = rhs.theY;
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
   theX += rhs.theX;
   theY += rhs.theY;
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
   theX -= rhs.theX;
   theY -= rhs.theY;
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
   theX *= rhs;
   theY *= rhs;
   return *this;
}

float& XYPair::operator[](std::size_t idx)
{
   if (idx > 1)
   {
      LOG_FATAL() << "XYPair index" << idx << "out of bounds";
      return theX;
   }

   if (idx == 0)
   {
      return theX;
   }
   else
   {
      return theY;
   }
}

float const & XYPair::operator[](std::size_t idx) const
{
   if (idx > 1)
   {
      LOG_FATAL() << "XYPair index" << idx << "out of bounds";
      return theX;
   }

   if (idx == 0)
   {
      return theX;
   }
   else
   {
      return theY;
   }
}

std::ostream& operator<<(std::ostream& os, const XYPair& obj)
{
  os << "(" << obj[0] << "," << obj[1] << ")";
  return os;
}
