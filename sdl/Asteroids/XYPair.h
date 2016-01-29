#pragma once

class XYPair
{
public:
   XYPair();

   XYPair(XYPair const & rhs);

   XYPair(float x, float y);

   XYPair & operator=(XYPair const & rhs);

   XYPair operator+(XYPair const & rhs) const;

   XYPair & operator+=(XYPair const & rhs);

   XYPair operator-(XYPair const & rhs) const;

   XYPair & operator-=(XYPair const & rhs);

   XYPair operator*(float const & rhs) const;

   XYPair & operator*=(float const & rhs);

   float& operator[](std::size_t idx);

   float const & operator[](std::size_t idx) const;

protected:

   float _x;

   float _y;
};

std::ostream& operator<<(std::ostream& os, const XYPair& obj);

