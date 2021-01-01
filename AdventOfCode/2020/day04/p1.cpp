#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

#ifdef AOC_DEBUG
   #define DEBUG std::cout
#else
   #define DEBUG if(0) std::cout
#endif

int isValidPassport(std::vector<std::string> const & termList)
{
	std::map<std::string, int> reqField;
	reqField["byr"] = 0;
	reqField["iyr"] = 0;
	reqField["eyr"] = 0;
	reqField["hgt"] = 0;
	reqField["hcl"] = 0;
	reqField["ecl"] = 0;
	reqField["pid"] = 0;
	//reqField["cid"] = 0;

	for(auto curField: termList)
	{
		DEBUG << "Term=" << curField << std::endl;

		std::vector<std::string> pairs = stringSplit(curField, ':');

		if (reqField.find(pairs[0]) != reqField.end())
		{
			reqField[pairs[0]] = 1;
		}

	}

	for(auto curField : reqField)
	{
		if (curField.second == 0)
		{

			DEBUG << "Missing field " << curField.first << std::endl;
			return 0;
		}
	}

	DEBUG << "Valid passport" << std::endl;
	return 1;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	std::vector<std::vector<std::string> > passportCollection;
	std::vector<std::string> passportFields;

	for(auto curLine: fileData)
	{
		std::vector<std::string> terms = stringSplit(curLine, ' ');
		
		if (curLine == "")
		{
			DEBUG << "End of passport detected" << std::endl;
			passportCollection.push_back(passportFields);
			passportFields.clear();
		}

		passportFields = append(passportFields, terms);

		DEBUG << curLine << std::endl;
	}

	if (passportFields.size())
	{
		   passportCollection.push_back(passportFields);
	}

	int numValid = 0;
	for(auto singlePassport: passportCollection)
	{
		numValid += isValidPassport(singlePassport);
	}

	std::cout << "Num Valid Passports = " << numValid << std::endl;

	return 0;
}
