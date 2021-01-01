#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

//#define AOC_DEBUG 1
#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

int getSeatId(std::string pass)
{
	DEBUG << "Before: " << pass << std::endl;

	std::string after = replaceChar(pass, 'B', '1');
	after = replaceChar(after, 'F', '0');
	after = replaceChar(after, 'R', '1');
	after = replaceChar(after, 'L', '0');

	unsigned long seatId = strtoul(after.c_str(), NULL, 2);

	DEBUG << "After: " << after << " = " << seatId << std::endl;
	return seatId;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	int maxSeatId = 0;

	std::map<int, bool> seatsTaken;
	for(int i = 0; i < 1024; i++)
	{
		seatsTaken[i] = false;
	}

	for(auto curLine: fileData)
	{

		int sid = getSeatId(curLine);
		seatsTaken[sid] = true;

		if (sid > maxSeatId)
		{
			maxSeatId = sid;
		}

	}

	
	std::cout << "Max seat ID = " << maxSeatId << std::endl;

	bool seatPrevEmpty = false;
	int prevEmpty = 0;
	for(int i = 0; i < 1024; i++)
	{
		if (seatsTaken[i] == false)
		{
			//DEBUG << "Seat " << i << " empty" << std::endl;

			if (!seatPrevEmpty)
			{
				prevEmpty = i;
				seatPrevEmpty = true;
			}
		}
		else
		{
			if (seatPrevEmpty)
			{
				std::cout << "Empty section = " << prevEmpty << " - " << i - 1 << std::endl;
				seatPrevEmpty = false;
			}
		}
	}


	std::cout << "Empty section = " << prevEmpty << " - " << 1024 - 1 << std::endl;

	return 0;
}
