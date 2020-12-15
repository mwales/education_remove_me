/**
 * Converted the map I created in p2.cpp into a int array on the heap.  Much faster
 * performance.
 *
 * time ./a2 input.txt 30000000
 * 30000000th number = 1407
 *
 * real    0m0.899s
 */

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <cstring>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif


uint64_t addNextNumber(int* numberList, uint64_t lastNumberAdded, uint64_t & index)
{
	if (numberList[lastNumberAdded] == -1)
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

	int endingNumber = atoi(argv[2]);

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

		int* numberList = (int*) malloc(endingNumber * sizeof(int));
		for(int i = 0; i < endingNumber; i++)
		{
			numberList[i] = -1;
		}


		uint64_t index = 0;
		for(index = 0; index < startNumStrs.size() - 1; index++)
		{
			numberList[atoi(startNumStrs[index].c_str())] = index;

		}


		uint64_t lastNumberAdded = atoi(startNumStrs.rbegin()->c_str());
		while(index < endingNumber-1)
		{
			lastNumberAdded = addNextNumber(numberList, lastNumberAdded, index);
		}

		std::cout << endingNumber << "th number = " << lastNumberAdded << std::endl;
	}

	infile.close();

	

	return 0;
}
