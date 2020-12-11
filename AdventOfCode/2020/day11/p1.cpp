#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

class ChairMap
{
	public:
		ChairMap();

		void addRowSeats(std::string text);

		bool iterateSimulation();
		bool iterateSimulationPt2();

		int getNumOccupied(int x, int y);
		int getNumOccupied2(int x, int y);

		char getData(int x, int y);

		void setData(int x, int y, char val);

		int getMaxX();

		int getMaxY();

		int countAllSeats();

		void printMap();

		std::vector<std::string> theSeats;

};

ChairMap::ChairMap()
{

}

void ChairMap::addRowSeats(std::string text)
{
	theSeats.push_back(text);
}

int ChairMap::getMaxX()
{
	return theSeats[0].size();
}

int ChairMap::getMaxY()
{
	return theSeats.size();
}

int ChairMap::getNumOccupied(int x, int y)
{
	std::vector<std::pair<int, int> > offsets = { {-1,-1}, {0, -1}, {1, -1},
	                                              {-1, 0},          {1, 0},
					              {-1, 1}, {0, 1},  {1, 1}};

	int retVal = 0;
	for(auto curOffset: offsets)
	{
		int curX = x + curOffset.first;
		int curY = y + curOffset.second;

		if ( (curX < 0) || (curX >= getMaxX()))
		{
			continue;
		}

		if ( (curY < 0) || (curY >= getMaxY()))
		{
			continue;
		}

		if (getData(curX, curY) == '#')
		{
			retVal++;
		}
	}

	return retVal;
}

int ChairMap::getNumOccupied2(int x, int y)
{
	std::vector<std::pair<int, int> > directions = { {-1,-1}, {0, -1}, {1, -1},
	                                              {-1, 0},          {1, 0},
					              {-1, 1}, {0, 1},  {1, 1}};

	int retVal = 0;
	for(auto curOffset: directions)
	{
		int curX = x;
		int curY = y;
		
		while(true)
		{
			curX += curOffset.first;
			curY += curOffset.second;

			if ( (curX < 0) || (curX >= getMaxX()))
			{
				break;
			}

			if ( (curY < 0) || (curY >= getMaxY()))
			{
				break;
			}

			if (getData(curX, curY) == '#')
			{
				retVal++;
				break;
			}

			if (getData(curX, curY) == 'L')
			{
				break;
			}
		}
	}

	return retVal;
}




bool ChairMap::iterateSimulation()
{
	bool retVal = true; // map unchanged

	std::vector<std::string> nextState;

	for(int y = 0; y < getMaxY(); y++)
	{
		std::string curRow;
		for(int x = 0; x < getMaxX(); x++)
		{
			if (getData(x,y) == 'L')
			{
				// This seat will become occpied if no seats around it occpupied
				if (getNumOccupied(x,y) == 0)
				{
					curRow += '#';
				}
				else
				{
					curRow += 'L';
				}
			}
			else if (getData(x,y) == '#')
			{
				// Seat will become non-occupied if 4 or more near occupied
				if (getNumOccupied(x,y) >= 4)
				{
					curRow += 'L';
				}
				else
				{
					curRow += '#';
				}
				continue;
			}
			else
			{
				curRow += getData(x,y);
			}
		}

		nextState.push_back(curRow);

		if (curRow != theSeats[y])
		{
			retVal = false;
		}

	}

	theSeats = nextState;
	return retVal;
}

bool ChairMap::iterateSimulationPt2()
{
	bool retVal = true; // map unchanged

	std::vector<std::string> nextState;

	for(int y = 0; y < getMaxY(); y++)
	{
		std::string curRow;
		for(int x = 0; x < getMaxX(); x++)
		{
			DEBUG << getNumOccupied2(x,y);
			
			if (getData(x,y) == 'L')
			{
				// This seat will become occpied if no seats around it occpupied
				if (getNumOccupied2(x,y) == 0)
				{
					//DEBUG << "+";
					curRow += '#';
				}
				else
				{
					//DEBUG << "-";
					curRow += 'L';
				}
			}
			else if (getData(x,y) == '#')
			{
				// Seat will become non-occupied if 5 or more near occupied
				if (getNumOccupied2(x,y) >= 5)
				{
					//DEBUG << "!";
					curRow += 'L';
				}
				else
				{
					//DEBUG << "?";
					curRow += '#';
				}
				continue;
			}
			else
			{
				//DEBUG << ":";
				curRow += getData(x,y);
			}
		}

		DEBUG << std::endl;

		nextState.push_back(curRow);

		if (curRow != theSeats[y])
		{
			retVal = false;
		}

	}

	theSeats = nextState;
	return retVal;
}


void ChairMap::printMap()
{
	for(auto curRow: theSeats)
	{
		DEBUG << curRow << std::endl;
	}
}

int ChairMap::countAllSeats()
{
	int retVal = 0;
	for(int x = 0; x < getMaxX(); x++)
	{
		for(int y = 0; y < getMaxY(); y++)
		{
			if (getData(x,y) == '#')
			{
				retVal++;
			}
		}
	}

	return retVal;
}

char ChairMap::getData(int x, int y)
{
	return theSeats[y][x];
}

void ChairMap::setData(int x, int y, char val)
{
	theSeats[y][x] = val;
};

int main(int argc, char** argv)
{

	ChairMap cm;

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile,text);


		// out of output
check_for_eof:
      if (infile.eof())
		{
			break;
		}

		DEBUG << text << std::endl;
		cm.addRowSeats(text);
	}

	infile.close();

	ChairMap cm2 = cm;

	int iteration = 0;
	while(true)
	{
		if (cm.iterateSimulation() == true)
		{
			break;
		}

		DEBUG << "After Iteration " << ++iteration << std::endl;
		cm.printMap();
		DEBUG << std::endl;
	}

	DEBUG << "Final Part 1" << std::endl;
	cm.printMap();

	int pt1 = cm.countAllSeats();
	std::cout << "Num Seats at end part 1 = " << pt1 << std::endl;


	std::cout << "********************** PART 2 **********************" << std::endl;
	cm2.printMap();

	iteration = 0;
	while(true)
	{
		if (cm2.iterateSimulationPt2() == true)
		{
			break;
		}

		DEBUG << "After Iteration " << ++iteration << std::endl;
		cm2.printMap();
		DEBUG << std::endl;
	}

	DEBUG << "Final Part 2" << std::endl;
	cm2.printMap();

	int pt2 = cm2.countAllSeats();
	std::cout << "Num Seats at end part 2 = " << pt2 << std::endl;






	return 0;
}
