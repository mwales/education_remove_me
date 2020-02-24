#include "Maze.h"
#include <assert.h>

//#define MAZE_DEBUG
#ifdef MAZE_DEBUG
    #define MDBG std::cout
#else
    #define MDBG if(0) std::cout
#endif

Coord makeCoord(int x, int y, int ringLevel)
{
    Coord retVal;
    retVal.first = x;
    retVal.second = y;
    retVal.ring = ringLevel;
    return retVal;
}


bool operator==(Coord const & lhs, Coord const & rhs)
{
    return (lhs.first == rhs.first) &&
           (lhs.second == rhs.second) &&
           (lhs.ring == rhs.ring);
}

bool operator!=(Coord const & lhs, Coord const & rhs)
{
    return !(lhs == rhs);
}

bool operator<(Coord const & lhs, Coord const & rhs)
{
    if (lhs == rhs)
        return false;

    if (lhs.ring < rhs.ring)
        return true;

    if (lhs.ring > rhs.ring)
        return false;

    if (lhs.first < rhs.first)
        return true;

    if (lhs.first > rhs.first)
        return false;

    return (lhs.second < rhs.second);
}

bool operator>(Coord const & lhs, Coord const & rhs)
{
    if (lhs == rhs)
        return false;

    return !(lhs < rhs);
}

bool operator<=(Coord const & lhs, Coord const & rhs)
{
    if (lhs == rhs)
        return true;

    return (lhs < rhs);
}
bool operator>=(Coord const & lhs, Coord const & rhs)
{
    if (lhs == rhs)
        return true;

    return (lhs > rhs);
}

std::string coordToString(Coord c)
{
    std::string retVal = "(";
    retVal += std::to_string(c.first);
    retVal += ",";
    retVal += std::to_string(c.second);
    retVal += ",ring=";
    retVal += std::to_string(c.ring);
    retVal += ")";
    return retVal;
}

std::string coordListToString(std::vector<Coord> const & l)
{
    bool isFirst = true;
    std::string retVal;

    for(auto it = l.begin(); it != l.end(); it++)
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            retVal += ", ";
        }

        retVal += coordToString(*it);
    }

    return retVal;
}

Maze::Maze(std::vector<std::string> inputData)
{
    theWidth = getMazeWidth(inputData);
    theHeight = getMazeHeight(inputData);

    MDBG << "W=" << theWidth << ", and H=" << theHeight << std::endl;

    for(int y = 0; y < theHeight; y++)
    {
        for(int x = 0; x < theWidth; x++)
        {
            Coord curCoord = makeCoord(x,y);
            char curData = dataFromStringList(curCoord, inputData);

            if ( (curData == '.') || (curData == '#') )
            {
                // This is valid maze data, add to theMazeData
                theMazeData[curCoord] = curData;
            }
        }
    }

    findHole();

    findPortals(inputData);

    // Find the entrance
    std::string entranceString = "AA";
    int numEntrancesOuter = theOuterPortals.count(entranceString);
    int numEntrancesInner = theInnerPortals.count(entranceString);

    assert( (numEntrancesOuter == 1) || (numEntrancesInner == 1) );

    if (numEntrancesOuter)
    {
        // Entrance is in the outer
        theEntrance = theOuterPortals[entranceString];
        theOuterPortals.erase(theOuterPortals.find(entranceString));
    }
    else
    {
        // Entrance is in the inner
        theEntrance = theInnerPortals[entranceString];
        theInnerPortals.erase(theInnerPortals.find(entranceString));
    }

    // Find the exit
    std::string exitString = "ZZ";
    int numExitOuter = theOuterPortals.count(exitString);
    int numExitInner = theInnerPortals.count(exitString);

    assert( (numExitOuter == 1) || (numExitInner == 1) );


    if (numExitOuter)
    {
        // Entrance is in the outer
        theExit = theOuterPortals[exitString];
        theOuterPortals.erase(theOuterPortals.find(exitString));
    }
    else
    {
        // Entrance is in the inner
        theExit = theInnerPortals[exitString];
        theInnerPortals.erase(theInnerPortals.find(exitString));
    }
}

void Maze::setRecursionMode(bool val)
{
    theRecursionMode = val;
}

void Maze::dump()
{
    std::cout << "Maze " << theWidth << " x " << theHeight << std::endl;

    for(int y = 0; y < theHeight; y++)
    {
        for(int x = 0; x < theWidth; x++)
        {
            Coord cur = makeCoord(x,y);
            if (isPointInMaze(cur))
            {
                std::cout << theMazeData[cur];
            }
            else
            {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << "\nStats:" << std::endl;

    std::cout << "Recursion = " << (theRecursionMode ? "Recursive" : "Not Recursive") << std::endl;

    std::cout << "Hole @ " << coordToString(theHoleLocation) << "  " << theHoleWidth
              << "x" << theHoleHeight << std::endl;

    std::cout << "Outer portals:" << std::endl;
    for(auto it = theOuterPortals.begin(); it != theOuterPortals.end(); it++)
    {
        std::cout << "  " << it->first << " = " << coordToString(it->second) << std::endl;
    }

    std::cout << "Inner portals:" << std::endl;
    for(auto it = theInnerPortals.begin(); it != theInnerPortals.end(); it++)
    {
        std::cout << "  " << it->first << " = " << coordToString(it->second) << std::endl;
    }

    std::cout << "Entrance: " << coordToString(theEntrance) << std::endl;
    std::cout << "    Exit: " << coordToString(theExit) << std::endl;

}

std::vector<Coord> Maze::getAdjacentCoords(Coord c)
{
    MDBG << "getAdjacentCoords(" << coordToString(c) << ")" << std::endl;

    int originalRing = c.ring;
    c.ring = 0;

    assert (theMazeData[c] == '.');

    std::vector<Coord> retVal;

    std::vector<Coord> potentialPoints;
    potentialPoints.push_back(makeCoord(c.first+1, c.second, originalRing));
    potentialPoints.push_back(makeCoord(c.first-1, c.second, originalRing));
    potentialPoints.push_back(makeCoord(c.first, c.second+1, originalRing));
    potentialPoints.push_back(makeCoord(c.first, c.second-1, originalRing));

    // Is this coordinate a portal?
    std::string portalName;
    int isPortal = isLocationPortal(c, &portalName);
    if (isPortal)
    {
        // The other portal is "adjacent"
        if (isPortal == 1)
        {
            MDBG << "Found an inner portal!" << std::endl;
            // we are on inner portal, return the outer portal
            assert(theOuterPortals.find(portalName) != theOuterPortals.end());

            Coord otherSide = theOuterPortals[portalName];

            if (theRecursionMode)
            {
                otherSide.ring = originalRing + 1;
            }
            retVal.push_back(otherSide);
        }
        else
        {
            MDBG << "Found an outer portal" << std::endl;
            // we are on outer portal, return the inner portal
            assert(theInnerPortals.find(portalName) != theInnerPortals.end());

            Coord otherSide = theInnerPortals[portalName];
            if (theRecursionMode)
            {
                otherSide.ring = originalRing - 1;
            }

            retVal.push_back(otherSide);
        }
    }

    // Test the potential points and see if they are valid
    for(auto it = potentialPoints.begin(); it != potentialPoints.end(); it++)
    {
        // Is the point in range
        if ( (it->first < 0) || (it->first >= theWidth) )
        {
            // Not within bounds on x coordinate
            continue;
        }
        if ( (it->second < 0) || (it->second >= theHeight) )
        {
            // Not within bouds on y coordinate
            continue;
        }

        // Is it a wall?
        if (!isLocationWall(*it))
        {
            retVal.push_back(*it);
        }
    }

    // std::cout << "   retVal = " << coordListToString(retVal) << std::endl;
    return retVal;
}

// 0 = not a portal, 1 = inner portal, 2 = outer portal
int Maze::isLocationPortal(Coord c, std::string* portalVal)
{
    assert(portalVal != 0);

    for (auto it = theInnerPortals.begin(); it != theInnerPortals.end(); it++)
    {
        if (it->second == c)
        {
            *portalVal = it->first;
            MDBG << "isLocationPortal(" << coordToString(c) << ") = 1" << std::endl;
            return 1;
        }
    }

    for(auto it = theOuterPortals.begin(); it != theOuterPortals.end(); it++)
    {
        if (it->second == c)
        {
            *portalVal = it->first;
            MDBG << "isLocationPortal(" << coordToString(c) << ") = 2" << std::endl;
            return 2;
        }
    }

    // Must not have been a portal
    MDBG << "isLocationPortal(" << coordToString(c) << ") = 0" << std::endl;
    return 0;
}

bool Maze::isLocationWall(Coord c)
{
    MDBG << "entered isLocationWall(" << coordToString(c) << ")" << std::endl;

    int originalRing = c.ring;

    // Change ring to 0 for lookups and stuff
    c.ring = 0;

    // MDBG << "e" << std::endl;

    // Is this in the hole?
    if (theMazeData.find(c) == theMazeData.end())
    {
        MDBG << "isLocationWall(" << coordToString(c) << ") = true - hole" << std::endl;
        return true;
    }

    if (theMazeData[c] == '#')
    {
        MDBG << "isLocationWall(" << coordToString(c) << ") = true - wall" << std::endl;
        return true;
    }

    MDBG << "Need to check for recursion mode" << std::endl;

    if (!theRecursionMode)
    {
        // If not recursion mode, then this is not a wall
        MDBG << "isLocationWall(" << coordToString(c) << ") = false - simple tests prove not a wall" << std::endl;
        return false;
    }

    // So we are in recursion mode, things more compilicated
    // Entrance and exit are only valid on ring 0
    // Outer portals are walls on ring 0

    if (originalRing == 0)
    {
        // Ring 0 rules apply
        // Outer portals are walls
        for(auto it = theOuterPortals.begin(); it != theOuterPortals.end(); it++)
        {
            if (it->second == c)
            {
                MDBG << "isLocationWall(" << coordToString(c) << ") = true - ring 0 doesnt have outer portals" << std::endl;
                return true;
            }
        }

        MDBG << "isLocationWall(" << coordToString(c) << ") = false - ring 0 recursive mode" << std::endl;
        return false;
    }
    else
    {
        // Ring 1+ rules apply
        // Entrance and exit are walls
        bool retVal =( (theEntrance == c) || (theExit == c) );
        MDBG << "isLocationWall(" << coordToString(c) << ") = "
             << (retVal ? "true" : "false") << " recursive ring 1+ " << std::endl;
        return retVal;
    }

}

char Maze::dataFromStringList(Coord c, std::vector<std::string> const & stringData)
{
    int x = c.first + 2;
    int y = c.second + 2;

    assert(y < stringData.size());

    std::string selectedRow = stringData[y];

    assert(x < selectedRow.size());

    return selectedRow[x];
}

int Maze::getMazeWidth(std::vector<std::string> const & stringData)
{
    // Width is the width of any string, minus 2 before and 2 after
    return stringData.front().size() - 4;
}

int Maze::getMazeHeight(std::vector<std::string> const & stringData)
{
    // Height is number of string minus 2 before and 2 after
    return stringData.size() - 4;
}

void Maze::findPortals(std::vector<std::string> const & stringData)
{
    // Find the outer portals
    for(int x = 0; x < theWidth; x++)
    {
        MDBG << "findPortals outer top @ (" << x << ",0)" << std::endl;
        std::string ps = getPortalString(stringData, x, 0, false);
        if (ps != "  ")
        {
            theOuterPortals[ps] = makeCoord(x,0);
        }

        MDBG << "findPortals outer bottom @ (" << x << "," << theHeight - 1 << ")" << std::endl;
        ps = getPortalString(stringData, x, theHeight - 1, false);
        if (ps != "  ")
        {
            theOuterPortals[ps] = makeCoord(x,theHeight - 1);
        }
    }

    for(int y = 0; y < theHeight; y++)
    {
        MDBG << "findPortals outer left @ (" << 0 << "," << y << ")" << std::endl;
        std::string ps = getPortalString(stringData, 0, y, true);
        if (ps != "  ")
        {
            theOuterPortals[ps] = makeCoord(0, y);
        }

        MDBG << "findPortals outer right @ (" << theWidth - 1 << "," << y << ")" << std::endl;
        ps = getPortalString(stringData, theWidth-1, y, true);
        if (ps != "  ")
        {
            theOuterPortals[ps] = makeCoord(theWidth-1, y);
        }
    }

    // Find the inner portals
    for(int x = theHoleLocation.first; x < theHoleLocation.first + theHoleWidth - 1; x++)
    {
        MDBG << "findPortals inner top @ (" << x << "," << theHoleLocation.second - 1 << ")" << std::endl;
        std::string ps = getPortalString(stringData, x, theHoleLocation.second - 1, false);
        if ( (ps[0] != ' ') && (ps[1] != ' ') )
        {
            theInnerPortals[ps] = makeCoord(x, theHoleLocation.second - 1);
        }

        MDBG << "findPortals inner bottom @ (" << x << "," << theHoleLocation.second + theHoleHeight << ")" << std::endl;
        ps = getPortalString(stringData, x, theHoleLocation.second + theHoleHeight, false);
        if ( (ps[0] != ' ') && (ps[1] != ' ') )
        {
            theInnerPortals[ps] = makeCoord(x, theHoleLocation.second + theHoleHeight);
        }
    }

    for(int y = theHoleLocation.second; y < theHoleLocation.second + theHoleHeight; y++)
    {
        MDBG << "findPortals inner left @ (" << theHoleLocation.first-1 << "," << y << ")" << std::endl;
        std::string ps = getPortalString(stringData, theHoleLocation.first-1, y, true);
        if ( (ps[0] != ' ') && (ps[1] != ' ') )
        {
            theInnerPortals[ps] = makeCoord(theHoleLocation.first-1, y);
        }

        MDBG << "findPortals inner right @ (" << theHoleLocation.first + theHoleWidth << "," << y << ")" << std::endl;
        ps = getPortalString(stringData, theHoleLocation.first + theHoleWidth, y, true);
        if ( (ps[0] != ' ') && (ps[1] != ' ') )
        {
            theInnerPortals[ps] = makeCoord(theHoleLocation.first + theHoleWidth, y);
        }
    }

}

std::string Maze::getPortalString(std::vector<std::string> const & stringData, int x, int y, bool isHorizontal)
{
    MDBG << "getPortalString(data," << x << "," << y << "," << (isHorizontal ? "hor " : "vert") << ")  " ;

    assert(x >= 0);
    assert(x < theWidth);
    assert(y >= 0);
    assert(y < theHeight);

    if (isHorizontal)
    {
        assert( (x==0) || (x == theWidth - 1) || (x == theHoleLocation.first-1) || (x == theHoleLocation.first + theHoleWidth) );

        y += 2;

        if (x == 0)
        {
            // Is it the outer-left
            std::string retVal = stringData[y].substr(x,2);
            MDBG << "  outer-left retVal = " << retVal << std::endl;
            return retVal;
        }
        if (x == theHoleLocation.first-1)
        {
            // Is it in the inner-left
            std::string retVal = stringData[y].substr(x+3, 2);
            MDBG << "  inner-left retVal = " << retVal << std::endl;
            return retVal;
        }
        if (x == (theWidth - 1))
        {
            // Is it the outer-right
            std::string retVal = stringData[y].substr(x+3,2);
            MDBG << "  outer-right retVal = " << retVal << std::endl;
            return retVal;
        }

        // It must be inner-right
        std::string retVal = stringData[y].substr(x,2);
        MDBG << "  inner-right retVal = " << retVal << std::endl;
        return retVal;
    }

    assert( (y==0) || (y == theHeight - 1) || (y == theHoleLocation.second - 1) || (y == theHoleLocation.second + theHoleHeight) );

    x += 2;

    if (y == 0)
    {
        // Is it the outer top
        std::string retVal;
        retVal += stringData[y][x];
        retVal += stringData[y+1][x];
        MDBG << "  outer-top retVal = " << retVal << std::endl;
        return retVal;
    }
    if (y == (theHeight - 1))
    {
        // Is the outer bottom?
        std::string retVal;
        retVal += stringData[y+3][x];
        retVal += stringData[y+4][x];
        MDBG << "  outer-bottom retVal = " << retVal << std::endl;
        return retVal;
    }
    if (y == theHoleLocation.second-1)
    {
        // Is the inner top
        std::string retVal;
        retVal += stringData[y+3][x];
        retVal += stringData[y+4][x];
        MDBG << "  inner-top retVal = " << retVal << std::endl;
        return retVal;
    }

    // It must be inner bottom
    std::string retVal;
    retVal += stringData[y][x];
    retVal += stringData[y+1][x];
    MDBG << "  inner-bottom retVal = " << retVal << std::endl;
    return retVal;
}

void Maze::findHole()
{
    int halfX = theWidth / 2;
    int halfY = theHeight / 2;

    // Work acress the maze horizontally to find x location and width of hole
    int xLoc = -1;
    theHoleWidth = 0;
    for(int x = 0; x < theWidth; x++)
    {
        Coord curCoord = makeCoord(x, halfY);

        if (!isPointInMaze(curCoord))
        {
            // Is it the first point not in the m2aze, that would be hole location!
            if (xLoc == -1)
            {
                xLoc = x;
            }

            theHoleWidth++;
        }
    }

    // Work acress the maze horizontally to find x location and width of hole
    int yLoc = -1;
    theHoleHeight = 0;
    for(int y = 0; y < theHeight; y++)
    {
        Coord curCoord = makeCoord(halfX, y);

        if (!isPointInMaze(curCoord))
        {
            // Is it the first point not in the maze, that would be hole location!
            if (yLoc == -1)
            {
                yLoc = y;
            }

            theHoleHeight++;
        }
    }

    theHoleLocation = makeCoord(xLoc, yLoc);
}



bool Maze::isPointInMaze(Coord c)
{
    return theMazeData.find(c) != theMazeData.end();
}

