#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <iostream>
#include <vector>
#include <set>

class Maze;

class MazeSolver
{
public:

   MazeSolver(Maze* m, int x, int y, int numSteps);

   void setKeys(std::vector<char> keys);

   int numStepsToSolution(int bestSolutionFound = -1);

   void addNeighbors(int x, int y);

   bool haveWeVisited(int x, int y);

   void addPointVisited(int x, int y);

   std::string pointsVisitedToString(std::string sep);

   bool isPointBlocked(int x, int y);



   bool doWeHaveKey(char x);

   void printSolverStats();

protected:

   Maze* theMaze;

   std::vector<char> theKeys;

   std::set<int> thePointsVisited;

   std::vector<std::pair<int, int> > thePointsToVisit;

   std::vector<MazeSolver> theNextSolvers;

   int theNumSteps;

   int thePosX;

   int thePosY;

   static int theNumSolversCreated;

   static int theNumSolversFinished;

   static int theNumSolversAborted;
};


#endif
