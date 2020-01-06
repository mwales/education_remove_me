#ifndef MAZE_LOCATION_H
#define MAZE_LOCATION_H

#include<vector>
#include"MazeCommon.h"

class MazeLocation
{
public:

   MazeLocation(Coord const & loc);

   MazeLocation createNeighbor(int direction);

   void setContent(int mazeContent);

   Coord getLocation() const;

   int getNumSteps() const;

   std::vector<int> getPath() const;

   int getContent() const;

   std::string toString();


protected:

   Coord theLoc;

   int theContent; // wall, empty, goal

   std::vector<int> theShortestPath;
};

bool operator==(MazeLocation const & lhs, MazeLocation const & rhs);

bool operator!=(MazeLocation const & lhs, MazeLocation const & rhs);

#endif // MAZE_LOCATION_H
