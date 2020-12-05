#include <iostream>
#include <vector>

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

void processLine(std::string const & text)
{
	int i = 0;
	bool foundBasement = false;
	int stepCount = 0;

	for(auto singleChar: text)
	{
		stepCount++;
		
		if (singleChar == '(')
		{
			i++;
		}
		else
		{
			i--;
		}

		if ( (i == -1) && !foundBasement)
		{
			std::cout << "Found basementat step: " << stepCount << std::endl;
			foundBasement = true;
		}

	}

	std::cout << "Value = " << i << std::endl;
}

int main(int argc, char** argv)
{
	
	while(1)
	{
		std::string text;
		std::cin >> text;

		// out of output
		if (std::cin.eof())
		{
			break;
		}

		processLine(text);
	}


	return 0;
}
