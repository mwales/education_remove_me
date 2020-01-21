#ifndef MAZE_SOLVER_2
#define MAZE_SOLVER_2

#include <iostream>
#include <vector>
#include <set>
#include "Maze.h"

typedef struct MazePathStruct
{
   std::vector<char> thePath;

   int theLength;
} MazePath;

std::string mazePathToString(MazePath const & mp);

class MazeSolver2
{
public:

   MazeSolver2(Maze* m);

   std::vector<MazePath> distToGetFollowingKeys(std::set<char> keysToGet,
                                 std::vector<char> keysHave,
                                 char keyAtCurrentLoc);

private:

   Maze* theMaze;
};









#endif
