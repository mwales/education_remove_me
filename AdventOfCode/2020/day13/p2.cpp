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

int64_t distToNextMultiple(int64_t curVal, std::vector<int64_t> const & list, int64_t& multipleOf)
{
	int64_t smallestMultiple = 0xffffffffffffffff;
	multipleOf = 0;

	for(auto listItem: list)
	{
		int64_t modulusResult = curVal % listItem;
		int64_t distToNextMult = (listItem - modulusResult) % listItem;

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

// Start2 needs to be > start1
int64_t findMeeting(int64_t start1, int64_t interval1, int64_t endingOffset1,
                     int64_t start2, int64_t interval2, int64_t endingOffset2,
		     int64_t & futureStepValue)
{
	int64_t counter1 = interval1 + start1; // + start1;
	int64_t counter2 = interval2 + start2; // + start1;
	//int64_t startDiff = start2 - start1;
	//counter2 -= startDiff;

	DEBUG << "Bus " << interval1 << " starts at " << start1 << " with end offset "
	      << endingOffset1 << std::endl;
	DEBUG << "Bus " << interval2 << " starts at " << start2 << " with end offset "
	      << endingOffset2 << std::endl;

	int64_t diff = (counter1 + endingOffset1) - (counter2 + endingOffset2);
	while( diff != 0 )
	{
		diff = (counter1 + endingOffset1) - (counter2 + endingOffset2);
		if (diff > 0)
		{
			DEBUG << "1. Bus " << interval2 << " next departure at "
			      << counter2 << std::endl;

			counter2 += interval2;
		}
		else
		{
			DEBUG << "1. Bus " << interval1 << " next departure at "
			      << counter1 << std::endl;
			
			counter1 += interval1;
		}
	}

	DEBUG << "meeting at " << counter1 << std::endl;

	int64_t retVal = counter1;
	counter1 += interval1;

	diff = (counter1 + endingOffset1) - (counter2 + endingOffset2);
	while(diff != 0)
	{
		diff = (counter1 + endingOffset1) - (counter2 + endingOffset2);

		if (diff > 0)
		{
			DEBUG << "2. Bus " << interval2 << " next departure at "
			      << counter2 << std::endl;
			counter2 += interval2;
		}
		else
		{
			DEBUG << "2. Bus " << interval1 << " next departure at "
			      << counter1 << std::endl;

			counter1 += interval1;
		}
	}

   futureStepValue = counter1 - retVal;
   DEBUG << "Future step value: " << futureStepValue << std::endl;
   //stepValue = counter1 - retVal;
   //DEBUG << "Step value: " << stepValue << std::endl;

   return retVal;

}

int64_t  findCommonDeparture(std::vector<std::string> const & busTimes)
{
	int st = 0;
	std::vector<int> startTimes;
	std::vector<int> intervals;

	for(auto curBus: busTimes)
	{
		if (curBus != "x")
		{
			intervals.push_back(atoi(curBus.c_str()));
			startTimes.push_back(st);
		}
		st++;
	}

	st = startTimes[0];
	
	// starttime, interval, endingoffset
	int64_t curInterval = 0;
	st = findMeeting(st, intervals[0], startTimes[0],
                         startTimes[1], intervals[1], startTimes[1], curInterval);

	for(int i = 2; i < startTimes.size(); i++)
	{
		st = findMeeting(st, curInterval, startTimes[0], 
				 startTimes[i], intervals[i], startTimes[i], curInterval);
	}

	return curInterval;
}

int64_t findFirstMeeting(int64_t bus1Interval, int64_t bus1StartTime,
                         int64_t bus2Interval, int64_t bus2StartTime,
			 int64_t& minIntervalAfter)
{
	DEBUG << "findFirstMeeting(" << bus1Interval << "," << bus1StartTime << ","
	      << bus2Interval << "," << bus2StartTime << std::endl;

	int64_t bus1 = bus1StartTime + bus1Interval;
	int64_t retVal;

	while(true)
	{
		int64_t diff = (bus1 + bus2StartTime) % bus2Interval;
		DEBUG << "bus1=" << bus1 << ", diff=" << diff << std::endl;

		if (diff)
		{
			bus1 += bus1Interval;
		}
		else
		{
			// WE wound it!
			DEBUG << "Found the meeting: " << bus1 << std::endl;
			retVal = bus1;
			bus1 += bus1Interval;
			break;
		}
	}

	while(true)
	{
		int64_t diff = (bus1 + bus2StartTime) % bus2Interval;
		DEBUG << "bus1=" << bus1 << ", diff=" << diff << std::endl;

		if (diff)
		{
			bus1 += bus1Interval;
		}
		else
		{
			// WE wound it!
			DEBUG << "Found the 2nd meeting: " << bus1 << std::endl;
			minIntervalAfter = bus1 - retVal;
			DEBUG << "Interval = " << minIntervalAfter << std::endl;
			bus1 += bus1Interval;
			break;
		}
	}
	
	/*
	while(true)
	{
		int64_t diff = (bus1 + bus2StartTime) % bus2Interval;
		DEBUG << "bus1=" << bus1 << ", diff=" << diff << std::endl;

		if (diff)
		{
			bus1 += bus1Interval;
		}
		else
		{
			// WE wound it!
			DEBUG << "Found the 3rd meeting: " << bus1 << std::endl;
			DEBUG << "Interval x 2 + retVal = " << 2 * minIntervalAfter + retVal << std::endl;
			return retVal;
		}
	}
	*/

	return retVal;
}


int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> inputData = readFile(argv[1]);
	
	
	// First line in file not used	
	std::vector<std::string> buses = stringSplit(inputData[1], ',');

	std::vector<int64_t> busTimes;
	std::vector<int64_t> firstTimes;

	for(int i = 0; i < buses.size(); i++)
	{
		if (buses[i] != "x")
		{
			int64_t bt = strtoull(buses[i].c_str(), NULL, 10);
			busTimes.push_back(bt);
			firstTimes.push_back(i);
		}
	}

	int64_t minInterval = busTimes[0];
	int64_t initialStartTime = firstTimes[0];
	
	for(int i = 1; i < busTimes.size(); i++)
	{
		int64_t newInterval;
		int64_t newSt = findFirstMeeting(minInterval, initialStartTime,
				                 busTimes[i], firstTimes[i],
		                                 newInterval);

		minInterval = newInterval;
		initialStartTime = newSt;
	}

	std::cout << "Part 2: " << initialStartTime << std::endl;
	
	return 0;
}
