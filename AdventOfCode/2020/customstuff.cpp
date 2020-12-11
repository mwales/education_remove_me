#include "customstuff.h"

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>


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
