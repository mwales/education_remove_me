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

uint64_t distToNextMultiple(uint64_t curVal, std::vector<uint64_t> const & list, uint64_t& multipleOf)
{
	uint64_t smallestMultiple = 0xffffffffffffffff;
	multipleOf = 0;

	for(auto listItem: list)
	{
		uint64_t modulusResult = curVal % listItem;
		uint64_t distToNextMult = (listItem - modulusResult) % listItem;

		std::cout << "For " << listItem << ", modulus was " << modulusResult
		          << ", next dist = " << distToNextMult << std::endl;


		if (distToNextMult <= smallestMultiple)
		{
			smallestMultiple = distToNextMult;
			multipleOf = listItem;
		}
	}

	return smallestMultiple;
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);
	std::vector<std::string> inputData;
	
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
		inputData.push_back(text);
	}

	infile.close();

	uint64_t curTime = strtoull(inputData[0].c_str(), NULL, 10);
	std::vector<std::string> buses = stringSplit(inputData[1], ',');

	std::vector<uint64_t> busTimes;
	for(auto curBus: buses)
	{
		if (curBus != "x")
		{
			uint64_t bt = strtoull(curBus.c_str(), NULL, 10);
			busTimes.push_back(bt);
		}
	}
	uint64_t multipleOf;
	uint64_t nextEarliestBusTime = distToNextMultiple(curTime, busTimes, multipleOf);
	std::cout << "Part 1 = next earliest = " << nextEarliestBusTime << std::endl;
	std::cout << "Part 1 = " << multipleOf * nextEarliestBusTime << std::endl;
	
	return 0;
}
