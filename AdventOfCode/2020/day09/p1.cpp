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

template<typename printableType>
void printVector(std::vector<printableType> v)
{
        std::string retVal;
        bool first = true;
        for(auto const & curItem: v)
        {
                if (!first)
                {
                        DEBUG << ",";
                }
                else
                {
                        first = false;
                }

                DEBUG << curItem;
        }
}

template<typename sortableType>
void insertOrdered(std::vector<sortableType>& origList, sortableType x)
{
        for(auto it = origList.begin(); it != origList.end(); it++)
        {
                if (*it > x)
                {
                        origList.insert(it, x);
                        return;
                }
        }

        // If we get here, empty list?
        origList.push_back(x);
}

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

std::set<int> findAllSums(std::vector<int> numList)
{
	std::set<int> retVal;

	for(int i = 0; i < numList.size(); i++)
	{
		for(int j = 0; j < numList.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			retVal.emplace(numList[i] + numList[j]);
		}
	}
	return retVal;
}

void printSet(std::set<int> s)
{
	for(auto item: s)
	{
		DEBUG << item << ", ";
	}
}


int attackCt(std::vector<int> ct, int preambleLen)
{
	for(int i = preambleLen; i < ct.size(); i++)
	{
		std::vector<int> smallList;
		for(int j = i - preambleLen; j < i; j++)
		{
			smallList.push_back(ct[j]);
		};

		DEBUG << "Small List for i = " << i << ": ";
		printVector(smallList);
		DEBUG << std::endl;

		std::set<int> allSums = findAllSums(smallList);
		
		DEBUG << "Set: ";
		printSet(allSums);
		DEBUG << std::endl;


		if (allSums.find(ct[i]) == allSums.end())
		{
			return ct[i];
		}

	}

	return -1;
}

int findWeakness(std::vector<int> const & ct, int attackNum)
{
	int i, j;
	for(i = 0; i < ct.size() - 1; i++)
	{
		DEBUG << "Reseting search for weakness" << std::endl;

		int curSum = 0;
		int curSmallest = ct[i];
		int curLargest = curSmallest;

		for(j = i; j < ct.size(); j++)
		{
			DEBUG << "Adding " << ct[j] << " to the mix, sum = " << curSum << std::endl;

			curSum += ct[j];

			if (ct[j] < curSmallest)
			{
				curSmallest = ct[j];
			}

			if (ct[j] > curLargest)
			{
				curLargest = ct[j];
			}

			if (curSum == attackNum)
			{
				return curSmallest + curLargest;
			}

			if (curSum > attackNum)
				j = ct.size();
		}
	}

	return -1;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Need to provide preamble length" << std::endl;
		return 0;
	}

	std::vector<int> ciphertext;	

	while(1)
	{
		std::string text;
		std::getline(std::cin,text);


		// out of output
check_for_eof:
		if (std::cin.eof())
		{
			break;
		}

		ciphertext.push_back(atoi(text.c_str()));
	}

	printVector(ciphertext);
	DEBUG << std::endl;

	int weakness = attackCt(ciphertext, atoi(argv[1]));
	std::cout << "Weakness = " << weakness << std::endl;

	int weak2 = findWeakness(ciphertext, weakness);

	std::cout << "Wekaness 2 = " << weak2 << std::endl;

	return 0;
}
