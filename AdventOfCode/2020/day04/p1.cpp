#include <iostream>
#include <vector>
#include <map>

#define AOC_DEBUG 1
#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif



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

	std::vector<std::string> passportFields;
	int numValid = 0;

	while(1)
	{
		std::string text;
		std::getline(std::cin,text);

		std::vector<std::string> terms = stringSplit(text, ' ');
		
		if (text == "")
		{
			DEBUG << "End of passport detected" << std::endl;
			numValid += isValidPassport(passportFields);
			passportFields.clear();
			goto check_for_eof;
		}


		passportFields.insert(passportFields.end(), terms.begin(), terms.end());

		std::cout << text << std::endl;


		// out of output
check_for_eof:
		if (std::cin.eof())
		{
			break;
		}


	}

	std::cout << "Num Valid Passports = " << numValid << std::endl;

	return 0;
}
