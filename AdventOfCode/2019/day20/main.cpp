#include<iostream>
#include<vector>
#include<fstream>

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
            std::cout << "Found delimiter with curString: " << curString << std::endl;

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

    std::cout << "CurString at end: " << curString << std::endl;

    std::cout << "Processed " << numChars << " chars" << std::endl;

    return retVal;
}

void printStringList(std::vector<std::string> const & data)
{
    for(auto it = data.begin(); it != data.end(); it++)
    {
        std::cout << *it << std::endl;
    }
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

    // 7,1
    // 16,6
    std::vector<Coord> a = m.getAdjacentCoords(std::make_pair(7,1));
    std::cout << "(7,1) = " << coordListToString(a) << std::endl;

    std::vector<Coord> b = m.getAdjacentCoords(std::make_pair(0,11));
    std::cout << "(0,11) = " << coordListToString(b) << std::endl;


}
