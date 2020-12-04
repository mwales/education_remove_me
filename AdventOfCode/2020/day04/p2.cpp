#include <iostream>
#include <vector>
#include <map>

//#define AOC_DEBUG 1
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

bool validateField(std::string field, std::string value)
{
	int valueLen = value.size();
	int valueInt = atoi(value.c_str());
	int i;

	if (field == "byr")
	{
		if (valueLen != 4)
			goto invalid_field;

		if ( (valueInt < 1920) || (valueInt > 2002) )
			goto invalid_field;

		return true;
	}

	if (field == "iyr")
	{
		if (valueLen != 4)
			goto invalid_field;

		if ( (valueInt < 2010) || (valueInt > 2020) )
			goto invalid_field;

		return true;
	}

	if (field == "eyr")
	{
		if (valueLen != 4)
			goto invalid_field;

		if ( (valueInt < 2020) || (valueInt > 2030) )
			goto invalid_field;

		return true;
	}

	if (field == "hgt")
	{
		if ( (value[valueLen - 2] == 'c') && (value[valueLen - 1] == 'm') )
		{
			 if ( (valueInt >= 150) && (valueInt <= 193) )
				 return true;
		}
		
		if ( (value[valueLen - 2] == 'i') && (value[valueLen - 1] == 'n') )
		{
			 if ( (valueInt >= 59) && (valueInt <= 76) )
				 return true;
		}


	}

	if (field == "ecl")
	{
		if ( (value == "amb") || (value == "blu") || (value == "brn") ||
		     (value == "gry") || (value == "grn") || (value == "hzl") || (value == "oth") )
		{
			return true;
		}
	}

	if (field == "hcl")
	{
		if (valueLen != 7)
			goto invalid_field;

		if (value[0] != '#')
			goto invalid_field;

		for(i = 1; i < 7; i++)
		{
			if ( (value[i] >= '0') && (value[i] <= '9') )
				continue;

			if ( (value[i] >= 'a') && (value[i] <= 'f') )
				continue;

			goto invalid_field;
		}

		return true;
	}

	if (field == "cid")
	{
		return true;
	}

	if (field == "pid")
	{
		if (valueLen != 9)
			goto invalid_field;

		for(i = 0; i < 9; i++)
		{
			if ( (value[i] >= '0') && (value[i] <= '9') )
				continue;

			goto invalid_field;
		}

			return true;
	}

	DEBUG << "Fell through all cases on field " << field << " = " << value << std::endl;
	return false;

invalid_field:
	DEBUG << "Invalid field of passport: " << field << " = " << value << std::endl;
	return false;
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
		// DEBUG << "Term=" << curField << std::endl;

		std::vector<std::string> pairs = stringSplit(curField, ':');

		if (reqField.find(pairs[0]) != reqField.end())
		{
			reqField[pairs[0]] = 1;

			if (!validateField(pairs[0], pairs[1]))
			{
				DEBUG << "Invalid passport due to validation" << std::endl;
				return 0;
			}
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

		// std::cout << text << std::endl;


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
