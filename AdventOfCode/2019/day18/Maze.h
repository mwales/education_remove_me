#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>

class Maze
{
public:

   Maze(std::vector<std::string> data);

   void findStartPoint(int & x, int & y);

   char getPoint(int x, int y);

   int getNumKeys() { return theNumKeys; };

   void computeKeyPathLens();

private:

   std::vector<std::string> theMazeData;

   int theWidth;

   int theHeight;

   int theNumKeys;
};

#endif // MAZE_H
