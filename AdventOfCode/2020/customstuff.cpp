#include "customstuff.h"

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <fstream>

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



std::vector<std::string> stringSplit(std::string const & input, std::vector<char> const & delimeters)
{
	std::vector<std::string> retVal;
	std::string curStr;

	for(auto singleChar = input.begin(); singleChar != input.end(); singleChar++)
	{
		if (std::find(delimeters.begin(), delimeters.end(), *singleChar) != delimeters.end())
		{
			if (curStr.size() > 0)
			{
				retVal.push_back(curStr);
				curStr = "";
			}
		}
		else
		{
			curStr += *singleChar;
		}
	}

	if (curStr.size())
	{
		retVal.push_back(curStr);
	}

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

std::vector<std::string> readFile(char* filename)
{
	std::vector<std::string> retVal;
	std::ifstream infile(filename);

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile, text);

		if (infile.eof())
		{
			break;
		}

		retVal.push_back(text);
	}
	
	infile.close();

	return retVal;
}
