/**
 * One of my favorite fails.  My solution for part1 wasn't fast enough, so I tried
 * to add a std::map to speed it up.  Reality was, I made it way way way slower:
 *
 * time ./a.out input.txt 30000
 * 30000th number = 6
 *
 * real    0m8.269s
 *
 * time ./a.out input.txt 300000
 * 300000th number = 100
 *
 * real    17m8.831s
 *
 * No way I'm ever going to get to 30million iterations... *
 */

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

int findLastOccurence(std::vector<int> const & numberList, int needle)
{
	int numTimesFound = 0;
	int firstTimeFound, secondTimeFound;
	for(int i = numberList.size() - 1; i >= 0; i--)
	{
		if (needle == numberList[i])
		{
			numTimesFound++;
			if (numTimesFound == 1)
			{
				firstTimeFound = i+1;
			}
			else
			{
				secondTimeFound = i+1;
				int retVal = firstTimeFound - secondTimeFound;
				DEBUG << needle << " found " << firstTimeFound << " - " << secondTimeFound << " = "
				      << retVal << std::endl;
				return retVal;
			}
		}


	}

	return -1;

}

uint64_t addNextNumber(std::map<uint64_t,uint64_t> & numberList, uint64_t lastNumberAdded, uint64_t & index)
{
	if (numberList.find(lastNumberAdded) == numberList.end())
	{
		DEBUG << "Adding 0 to the list (last num " << lastNumberAdded << " not in list)" << std::endl;

		numberList[lastNumberAdded] = index;
		index++;
		return 0;
	}
	else
	{

		uint64_t retVal = index - numberList[lastNumberAdded];
		numberList[lastNumberAdded] = index;

		DEBUG << "Adding " << retVal << " to the list (last num " << lastNumberAdded << ")" << std::endl;

		index++;
		return retVal;
	}
}

int main(int argc, char** argv)
{

	if (argc < 3)
	{
		std::cerr << "Provide filename and iter count" << std::endl;
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

		std::vector<std::string> startNumStrs = stringSplit(text, ',');

		DEBUG << "Len of startNumStrs = " << startNumStrs.size() << " = " << startNumStrs[0] << std::endl;
		debugPrintVector(startNumStrs);
		DEBUG << std::endl;

		std::map<uint64_t, uint64_t> numberList;
		uint64_t index = 0;
		for(index = 0; index < startNumStrs.size() - 1; index++)
		{
			numberList[atoi(startNumStrs[index].c_str())] = index;

		}

		DEBUG << "START: ";
		debugPrintMap(numberList);
		DEBUG << std::endl;

		int endingNumber = atoi(argv[2]);


		uint64_t lastNumberAdded = atoi(startNumStrs.rbegin()->c_str());
		while(index < endingNumber-1)
		{
			lastNumberAdded = addNextNumber(numberList, lastNumberAdded, index);

			DEBUG << "List Now: ";
			debugPrintMap(numberList);
			DEBUG << std::endl;

		}

		std::cout << endingNumber << "th number = " << lastNumberAdded << std::endl;
	}

	infile.close();

	

	return 0;
}
