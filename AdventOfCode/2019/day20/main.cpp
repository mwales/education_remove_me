#include<iostream>
#include<vector>
#include<fstream>
#include<set>

#include "Maze.h"


std::string readFile(std::string filename)
{
   std::ifstream f;
   f.open(filename);
   std::string retVal;

   while(true)
   {
      std::string nextLine;
      std::getline(f, nextLine);

      if (f.eof())
      {
         break;
      }

      retVal += nextLine;
      retVal += "\n";
   }

   return retVal;
}

std::vector<std::string> splitString(std::string const & longString, char delim)
{
    std::vector<std::string> retVal;
    std::string curString;
    int numChars = 0;
    for(auto it = longString.begin(); it != longString.end(); it++)
    {
        numChars++;
        if (*it == delim)
        {
            // std::cout << "Found delimiter with curString: " << curString << std::endl;

            if (curString.size() > 0)
            {
                retVal.push_back(curString);
            }

            curString = "";
        }
        else
        {
            curString += *it;
        }
    }

    // std::cout << "CurString at end: " << curString << std::endl;

    // std::cout << "Processed " << numChars << " chars" << std::endl;

    return retVal;
}

void printStringList(std::vector<std::string> const & data)
{
    for(auto it = data.begin(); it != data.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}

void solveMaze(Maze m)
{
    std::set<Coord> placesVisited;

    std::set<Coord> placesToVisitThisTurn;
    std::set<Coord> placesVisitedLastTurn;

    placesVisited.insert(m.getEntrance());
    placesVisitedLastTurn.insert(m.getEntrance());

    int numRounds = 0;

    while(true)
    {
        // A turn of visiting stuff
        numRounds++;

        if(placesVisitedLastTurn.size() == 0)
        {
            std::cout << "Nothing more to visit" << std::endl;
            return;
        }

        // Find all the places we should visit
        placesToVisitThisTurn.clear();
        for(auto it = placesVisitedLastTurn.begin(); it != placesVisitedLastTurn.end(); it++)
        {
            std::vector<Coord> neighbors = m.getAdjacentCoords(*it);

            // Before adding to places to visit this turn, make sure we haven't already visited
            for(auto nIt = neighbors.begin(); nIt != neighbors.end(); nIt++)
            {
                if (placesVisited.find(*nIt) == placesVisited.end())
                {
                    // This is a new place to visit
                    placesToVisitThisTurn.insert(*nIt);
                }
            }
        }

        placesVisitedLastTurn.clear();

        // Visit all the places that we are supposed to on this turn
        for(auto vIt = placesToVisitThisTurn.begin(); vIt != placesToVisitThisTurn.end(); vIt++)
        {
            // Is this coord the exit?
            if (*vIt == m.getExit())
            {
                goto DONE_SEARCH;
            }

            placesVisited.insert(*vIt);
            placesVisitedLastTurn.insert(*vIt);
        }

    }

    DONE_SEARCH:

    std::cout << "Done searching after " << numRounds << " steps" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " mazefile" << std::endl;
        return 1;
    }

    std::string fileData = readFile(argv[1]);

    std::cout << "Length of file = " << fileData.size() << std::endl;

    std::vector<std::string> mazeData = splitString(fileData, '\n');

    std::cout << "Rows of maze data " << mazeData.size() << std::endl;

    printStringList(mazeData);

    Maze m(mazeData);

    m.dump();

    solveMaze(m);

    std::cout << "\n\n\nSolve Recursion Mode:" << std::endl;
    m.setRecursionMode(true);

    solveMaze(m);
}
