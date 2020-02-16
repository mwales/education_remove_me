#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <map>

typedef std::pair<int, int> Coord;

std::string coordToString(Coord c);
std::string coordListToString(std::vector<Coord> const & l);

class Maze
{
public:

    Maze(std::vector<std::string> inputData);

    void dump();

    int getWidth() { return theWidth; }

    int getHeight() { return theHeight; }

    Coord getEntrance() { return theEntrance; }
    Coord getExit() { return theExit; }

    std::vector<Coord> getAdjacentCoords(Coord c);

    // 0 = not a portal, 1 = inner portal, 2 = outer portal
    int isLocationPortal(Coord c, std::string* portalVal);

private:

    char dataFromStringList(Coord, std::vector<std::string> const & stringData);

    int getMazeWidth(std::vector<std::string> const & stringData);

    int getMazeHeight(std::vector<std::string> const & stringData);

    void findPortals(std::vector<std::string> const & stringData);

    std::string getPortalString(std::vector<std::string> const & stringData, int x, int y, bool isHorizontal);

    void findHole();

    bool isPointInMaze(Coord c);

    std::map<Coord, char> theMazeData;

    std::map<std::string, Coord> theInnerPortals;

    std::map<std::string, Coord> theOuterPortals;

    Coord theEntrance;

    Coord theExit;

    int theWidth;

    int theHeight;

    Coord theHoleLocation;

    int theHoleWidth;

    int theHoleHeight;


};



#endif
