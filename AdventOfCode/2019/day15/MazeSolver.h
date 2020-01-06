#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <stdint.h>
#include "ElfComputer.h"
#include "MazeLocation.h"
#include "MazeViewer.h"

class MazeSolver
{
public:

   MazeSolver(ElfCode pd);

   std::vector<int> findPathToExit(bool stopAtGoal = true);

   int longestPath();

   void printMap();

   void getMapLimits(Coord & topLeft, Coord & bottomRight);

   void reset();

protected:

   int move(int direction);

   void backtrack(Coord loc);

   Coord findCommonBackTrackPoint(MazeLocation a, Coord b, Coord startPoint);

   bool isAlreadyVisited(MazeLocation ml);

   MazeLocation getVisitedLocation(Coord c);

   void visitUnvisitedNeighbors(MazeLocation ml);

   std::vector<MazeLocation> theLocationsVisited;

   std::vector<MazeLocation> theLocationsToVisit;

   std::vector<int64_t> theEmuInputQ;

   std::vector<int64_t> theEmuOutputQ;

   MazeViewer theViewer;

   Coord theDroidPosition;

   ElfComputer theEmu;
};


#endif
