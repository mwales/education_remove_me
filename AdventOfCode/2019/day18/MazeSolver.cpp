#include "MazeSolver.h"

#include "Maze.h"

//#define MAZE_SOLVER_DEBUG
#ifdef MAZE_SOLVER_DEBUG
   #define MSDEBUG std::cout
#else
   #define MSDEBUG if(0) std::cout
#endif

int MazeSolver::theNumSolversCreated = 0;
int MazeSolver::theNumSolversFinished = 0;
int MazeSolver::theNumSolversAborted = 0;

MazeSolver::MazeSolver(Maze* m, int x, int y, int numSteps):
   theMaze(m),
   thePosX(x),
   thePosY(y),
   theNumSteps(numSteps)
{
   MSDEBUG << "Create a maze solver @ " << x << "," << y << " with numSteps =" << numSteps << std::endl;

   theNumSolversCreated++;

   if ( (theNumSolversCreated % 1000) == 0)
   {
      printSolverStats();
   }
}

void MazeSolver::setKeys(std::vector<char> keys)
{
   theKeys = keys;
   MSDEBUG << "Number of keys: " << theKeys.size() << std::endl;
}

int MazeSolver::numStepsToSolution(int bestSolutionFound)
{
   // Do we have the keys already?
   if (theMaze->getNumKeys() == theKeys.size())
   {
      MSDEBUG << "We are at the end of the maze!!! Steps=" << theNumSteps << " @ " << thePosX
              << "," << thePosY << std::endl;

      theNumSolversFinished++;

      if ( (theNumSolversFinished % 1000) == 0)
      {
         printSolverStats();
      }

      return theNumSteps;
   }

   // Should we abort this before even starting?
   if ( (bestSolutionFound != -1) && (theNumSteps >= bestSolutionFound) )
   {
      // Abort!
      theNumSolversAborted++;
      return -1;
   }

   // Make sure the current position is already visited
   addPointVisited(thePosX, thePosY);

   addNeighbors(thePosX, thePosY);

   std::vector<MazeSolver> nextSolvers;

   while(!thePointsToVisit.empty())
   {
      std::vector<std::pair<int, int> > visitOnThisLoop = thePointsToVisit;
      theNumSteps++;

      MSDEBUG << "Main Iteration Loop.  List To Visit:" << std::endl;
      for(auto it = visitOnThisLoop.begin(); it != visitOnThisLoop.end(); it++)
      {
         MSDEBUG << "   * " << it->first << "," << it->second << std::endl;
      }
      MSDEBUG << "List of visited already:" << pointsVisitedToString(", ") << std::endl;

      for(auto it = visitOnThisLoop.begin(); it != visitOnThisLoop.end(); it++)
      {
         MSDEBUG << "Inner Loop @ " << it->first << "," << it->second << "    AlreadyVisited="
                 << pointsVisitedToString(", ") << std::endl;

         if (haveWeVisited(it->first, it->second))
         {
            // We have already visited
            continue;
         }

         addPointVisited(it->first, it->second);

         if (!isPointBlocked(it->first, it->second))
         {
            // This point wasn't blocked, was it a new key?
            char data = theMaze->getPoint(it->first, it->second);
            if ( (data >= 'a') && (data <= 'z'))
            {
               // It is a key, did we already have it?
               if (!doWeHaveKey(data))
               {
                  // We found a new key!!!!
                  MazeSolver ms(theMaze, it->first, it->second, theNumSteps);
                  std::vector<char> keys = theKeys;
                  keys.push_back(data);
                  ms.setKeys(keys);

                  nextSolvers.push_back(ms);

                  // Not going to add neighbors
                  continue;
               }
            }

            addNeighbors(it->first, it->second);
         }
      }

      // Remove a single point from the toVisit list
      thePointsToVisit.erase(thePointsToVisit.begin());

      if ( (bestSolutionFound != -1) && (theNumSteps >= bestSolutionFound) )
      {
         // Don't bother processing anymore if better solutions exist
         break;
      }
   }

   MSDEBUG << "No more points to visit" << std::endl;

   MSDEBUG << "Maze Solver Objects: " << nextSolvers.size() << std::endl;

   if (nextSolvers.size() <= 0)
   {
      MSDEBUG << "We bailed before finding good solutuions" << std::endl;
      theNumSolversAborted++;
      return -1;
   }

   int retVal = -1;
   bool firstVal = true;

   for(auto it = nextSolvers.begin(); it != nextSolvers.end(); it++)
   {
      int curSolSteps = it->numStepsToSolution(retVal);
      MSDEBUG << "Cur Solver Num Steps " << curSolSteps << std::endl;

      if (firstVal)
      {
         retVal = curSolSteps;
         firstVal = false;
      }
      else
      {
         if ( (curSolSteps != -1) && (retVal > curSolSteps) )
         {
            retVal = curSolSteps;
         }
      }

   }

   theNumSolversFinished++;

   if ( (theNumSolversFinished % 1000) == 0)
   {
      printSolverStats();
   }

   return retVal;
}

void MazeSolver::addNeighbors(int x, int y)
{
   MSDEBUG << "addNeighbors() called @ " << x << "," << y << std::endl;

   std::vector<std::pair<int, int> > neighbors;
   neighbors.push_back(std::make_pair(x, y+1));
   neighbors.push_back(std::make_pair(x, y-1));
   neighbors.push_back(std::make_pair(x+1, y));
   neighbors.push_back(std::make_pair(x-1, y));

   for(auto it = neighbors.begin(); it != neighbors.end(); it++)
   {
      if (!haveWeVisited(it->first, it->second))
      {
         MSDEBUG << "Added " << it->first << "," << it->second << " to list of points to visit" << std::endl;
         thePointsToVisit.push_back(*it);
      }
   }
}

bool MazeSolver::haveWeVisited(int x, int y)
{
   int val = (x << 16) + y;
   if (thePointsVisited.find(val) != thePointsVisited.end())
   {
      MSDEBUG << "haveWeVisited(" << x << "," << y << ") = true" << std::endl;
      return true;
   }
//   for(auto it = thePointsVisited.begin(); it != thePointsVisited.end(); it++)
//   {
//      if ( (it->first == x) && (it->second == y) )
//      {
//         // MSDEBUG << "haveWeVisited(" << x << "," << y << ") = true" << std::endl;
//         return true;
//      }
//   }

   MSDEBUG << "haveWeVisited(" << x << "," << y << ") = false" << std::endl;
   return false;
}

void MazeSolver::addPointVisited(int x, int y)
{
   if (haveWeVisited(x,y))
   {
      return;
   }

   int valToEnter = (x << 16) + y;
   thePointsVisited.insert(valToEnter);
}

std::string MazeSolver::pointsVisitedToString(std::string sep)
{
   bool first = true;
   std::string retVal;

   for(auto it = thePointsVisited.begin(); it != thePointsVisited.end(); it++)
   {
      int y = 0xffff & *it;
      int x = (*it - y) >> 16;

      if (first)
      {
         first = false;
      }
      else
      {
         retVal += sep;
      }

      retVal += "(";
      retVal += std::to_string(x);
      retVal += ",";
      retVal += std::to_string(y);
      retVal += ")";
   }

   return retVal;
}

bool MazeSolver::isPointBlocked(int x, int y)
{
   MSDEBUG << "isPointBlocked(" << x << "," << y << ")" << std::endl;
   char data = theMaze->getPoint(x,y);

   // Path is always open
   if ( (data == '.') || (data == '@') )
   {
      MSDEBUG << "Found an empty space at " << x << "," << y << std::endl;
      return false;
   }

   // Path is always closed
   if (data == '#')
   {
      MSDEBUG << "Found a wall at " << x << "," << y << std::endl;
      return true;
   }

   // At this point, the path must be a key or a door

   // If it is lower-case, it is a key
   if ( (data >= 'a') && (data <= 'z') )
   {
      MSDEBUG << "Found a key at " << x << "," << y << std::endl;
      return false;
   }

   // It must be a locked door, do we have the key?
   if (doWeHaveKey(data))
   {
      MSDEBUG << "Found a locked door at " << x << "," << y << std::endl;
      return false;
   }

   // Must be a locked door that we don't have key for
   MSDEBUG << "Found a locked door at " << x << "," << y << std::endl;
   return true;
}

bool MazeSolver::doWeHaveKey(char x)
{
   char keyRequired = tolower(x);

   for(auto it = theKeys.begin(); it != theKeys.end(); it++)
   {
      if (*it == keyRequired)
      {
         MSDEBUG << "doWeHaveKey(" << x << ") = true" << std::endl;
         return true;
      }
   }

   // We didn't have the key
   MSDEBUG << "doWeHaveKey(" << x << ") = false" << std::endl;
   return false;
}

void MazeSolver::printSolverStats()
{
   std::cout << "The num solvers created = " << theNumSolversCreated << ", aborted = "
             << theNumSolversAborted << ", and solved = " << theNumSolversFinished << std::endl;
}
