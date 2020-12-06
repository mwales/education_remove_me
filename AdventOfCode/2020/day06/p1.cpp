#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>

//#define AOC_DEBUG 1
#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

template<typename t>
std::vector<t> append(std::vector<t> a, std::vector<t> b)
{
	std::vector<t> retVal;
	retVal = a;
	retVal.insert(retVal.end(), b.begin(), b.end());
	return retVal;
}


std::vector<std::string> stringSplit(std::string const & input, char delimeter)
{
	std::vector<std::string> retVal;
	std::string curStr;

	for(auto singleChar = input.begin(); singleChar != input.end(); singleChar++)
	{
		if (*singleChar == delimeter)
		{
			retVal.push_back(curStr);
			curStr = "";
		}
		else
		{
			curStr += *singleChar;
		}
	}

	retVal.push_back(curStr);

	return retVal;
}

std::string replaceChar(std::string orig, char before, char after)
{
	std::string retVal;
	for(auto singleChar: orig)
	{
		if (singleChar == before)
		{
			retVal += after;
		}
		else
		{
			retVal += singleChar;
		}
	}

	return retVal;
}

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
	std::vector<std::string> peopleInGroup;

	int totalQs = 0;
	int totalYs = 0;

	while(1)
	{
		std::string text;
		std::getline(std::cin,text);

		if (text == "")
		{
			DEBUG << "End of group" << std::endl;
			int numQs = processGroup(peopleInGroup);

			DEBUG << "Group answered " << numQs << " questions" << std::endl;

			totalQs += numQs;

			int numYs = processGroupPt2(peopleInGroup);

			DEBUG << "Group Ys " << numYs << std::endl;
			totalYs += numYs;

			peopleInGroup.clear();
		}
		else
		{
			DEBUG << text << std::endl;
			peopleInGroup.push_back(text);
		}
		// out of output
check_for_eof:
		if (std::cin.eof())
		{
			break;
		}


	}

	std::cout << "Totals Qs " << totalQs << std::endl;	
	std::cout << "Totals Ys " << totalYs << std::endl;	

	return 0;
}
