#ifndef MAZE_COMMON_H
#define MAZE_COMMON_H

#include <iostream>


#define MAZE_NORTH 1
#define MAZE_SOUTH 2
#define MAZE_WEST  3
#define MAZE_EAST  4

#define MAZE_WALL  0
#define MAZE_EMPTY 1
#define MAZE_GOAL  2

class Coord : public std::pair<int, int>
{
public:
   Coord();
   Coord(int x, int y);

   Coord(Coord const & x);

   std::string toString() const;

   int x() const { return first; }
   int y() const { return second; }

   void setX(int x) { first = x; }
   void setY(int y) { second = y; }

};

bool operator==(Coord const & lhs, Coord const & rhs);

bool operator!=(Coord const & lhs, Coord const & rhs);

std::string directionToString(int dir);

std::string mazeValueToString(int val);

#endif
