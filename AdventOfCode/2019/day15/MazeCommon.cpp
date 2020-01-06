#include "MazeCommon.h"
#include <iostream>


Coord::Coord(int x, int y)
{
   first = x;
   second = y;
}

Coord::Coord()
{
   first = 0;
   second = 0;
}

Coord::Coord(Coord const & x)
{
   first = x.first;
   second = x.second;
}

std::string Coord::toString() const
{
   std::string retVal;
   retVal += "(";
   retVal += std::to_string(x());
   retVal += ",";
   retVal += std::to_string(y());
   retVal += ")";
   return retVal;
}

bool operator==(Coord const & lhs, Coord const & rhs)
{
   // std::cerr << "operator== for coord.  LHS=" << lhs.toString()
   //           << ", RHS=" << rhs.toString() << std::endl;

   return ( (lhs.x() == rhs.x()) && (lhs.y() == rhs.y()) );
}

bool operator!=(Coord const & lhs, Coord const & rhs)
{
   return !(lhs == rhs);
}

std::string directionToString(int dir)
{
   switch(dir)
   {
   case MAZE_NORTH:
      return "North";
   case MAZE_EAST:
      return "East";
   case MAZE_SOUTH:
      return "South";
   case MAZE_WEST:
      return "West";
   default:
      return "Invalid";
   }
}

std::string mazeValueToString(int val)
{
   switch(val)
   {
   case MAZE_WALL:
      return "Wall";
   case MAZE_EMPTY:
      return "Empty";
   case MAZE_GOAL:
      return "Goal";
   default:
      return "InvalidMazeResult";
   }
}
