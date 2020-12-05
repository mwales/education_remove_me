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

int getSeatId(std::string pass)
{
	DEBUG << "Before: " << pass << std::endl;

	std::string after = replaceChar(pass, 'B', '1');
	after = replaceChar(after, 'F', '0');
	after = replaceChar(after, 'R', '1');
	after = replaceChar(after, 'L', '0');

	unsigned long seatId = strtoul(after.c_str(), NULL, 2);



	DEBUG << "After: " << after << " = " << seatId << std::endl;
	return seatId;
}

int main(int argc, char** argv)
{

	int maxSeatId = 0;

	std::map<int, bool> seatsTaken;
	for(int i = 0; i < 1024; i++)
	{
		seatsTaken[i] = false;
	}

	while(1)
	{
		std::string text;
		std::getline(std::cin,text);

		int sid = getSeatId(text);
		seatsTaken[sid] = true;

		if (sid > maxSeatId)
		{
			maxSeatId = sid;
		}

		// out of output
check_for_eof:
		if (std::cin.eof())
		{
			break;
		}


	}

	
	std::cout << "Max seat ID = " << maxSeatId << std::endl;

	bool seatPrevEmpty = false;
	int prevEmpty = 0;
	for(int i = 0; i < 1024; i++)
	{
		if (seatsTaken[i] == false)
		{
			//DEBUG << "Seat " << i << " empty" << std::endl;

			if (!seatPrevEmpty)
			{
				prevEmpty = i;
				seatPrevEmpty = true;
			}
		}
		else
		{
			if (seatPrevEmpty)
			{
				std::cout << "Empty section = " << prevEmpty << " - " << i - 1 << std::endl;
				seatPrevEmpty = false;
			}
		}
	}


	std::cout << "Empty section = " << prevEmpty << " - " << 1024 - 1 << std::endl;

	return 0;
}
