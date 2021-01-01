#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

typedef std::pair<int, int> TileCoord;

class TileMap
{
public:
	TileMap();

	void displayMap();

	void getMinMax(int& xMin, int& xMax, int& yMin, int& yMax);

	void dump();

	int numNeighbors(int x, int y, char color);

	void addPath(std::string text);

	int countTiles(char state);

	TileCoord pathToCoord(std::string text);

	void runLifeRound();

	char getTile(int x, int y);

	std::map<TileCoord, char> theMap;
	int theRound;
};


char TileMap::getTile(int x, int y)
{
	if (theMap.find({x,y}) == theMap.end())
	{
		return 'W';
	}
	else
	{
		return theMap[{x,y}];
	}
}

void TileMap::runLifeRound()
{
	int minx, maxx, miny, maxy;

	// Leave white tiles out of sim.  keeps maps from growing too fast
	std::map<TileCoord, char> theFutureMap;

	getMinMax(minx, maxx, miny, maxy);

	// Expand the simulation area beyond the limits
	minx -= 2;
	maxx += 2;
	miny -= 1;
	maxy += 1;

	for(int y = miny; y <= maxy; y++)
	{
		for(int x = minx; x <= maxx; x++)
		{
			char curColor = getTile(x,y);

			if (curColor == 'W')
			{
				// Any white tile with 2 black neighbors turns black
				if (numNeighbors(x,y,'B') == 2)
				{
					theFutureMap[{x,y}] = 'B';
				}
			}
			else
			{
				// Currently black.  If neighbors are 0 or >2 black, turn white
				int blackNs = numNeighbors(x,y,'B');
				if ( (blackNs == 1) || (blackNs == 2) )
				{
					theFutureMap[{x,y}] = 'B';
				}
			}
		}

	}

	theMap = theFutureMap;
	theRound++;
}

int TileMap::numNeighbors(int x, int y, char color)
{
	std::vector<std::pair<int, int> > offsetList = { {-2,0}, {2,0}, {-1,-1},
	                                                 {-1,1}, {1,-1}, {1,1} };

	int retVal = 0;
	for(auto curOffset: offsetList)
	{
		int testX = x + curOffset.first;
		int testY = y + curOffset.second;

		if (getTile(testX, testY) == color)
		{
			retVal++;
		}
	}

	return retVal;
}

int TileMap::countTiles(char state)
{
	int retVal = 0;

	for(auto curTile: theMap)
	{
		if (curTile.second == state)
		{
			retVal++;
		}
	}

	return retVal;
}

TileCoord TileMap::pathToCoord(std::string text)
{
	TileCoord retVal = {0,0};

	for(int i = 0; i < text.size(); i++)
	{
		DEBUG << "Processing " << text << " at i=" << i << " and retVal="
		      << retVal << std::endl;

		if (text[i] == 'e')
		{
			retVal.first += 2;
			continue;
		}

		if (text[i] == 'w')
		{
			retVal.first -= 2;
			continue;
		}

		assert(i + 1 < text.size());

		if ( (text[i] == 'n') && (text[i+1] == 'w') )
		{
			retVal.second--;
			retVal.first--;
			i++;
			continue;
		}

		if ( (text[i] == 'n') && (text[i+1] == 'e') )
		{
			retVal.second--;
			retVal.first++;
			i++;
			continue;
		}

		if ( (text[i] == 's') && (text[i+1] == 'w') )
		{
			retVal.second++;
			retVal.first--;
			i++;
			continue;
		}

		if ( (text[i] == 's') && (text[i+1] == 'e') )
		{
			retVal.second++;
			retVal.first++;
			i++;
			continue;
		}

		std::cout << "We got to bad spot of " << __FUNCTION__
		          << "next chars: " << text[i] << text[i+1] << std::endl;
		assert(false);

	}
	
	DEBUG << __FUNCTION__ << " of " << text << " = " << retVal << std::endl;
	return retVal;
}

void TileMap::addPath(std::string text)
{
	TileCoord tc = pathToCoord(text);

	if (theMap.find(tc) == theMap.end())
	{
		theMap[tc] = 'B';
	}
	else
	{
		// Flip the tile that exists already
		DEBUG << "Fliiping existing tile from old color of " << theMap[tc] << std::endl;
		if (theMap[tc] == 'W')
		{
			theMap[tc] = 'B';
		}
		else
		{
			theMap[tc] = 'W';
		}
	}
}

TileMap::TileMap()
{
	theRound = 0;
}

void TileMap::displayMap()
{
	int minx, miny, maxx, maxy;
	getMinMax(minx, maxx, miny, maxy);
	
	// Start map off on even numbers
	if (minx % 2)
	{
		minx--;
	}

	if (miny % 2)
	{
		miny--;
	}

	for(int y = miny; y <= maxy; y++)
	{
		// Display a top
		for(int x = minx; x <= maxx; x += 2)
		{
			DEBUG << " / \\";
		}
		DEBUG << std::endl;

		for(int x = minx; x <= maxx; x += 2)
		{
			DEBUG << "| ";

			if (theMap.find({x,y}) == theMap.end())
			{
				// Empty square
				if ( (x == 0) && (y == 0) )
				{
					DEBUG << "r";
				}
				else
				{
					DEBUG << " ";
				}
			}
			else
			{
				if ( (x == 0) && (y == 0) )
				{
					DEBUG << (char) tolower(theMap[{x,y}]);
				}
				else
				{
					DEBUG << theMap[{x,y}];
				}
			}

			DEBUG << " ";

		}
		DEBUG << std::endl;	

		for(int x = minx; x <= maxx; x += 2)
		{
			DEBUG << " \\ /";
		}
		DEBUG << std::endl;

		y++;
		DEBUG << "  ";
		for(int x = minx + 1; x <= maxx; x += 2)
		{
			DEBUG << "| ";

			if (theMap.find({x,y}) == theMap.end())
			{
				// Empty square
				if ( (x == 0) && (y == 0) )
				{
					DEBUG << "r";
				}
				else
				{
					DEBUG << " ";
				}
			}
			else
			{
				DEBUG << theMap[{x,y}];
			}

			DEBUG << " ";

		}
		DEBUG << std::endl;	


		

	}


}

void TileMap::getMinMax(int& xMin, int& xMax, int& yMin, int& yMax)
{
	xMin = 0;
	xMax = 0;
	yMin = 0;
	yMax = 0;

	for(auto curTile: theMap)
	{
		int curX = curTile.first.first;
		int curY = curTile.first.second;

		xMin = (curX < xMin ? curX : xMin);
		yMin = (curY < yMin ? curY : yMin);
		xMax = (curX > xMax ? curX : xMax);
		yMax = (curY > yMax ? curY : yMax);
	}
}

void TileMap::dump()
{
	DEBUG << "*** CURRENT TILES ***" << std::endl;
	for(auto curCoord: theMap)
	{
		DEBUG << curCoord.first.first << "," << curCoord.first.second
		      << " = " << curCoord.second << std::endl;
	}
}

int main(int argc, char** argv)
{

	if (argc < 3)
	{
		std::cerr << "Provide filename numRounds" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	TileMap tm;
	for(auto curLine: fileData)
	{
		tm.addPath(curLine);	
	}

	tm.dump();
	tm.displayMap();

	int part1Sol = tm.countTiles('B');

	for(int i = 0; i < atoi(argv[2]); i++)
	{
		tm.runLifeRound();

		DEBUG << "Round " << tm.theRound << std::endl;
		tm.displayMap();
		DEBUG << "Num Black Tiles: " << tm.countTiles('B') << std::endl;

		DEBUG << std::endl;
	}

	int part2Sol = tm.countTiles('B');

	
	std::cout << "Part 1: " << part1Sol << std::endl;
	std::cout << "Part 2: " << part2Sol << std::endl;
	
	return 0;
}
