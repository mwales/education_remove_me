#include <iostream>
#include <vector>

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

bool checkPass(int min, int max, char letter, std::string const & pass)
{
	int qty = 0;
	for(auto singleChar = pass.begin(); singleChar != pass.end(); singleChar++)
	{
		if (*singleChar == letter)
		{
			qty++;
		}
	}

	return ( (qty >= min) && (qty <= max) );
}

bool checkPass2(int x, int y, char letter, std::string const & pass)
{
	int numPosWithLetter = 0;
	
	if ( (x > pass.size()) || (y > pass.size()) )
	{
		DEBUG << "Check2 size check fail" << std::endl;
		return false;
	}

	if (pass[x-1] == letter)
	{
		DEBUG << "Check2 Pos 1 has letter!" << std::endl;
		numPosWithLetter++;
	}

	if (pass[y-1] == letter)
	{
		DEBUG << "Check2 Pos 2 has letter!" << std::endl;
		numPosWithLetter++;
	}

	return (numPosWithLetter == 1);
}

bool verify(std::string const & rule, std::string const & letter, std::string const & password, bool & part2Ans)
{

	DEBUG << "R=" << rule << ", L=" << letter << ", P=" << password << std::endl;

	std::vector<std::string> parts = stringSplit(rule, '-');

	DEBUG << "Num parts = " << parts.size() << std::endl;

	if (parts.size() != 2)
	{
		std::cerr << "Rule " << rule << " invalid" << std::endl;
		return false;
	}

	DEBUG << "Lengts of each parts are " << parts[0].size() << " and " << parts[1].size() << std::endl;
	if ( (parts[0].size() < 1) || (parts[1].size() < 1) )
	{
		std::cerr << "Rule " << rule << " invalid (size of parts)" << std::endl;
		return false;
	}

	int min = atoi(parts[0].c_str());
	int max = atoi(parts[1].c_str());

	DEBUG << "Min = " << min << ", max = " << max << std::endl;

	if (letter.size() < 1)
	{
		std::cerr << "Letter " << letter << " invalid" << std::endl;
		return false;
	}

	char singleLetter = letter[0];

	part2Ans = checkPass2(min, max, singleLetter, password);

	return checkPass(min, max, singleLetter, password);
}


int main(int argc, char** argv)
{

	int qtyGood = 0;
	int qtyGoodPt2 = 0;
	
	while(1)
	{
		std::string rule;
		std::string letter;
		std::string password;

		std::cin >> rule;
		std::cin >> letter;
		std::cin >> password;

		if (std::cin.eof())
		{
			break;
		}

		bool part2;
		bool valid = verify(rule, letter, password, part2);
		
		DEBUG << "Valid = " << (valid ? "valid" : "bad" ) << std::endl;

		if (valid)
		{
			qtyGood++;
		}

		if (part2)
		{
			qtyGoodPt2++;
		}
	}

	std::cout << "Num valid passwords = " << qtyGood << std::endl;

	std::cout << "Num valid passwords for part 2= " << qtyGoodPt2 << std::endl;

	return 0;
}
