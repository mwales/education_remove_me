/**
 * I somehow decided it would be a good idea to only read a single line of text
 * from file and do the problem a single time when I did part 1 the first time.
 * I quickly realized I wanted to be able to run all of the test sets quickly,
 * and had to change it back to reading all lines of file, and running the solution
 * on each line...
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

void addNextNumber(std::vector<int> & numberList)
{
	int lastNumber = *numberList.rbegin();

	int lastOccur = findLastOccurence(numberList, lastNumber);
	if (lastOccur == -1)
	{
		// It never occured before
		DEBUG << "Last number " << lastNumber << " never occured, adding 0" << std::endl;
		numberList.push_back(0);
	}
	else
	{
		DEBUG << "Last number " << lastNumber << " occured " << lastOccur << " rounds ago" << std::endl;
		numberList.push_back(lastOccur);
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

		std::vector<int> numberList;
		for(auto curNumStr: startNumStrs)
		{
			numberList.push_back(atoi(curNumStr.c_str()));
		}

		DEBUG << "START: ";
		debugPrintVector(numberList);
		DEBUG << std::endl;

		int endingNumber = atoi(argv[2]);

		while(numberList.size() <= endingNumber)
		{
			addNextNumber(numberList);

			DEBUG << "List Now: ";
			debugPrintVector(numberList);
			DEBUG << std::endl;
		}

		std::cout << endingNumber << "th number = " << numberList[endingNumber - 1] << std::endl;
	}

	infile.close();

	

	return 0;
}
