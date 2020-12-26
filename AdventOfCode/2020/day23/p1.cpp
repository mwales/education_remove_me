#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

typedef struct CupStruct Cup;

struct CupStruct
{
	Cup* prev;
	Cup* next;
	int value;
};

void printCupList(Cup* start)
{
	Cup* curCup = start;
	int i = 0;
	do
	{
		DEBUG << curCup->value << " ";
		curCup = curCup->next;
		i++;
	} while( (curCup != start) && (i < 12) );

	DEBUG << std::endl;
}

Cup* unlink3After(Cup* origRing)
{
	Cup* retVal = origRing->next;
	Cup* origLinkEnd = origRing;
	Cup* retEndLink = retVal->next->next;
	Cup* origLinkRestart = retEndLink->next;

	retVal->prev = retEndLink;
	retEndLink->next = retVal;

	origLinkEnd->next = origLinkRestart;
	origLinkRestart->prev = origLinkEnd;

	return retVal;
}

Cup* findCupWithValue(Cup* list, int val, std::vector<Cup*> lookupMap)
{
	DEBUG << "Searching for cup with val " << val << std::endl;
	return lookupMap[val];
}

Cup* findSmallerCup(Cup* curCup, Cup* unlinkedList, int val, int numCups, std::vector<Cup*> lookupMap)
{
	val--;
	DEBUG << "Should we search for val " << val << std::endl;
	if (val == 0)
	{
		val = numCups;
	}

	Cup* startCup = curCup;

	while(true)
	{
		if ( (unlinkedList->value != val) &&
		     (unlinkedList->next->value != val) &&
		     (unlinkedList->next->next->value != val) )
		{
			// Find that cup
			return findCupWithValue(curCup, val, lookupMap);
		}
		else
		{
			val--;
			DEBUG << "Should we search for val " << val << std::endl;

			if (val == 0)
			{
				val = numCups;
			}
		}
	}

}

void insertCupList(Cup* insertAfter, Cup* cupToInsert)
{
	Cup* oldNext = insertAfter->next;
	Cup* insertEnd = cupToInsert->prev;

	insertAfter->next = cupToInsert;
	cupToInsert->prev = insertAfter;

	oldNext->prev = insertEnd;
	insertEnd->next = oldNext;
}

void printPart1Solution(Cup* list, std::vector<Cup*> lookupMap)
{
	Cup* oneCup = findCupWithValue(list,1, lookupMap);

	Cup* stopHere = oneCup;
	list = list->next;

	while(list != stopHere)
	{
		std::cout << list->value;
		list = list->next;
	}

	std::cout << std::endl;
}

void printPart2Solution(Cup* curCup)
{
	while(true)
	{
		if (curCup->value != 1)
		{
			curCup = curCup->next;
		}
		else
		{
			break;
		}
	}

	curCup = curCup->next;
	uint64_t firstNum = curCup->value;
	uint64_t secondNum = curCup->next->value;

	uint64_t product = firstNum * secondNum;

	std::cout << "Part 2: " << firstNum << " x " << secondNum << " = "
	          << product << std::endl;
}


int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide iterations listSize seed" << std::endl;
		return 0;
	}

	std::string input = "389125467";

	if (argc == 4)
	{
		input = argv[3];
	}

	int num_iterations = atoi(argv[1]);
	DEBUG << "Num iter = " << num_iterations << std::endl;

	std::vector<int> cupVec;

	for(auto singleDigit: input)
	{
		cupVec.push_back((int) singleDigit - '0');
	}

	DEBUG << "CupVec: " << cupVec << std::endl;

	int numCups = cupVec.size();

	const int NUM_CUPS = atoi(argv[2]);
	Cup* mallocCup = (Cup*) malloc(NUM_CUPS * sizeof(Cup));

	DEBUG << "malloc complete" << std::endl;

	if (mallocCup == 0)
	{
		DEBUG << "malloc return 0" << std::endl;
		return 1;
	}


	Cup* curCup = mallocCup;

	int startFillValue = 0;
	for(auto val: cupVec)
	{
		DEBUG << val << std::endl;
		startFillValue = (startFillValue < val ? val : startFillValue);
	}

	startFillValue++;
	DEBUG << "Will start big list fill at " << startFillValue << std::endl;

	std::vector<Cup*> lookupMap;
	lookupMap.reserve(NUM_CUPS + 1);  // The 0th cup is never used
	for(int i = 0; i <= NUM_CUPS; i++)
	{
		lookupMap.push_back(nullptr);
	}

	for(int i = 0; i < NUM_CUPS; i++)
	{
		if (i == 0)
		{
			mallocCup[0].prev = &mallocCup[NUM_CUPS-1];
		}
		else
		{
			mallocCup[i].prev = &mallocCup[i-1];
		}

		if (i == (NUM_CUPS - 1))
		{
			mallocCup[NUM_CUPS - 1].next = &mallocCup[0];
		}
		else
		{
			mallocCup[i].next = &mallocCup[i+1];
		}

		if (i >= numCups)
		{
			lookupMap[startFillValue] = &mallocCup[i];
			mallocCup[i].value = startFillValue++;
		}
		else
		{
			lookupMap[cupVec[i]] = &mallocCup[i];
			mallocCup[i].value = cupVec[i];
		}
	}

	printCupList(mallocCup);

	int logCount = 0;
	for(int iter = 1; iter <= num_iterations; iter++)
	{
		DEBUG << "-- move " << iter << " --" << std::endl;

		DEBUG << "Cups: ";
		printCupList(curCup);

		int insertPosVal = curCup->value;

		Cup* unlinkedList = unlink3After(curCup);

		DEBUG << "Unlinked: ";
		printCupList(unlinkedList);

		DEBUG << "Whats left: ";
		printCupList(curCup);

		Cup* insertCup = findSmallerCup(curCup, unlinkedList, insertPosVal, NUM_CUPS, lookupMap);
		DEBUG << "Destination: " << insertCup->value << std::endl;

		insertCupList(insertCup, unlinkedList);

		curCup = curCup->next;
		DEBUG << std::endl;

		if (++logCount == 50000)
		{
			logCount = 0;
			std::cout << "Up to iteration " << iter << std::endl;
		}
	}

	DEBUG << "Final:";
	printCupList(curCup);

	DEBUG << "Part1: ";
	printPart1Solution(curCup, lookupMap);
	
	printPart2Solution(curCup);

	//printEnding(curCup);

	return 0;
}
