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
	do
	{
		DEBUG << curCup->value << " ";
		curCup = curCup->next;
	} while(curCup != start);

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

Cup* findSmallerCup(Cup* curCup, int val)

{
	Cup* startCup = curCup;

	int maxVal = 0;
	Cup* maxCup;

	int underVal = -1;
	Cup* underCup;

	while(true)
	{
		if ( (curCup->value < val) && (curCup->value > underVal) )
		{
			underVal = curCup->value;
			underCup = curCup;
		}

		if (curCup->value > maxVal)
		{
			maxVal = curCup->value;
			maxCup = curCup;
		}

		curCup = curCup->next;

		if (curCup == startCup)
		{
			break;
		}
	}

	if (underVal == -1)
	{
		return maxCup;
	}
	else
	{
		return underCup;
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

void printEnding(Cup* list)
{
	while(list->value != 1)
	{
		list = list->next;
	}

	Cup* stopHere = list;
	list = list->next;

	while(list != stopHere)
	{
		std::cout << list->value;
		list = list->next;
	}

	std::cout << std::endl;
}


int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide iterations" << std::endl;
		return 0;
	}

	std::string input = "389125467";

	if (argc == 3)
	{
		input = argv[2];
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
	Cup* mallocCup = (Cup*) malloc(cupVec.size() * sizeof(Cup));

	Cup* curCup = mallocCup;

	std::vector<Cup*> lookupMap;
	lookupMap.reserve(numCups + 1);


	for(int i = 0; i < numCups; i++)
	{
		if (i == 0)
		{
			mallocCup[0].prev = &mallocCup[numCups-1];
		}
		else
		{
			mallocCup[i].prev = &mallocCup[i-1];
		}

		if (i == (numCups - 1))
		{
			mallocCup[numCups - 1].next = &mallocCup[0];
		}
		else
		{
			mallocCup[i].next = &mallocCup[i+1];
		}

		mallocCup[i].value = cupVec[i];
		lookupMap[cupVec[i]] = callocCup[i];
	}

	printCupList(mallocCup);

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

		Cup* insertCup = findSmallerCup(curCup, insertPosVal);
		DEBUG << "Destination: " << insertCup->value << std::endl;

		insertCupList(insertCup, unlinkedList);

		curCup = curCup->next;
		DEBUG << std::endl;
	}

	DEBUG << "Final:";
	printCupList(curCup);

	printEnding(curCup);

	return 0;
}
