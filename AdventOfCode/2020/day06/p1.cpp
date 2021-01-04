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


int processGroup(std::vector<std::string> const & people)
{
	std::set<char> totalQs;

	for(auto singlePerson: people)
	{
		for(auto singleQ: singlePerson)
		{
			totalQs.emplace(singleQ);
		}
	}

	return totalQs.size();
}

int processGroupPt2(std::vector<std::string> const & people)
{
	std::map<char, int> totalQs;

	for(auto singlePerson: people)
	{
		for(auto singleQ: singlePerson)
		{
			if (totalQs.find(singleQ) == totalQs.end())
			{
				DEBUG << "First person to ans question Q " << singleQ << std::endl;
				totalQs[singleQ] = 0;
			}
			
			DEBUG << "Adding " << singleQ << " to our existing " << totalQs[singleQ] << std::endl;
			totalQs[singleQ] += 1;
		}
	}

	int numPeople = people.size();
	int commonYesQs = 0;
	for(auto singleQ: totalQs)
	{
		DEBUG << "For Q " << singleQ.first << " we had " << singleQ.second << " Ys, needed " << numPeople << std::endl;
		if (singleQ.second == numPeople)
		{
			commonYesQs++;
		}
	}

	return commonYesQs;
}


int main(int argc, char** argv)
{
	std::vector<std::vector<std::string> > groups;

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	int totalQs = 0;
	int totalYs = 0;

	std::vector<std::string> currentGroup;
	for(auto text: fileData)
	{
		if (text == "")
		{
			groups.push_back(currentGroup);
			currentGroup.clear();
		}
		else
		{
			currentGroup.push_back(text);
		}
	}

	if (currentGroup.size() != 0)
	{
		groups.push_back(currentGroup);
	}

	for(auto curGroup: groups)
	{
		DEBUG << "End of group" << std::endl;
		int numQs = processGroup(curGroup);

		DEBUG << "Group answered " << numQs << " questions" << std::endl;

		totalQs += numQs;

		int numYs = processGroupPt2(curGroup);

		DEBUG << "Group Ys " << numYs << std::endl;
		totalYs += numYs;
	}

	std::cout << "Totals Qs " << totalQs << std::endl;	
	std::cout << "Totals Ys " << totalYs << std::endl;	

	return 0;
}
