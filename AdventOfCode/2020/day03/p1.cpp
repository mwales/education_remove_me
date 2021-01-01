#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

// #define AOC_DEBUG
#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif


void printRow(std::string const & row, int pos)
{
	int x = 0;
	for(auto & singleChar: row)
	{
		if (x == pos)
		{
			// print X or O
			if (singleChar == '#')
			{
				DEBUG << "X";
			}
			else
			{
				DEBUG << "O";
			}
		}
		else
		{
			// print landscape
			DEBUG << singleChar;
		}

		x++;
	}

	DEBUG << std::endl;
}

int navSlope(std::vector<std::string> const & slopeRows, int x, int y)
{

	int curX = 0;
	int curY = 0;
	int treesHit = 0;

	while(true)
	{
		curX += x;

		// Print entire rows we are skipping...
		for(int i = 0; i < y - 1; i++)
		{
			printRow(slopeRows[curY + i], -1);
		}

		curY += y;

		if (curY >= slopeRows.size())
		{
			break;
		}

		int rowX = curX % slopeRows[curY].size();
		
		// Print current row
		printRow(slopeRows[curY], rowX);

		char curLoc = slopeRows[curY][rowX];

		if (curLoc == '#')
		{
			// Hit tree
			treesHit++;
		}

	}

	DEBUG << "Trees hit on current slope = " << treesHit << std::endl;

	return treesHit;
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> slopeRows = readFile(argv[1]);

	for(auto &  curRow: slopeRows)
	{
		DEBUG << " *** " << curRow << std::endl;
	}

	uint64_t a = navSlope(slopeRows, 1, 1);
	uint64_t b = navSlope(slopeRows, 3, 1);
	uint64_t c = navSlope(slopeRows, 5, 1);
	uint64_t d = navSlope(slopeRows, 7, 1);
	uint64_t e = navSlope(slopeRows, 1, 2);

	uint64_t total = a * b * c * d * e;

	std::cout << "Trees Hit Pt 1= " << b << std::endl;

	DEBUG << "Part 2 : " << a << ", " << b << ", " << c << ", " << d << ", " << e << std::endl;

	std::cout << "Pt 2 Sol = " << total << std::endl;

	return 0;
}
