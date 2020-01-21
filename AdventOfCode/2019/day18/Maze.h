#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <map>
#include <set>

typedef std::pair<int, int> Coord;
typedef std::vector<Coord> Path;

class Maze
{
public:

   Maze(std::vector<std::string> data);

   void findStartPoint(int & x, int & y);

   char getPoint(int x, int y);

   int getNumKeys() { return theNumKeys; };

   std::vector<char> getPossibleKeys();

   std::set<char> getPossibleKeys2();

   void computeKeyPathLens();

   int getWidth() { return theWidth; }

   int getHeight() { return theHeight; }

   std::set<Coord> getNeighbors(int x, int y);

   Coord getLocationOfKey(char key);

   void computeAllKeyDistances();

   std::map<char, int> getKeyDistances(char key);

   void breadthFirstFill(int x, int y, std::map<char, int> & keyDist);

   Path getPath(int x, int y);

   std::string charListToString(std::set<char> const & data);

   std::string charListToString(std::vector<char> const & data);

   void printState();

   bool canIGetKey(char key, std::vector<char> keysPossessed);

private:

   // Adds all the points surrounding the passed in point to thePathToEachPoint
   std::set<Coord> mapNeighbors(Coord coord);

   void computePathsToAllLocations();

   void computeAllKeyDependencies();

   bool isPointOnMap(int x, int y);

   std::vector<std::string> theMazeData;

   std::map<char, std::map<char, int> > theKeyDistances;

   int theWidth;

   int theHeight;

   int theNumKeys;

   std::map<Coord, Path > thePathToEachPoint;

   std::map<char, std::set<char> > theKeyDependencies;

   std::map<char, std::set<char> > theKeysAutomatic;
};

#endif // MAZE_H
