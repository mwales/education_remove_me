#include <iostream>
#include <vector>

std::vector<std::string> split(std::string data, std::string delimeter)
{
	std::vector<std::string> retVal;
	int pos= 0;
	while(true)
	{
		int delimPos = data.find(delimeter, pos);

		if (delimPos == std::string::npos)
		{
			// Add the last token to the list and return
			// std::cerr << "Token = " << data.substr(pos) << std::endl << std::endl;
			retVal.push_back(data.substr(pos));
			return retVal;
		}
		else
		{
			// We found a token and delimter
			// std::cerr << "Token = " << data.substr(pos, delimPos - pos) << std::endl;
			retVal.push_back(data.substr(pos, delimPos - pos));
			pos = delimPos + 1;
		}
	}

}

std::vector<int> strListToInt(std::vector<std::string> const & data)
{
	std::vector<int> retVal;
	for(auto di = data.begin(); di != data.end(); di++)
	{
		int val = atoi(di->c_str());
		// std::cerr << "Converting item " << *di << " to " << val << std::endl;
		retVal.push_back(val);
	}

	return retVal;
}

void printIntList(std::vector<int> const & data)
{
	if (data.size() == 0)
	{
		std::cout << std::endl;
		return;
	}
	
	auto di = data.begin();
        while(true)
	{
		std::cout << *di;

		di++;
		if (di == data.end())
		{
			std::cout << std::endl;
			return;
		}
		else
		{
			std::cout << ", ";
		}
	}
}

std::vector<int> multiplyLists(std::vector<int> a, std::vector<int> b)
{
	std::vector<int> retVal;
	
	int bi = 0;
	for(auto aIt = a.begin(); aIt != a.end(); aIt++)
	{
		 retVal.push_back(*aIt * b[bi]);

		bi++;
		if (bi >= b.size())
		{
			bi = 0;
		}
	}

	return retVal;
}

int multipleAndSumLists(std::vector<int> a, std::vector<int> b)
{
	std::vector<int> intermediate = multiplyLists(a, b);

	// std::cout << "intermediate: ";
	// printIntList(intermediate);

	int retVal = 0;
	for(auto it = intermediate.begin(); it != intermediate.end(); it++)
	{
		retVal += *it;
	}

	return retVal;
}



std::vector<int> createSequence(int numDupes)
{
	std::vector<int> baseSeq = { 0, 1, 0, -1 };
	std::vector<int> retVal;

	for(auto seqIt = baseSeq.begin(); seqIt != baseSeq.end(); seqIt++)
	{

		for(int i = 0; i <= numDupes; i++)
		{
			retVal.push_back(*seqIt);
		}
	}

	if (numDupes > 0)
	{
		// Eliminate the first number in the list
		retVal.erase(retVal.begin());
		return retVal;
	}
	else
	{
		return std::vector<int> { 1, 0, -1, 0 };
	}
}

std::vector<int> stringToSequence(std::string val)
{
	std::vector<int> retVal;
	for(auto it = val.begin(); it != val.end(); it++)
	{
		retVal.push_back(*it - '0');
	}

	return retVal;
}

std::vector<int> fftPhase(std::vector<int> s)
{
	std::vector<int> retVal;

	for(int digitNum = 0; digitNum < s.size(); digitNum++)
	{
		std::vector<int> genSeq = createSequence(digitNum);
		int curVal = multipleAndSumLists(s, genSeq);
		retVal.push_back(abs(curVal % 10));
	}

	return retVal;
}





int main(int argc, char** argv)
{
	for(int i = 0; i < 6; i++)
	{
		std::vector<int> s = createSequence(i);	
		
		std::cout << "Seq " << i << "     ";
		printIntList(s);
	
	
	}


	int numPhases;
	std::string userSequence;

	std::cin >> numPhases;
	std::cin >> userSequence;

	int numDigitsInSeq = userSequence.size();

	std::cout << "Phases=" << numPhases << "\tSeq=" << userSequence << "\tLen=" 
		  << numDigitsInSeq << std::endl;

	std::vector<int> s = stringToSequence(userSequence);
	printIntList(s);

	std::vector<int> workingData = s;
	for(int phNum = 0; phNum < numPhases; phNum++)
	{
		s = fftPhase(s);

		std::cout << "Phase " << phNum << "\t\t";
		printIntList(s);
	}
}
