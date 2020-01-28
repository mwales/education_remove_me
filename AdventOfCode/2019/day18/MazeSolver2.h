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

   std::vector<char> appendKeyPath(std::vector<char> const & original,
                                   std::vector<char> const & append);

private:

   bool isKeyInList(char needle, std::vector<char> const & keyList);

   std::set<char> updateKeysToGet(std::set<char> const & oldKeysToGet,
                                  std::vector<char> const & autoFoundKeys);

   std::vector<char> updateKeysHave(std::vector<char> const & oldKeyList,
                                    std::vector<char> const & autoFoundKeys);

   Maze* theMaze;
};









#endif
