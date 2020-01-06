#include "MazeSolver.h"
#include <assert.h>
#include <unistd.h>

// #define SOLVER_DEBUG_ENABLE
#ifdef SOLVER_DEBUG_ENABLE
   #define SOLVER_DEBUG std::cerr
#else
   #define SOLVER_DEBUG if(0) std::cerr
#endif

MazeSolver::MazeSolver(ElfCode pd):
   theDroidPosition(0,0),
   theEmu(pd, "solver")
{
   theEmu.setInputDataQ(&theEmuInputQ);
   theEmu.setOutputDataQ(&theEmuOutputQ);
}

std::vector<int> MazeSolver::findPathToExit(bool stopAtGoal)
{
   std::vector<int> retVal;
   // theViewer.closeViewer();

   Coord solveStartPoint = theDroidPosition;

   // Add 0,0 to the "to visit list"
   MazeLocation startLocation(theDroidPosition);
   startLocation.setContent(MAZE_EMPTY);
   theViewer.addPoint(0, 0, MazeViewer::EMPTY);
   theLocationsVisited.push_back(startLocation);

   // Add everything around the start as locations to visit
   visitUnvisitedNeighbors(startLocation);

   // Loop, Visit everything on the visit list
   while(!theLocationsToVisit.empty())
   {
      // Backtrack from current location to common location
      MazeLocation nextLoc = theLocationsToVisit.at(0);
      Coord backtrackPoint = findCommonBackTrackPoint(nextLoc, theDroidPosition, solveStartPoint);
      backtrack(backtrackPoint);

      // Now we are going to move forward to the proper location
      int moveResult = MAZE_EMPTY;
      while(moveResult != MAZE_WALL)
      {
         if (theDroidPosition == nextLoc)
         {
            break;
         }

         // What is the next direction we should move to?
         MazeLocation currentLoc = getVisitedLocation(theDroidPosition);
         int currentLocNumSteps = currentLoc.getNumSteps();
         int direction = nextLoc.getPath().at(currentLocNumSteps);

         moveResult = move(direction);
      }

      // Move location from "to visit" to "already visited"
      nextLoc.setContent(moveResult);
      theLocationsToVisit.erase(theLocationsToVisit.begin());
      theLocationsVisited.push_back(nextLoc);

      if (moveResult != MAZE_WALL)
      {
         // Add locations neighbors to the "to visit" list
         visitUnvisitedNeighbors(nextLoc);
      }

      // Did we happen to find the goal?
      if (moveResult == MAZE_GOAL)
      {
         retVal = nextLoc.getPath();

         if (stopAtGoal)
         {
            return retVal;
         }
      }

      SOLVER_DEBUG << "*** Iteration complete ***" << std::endl;
      SOLVER_DEBUG << " nextLoc = " << nextLoc.toString() << std::endl;
      SOLVER_DEBUG << " toVisit:" << std::endl;
      for(auto it = theLocationsToVisit.begin(); it != theLocationsToVisit.end(); it++)
      {
         SOLVER_DEBUG << "   " << it->toString() << std::endl;
      }
      SOLVER_DEBUG << " visited:" << std::endl;
      for(auto it = theLocationsVisited.begin(); it != theLocationsVisited.end(); it++)
      {
         SOLVER_DEBUG << "   " << it->toString() << std::endl;
      }
      SOLVER_DEBUG << std::endl;
   }

   return retVal;


}

int MazeSolver::longestPath()
{
   int retVal = 0;
   for(auto it = theLocationsVisited.begin(); it != theLocationsVisited.end(); it++)
   {
      if (it->getNumSteps() > retVal)
      {
         retVal = it->getNumSteps();
      }
   }

   return retVal;
}

void MazeSolver::printMap()
{
   theViewer.printWholeMap();
}

void MazeSolver::getMapLimits(Coord & topLeft, Coord & bottomRight)
{
   theViewer.getMapLimits(topLeft, bottomRight);
}

void MazeSolver::reset()
{
   theLocationsToVisit.clear();
   theLocationsVisited.clear();
}

int MazeSolver::move(int direction)
{
   theEmuInputQ.push_back(direction);

   Coord newDroidPos(0,0);
   switch(direction)
   {
   case MAZE_NORTH:
      newDroidPos = Coord(theDroidPosition.x(), theDroidPosition.y() + 1);
      break;
   case MAZE_EAST:
      newDroidPos = Coord(theDroidPosition.x() + 1, theDroidPosition.y());
      break;
   case MAZE_SOUTH:
      newDroidPos = Coord(theDroidPosition.x(), theDroidPosition.y() - 1);
      break;
   case MAZE_WEST:
      newDroidPos = Coord(theDroidPosition.x() - 1, theDroidPosition.y());
      break;
   }

   int numIterations = 0;
   while(!theEmu.isHalted() && theEmuOutputQ.empty())
   {
      theEmu.runIteration();
      numIterations++;
   }

   if (!theEmuOutputQ.empty())
   {
      int retVal = theEmuOutputQ.at(0);
      theEmuOutputQ.clear();

      std::string debugString;
      debugString = "Moved dir ";
      debugString += std::to_string(direction);
      debugString += " (";
      debugString += directionToString(direction);
      debugString += "), result ";
      debugString += std::to_string(retVal);
      debugString += " (";
      debugString += mazeValueToString(retVal);
      debugString += ") after ";
      debugString += std::to_string(numIterations);
      debugString += " iterations";
      theViewer.onScreenDebug(debugString);
      SOLVER_DEBUG << debugString << std::endl;

      switch(retVal)
      {
      case MAZE_EMPTY:
         theDroidPosition = newDroidPos;
         theViewer.addPoint(theDroidPosition.x(), theDroidPosition.y(),
                            MazeViewer::EMPTY);
         break;
      case MAZE_WALL:
         theViewer.addPoint(newDroidPos.x(), newDroidPos.y(),
                            MazeViewer::WALL);
         break;
      case MAZE_GOAL:
         theDroidPosition = newDroidPos;
         theViewer.addPoint(theDroidPosition.x(), theDroidPosition.y(),
                            MazeViewer::GOAL);
         break;
      }

      SOLVER_DEBUG << "New droid position is at " << theDroidPosition.toString() << " vs " << newDroidPos.toString() << std::endl;
      theViewer.setDroidPosition(theDroidPosition.x(), theDroidPosition.y());
      theViewer.printScreen();

      return retVal;
   }
   else
   {
      std::string debugString;
      debugString = "Emu halted after ";
      debugString += std::to_string(numIterations);
      debugString += " iterations while moving ";
      debugString += std::to_string(direction);
      theViewer.onScreenDebug(debugString);
      SOLVER_DEBUG << debugString << std::endl;

      theViewer.printScreen();

      return -1;
   }
}

void MazeSolver::backtrack(Coord loc)
{
   SOLVER_DEBUG << "backtrack(" << loc.toString() << ")" << std::endl;

   while(theDroidPosition != loc)
   {
      // Where do we go?
      MazeLocation currentLoc = getVisitedLocation(theDroidPosition);

      int lastDirection = currentLoc.getPath().back();

      int result = -1;
      std::string backTrackDirection;
      switch(lastDirection)
      {
      case MAZE_NORTH:
         result = move(MAZE_SOUTH);
         backTrackDirection = "south";
         break;
      case MAZE_EAST:
         result = move(MAZE_WEST);
         backTrackDirection = "west";
         break;
      case MAZE_SOUTH:
         result = move(MAZE_NORTH);
         backTrackDirection = "north";
         break;
      case MAZE_WEST:
         result = move(MAZE_EAST);
         backTrackDirection = "east";
         break;
      }

      std::string debugString;
      debugString = "Backtracked ";
      debugString += backTrackDirection;
      debugString += " from ";
      debugString += currentLoc.getLocation().toString();
      theViewer.onScreenDebug(debugString);
      SOLVER_DEBUG << debugString << std::endl;
   }
}

Coord MazeSolver::findCommonBackTrackPoint(MazeLocation a, Coord b, Coord startPoint)
{
   SOLVER_DEBUG << "findCommonBackTrackPoint(" << a.toString() << ","
                << b.toString() << ")" << std::endl;

   std::vector<int> pathA = a.getPath();
   std::vector<int> pathB = getVisitedLocation(b).getPath();

   int numStepsMax = pathA.size();
   if (numStepsMax > pathB.size())
   {
      numStepsMax = pathB.size();
   }

   Coord retVal = startPoint;
   for(int i = 0; i < numStepsMax; i++)
   {
      if (pathA.at(i) == pathB.at(i))
      {
         // Update the coord to this new location
         switch(pathA.at(i))
         {
         case MAZE_NORTH:
            retVal = Coord(retVal.x(), retVal.y() + 1);
            break;

         case MAZE_EAST:
            retVal = Coord(retVal.x() + 1, retVal.y());
            break;

         case MAZE_SOUTH:
            retVal = Coord(retVal.x(), retVal.y() - 1);
            break;

         case MAZE_WEST:
            retVal = Coord(retVal.x() - 1, retVal.y());
            break;

         }
      }
      else
      {
         // Paths differ at the next coordinate, so return the coord
         return retVal;
      }
   }

   return retVal;
}

bool MazeSolver::isAlreadyVisited(MazeLocation ml)
{
   for(auto it = theLocationsVisited.begin(); it != theLocationsVisited.end(); it++)
   {
      if (*it == ml)
      {
         SOLVER_DEBUG << "isAlreadyVisited(" << ml.toString() << ") = true" << std::endl;
         return true;
      }
   }

   SOLVER_DEBUG << "isAlreadyVisited(" << ml.toString() << ") = false" << std::endl;
   return false;
}

MazeLocation MazeSolver::getVisitedLocation(Coord c)
{
   for(auto it = theLocationsVisited.begin(); it != theLocationsVisited.end(); it++)
   {
      if (it->getLocation() == c)
      {
         SOLVER_DEBUG << "getVisitedLocation found match for " << c.toString()
                      << " and " << it->toString() << std::endl;
         return *it;
      }
      else
      {
         SOLVER_DEBUG << "getVisitedLocation missing with " << c.toString()
                      << " and " << it->toString() << std::endl;

      }
   }

   // If we don't find the coordinate in any of these, error
   SOLVER_DEBUG << "getVisitedLocation(" << c.toString() << ") failed!" << std::endl;
   assert(false);

   return MazeLocation(Coord(0,0));
}

void MazeSolver::visitUnvisitedNeighbors(MazeLocation ml)
{
   SOLVER_DEBUG << "visitUnvisitedNeighbors(" << ml.toString() << ")" << std::endl;
   for(int i = 1; i <= 4; i++)
   {
      MazeLocation unvisitedLoc = ml.createNeighbor(i);

      if (!isAlreadyVisited(unvisitedLoc))
      {
         std::string debugString;
         debugString = "Adding unvisited node @ ";
         debugString += ml.getLocation().toString();
         theViewer.onScreenDebug(debugString);

         theLocationsToVisit.push_back(unvisitedLoc);
      }
   }
}
