#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <utility>

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

template<typename keyType, typename valueType>
void printMap(std::map<keyType, valueType> m)
{
	DEBUG << "{";
	bool isFirst = true;
	for(auto singleItem: m)
	{
		if (!isFirst)
		{
			DEBUG << ", ";
		}
		else
		{
			isFirst = false;
		}

		DEBUG << singleItem.first << " -> " << singleItem.second;
	}

	DEBUG << "}";
}

template<typename keyType, typename valueType>
std::map<keyType, valueType> mergeMaps(std::map<keyType, valueType> m1,
		                       std::map<keyType, valueType> m2)
{
	std::map<keyType, valueType> retVal;

	for(auto curitem: m1)
	{
		if (retVal.find(curitem.first) == retVal.end())
		{
			// Not in list yet
			retVal[curitem.first] = curitem.second;
		}
		else
		{
			retVal[curitem.first] += curitem.second;
		}
	}

	for(auto curitem: m2)
	{
		if (retVal.find(curitem.first) == retVal.end())
		{
			// Not in list yet
			retVal[curitem.first] = curitem.second;
		}
		else
		{
			retVal[curitem.first] += curitem.second;
		}
	}

	return retVal;
}

template<typename keyType, typename valueType>
std::map<keyType, valueType> multiplyMap(std::map<keyType, valueType> m1, int val)
{
	std::map<keyType, valueType> retVal;

	for(auto curitem: m1)
	{
		retVal[curitem.first] = curitem.second * val;
	}
	return retVal;
}

template<typename keyType, typename valueType>
valueType sumMap(std::map<keyType, valueType> m1)
{
	valueType retVal = 0;

	for(auto curitem: m1)
	{
		retVal += curitem.second;
	}
	return retVal;
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

typedef std::pair<std::string, int> BagData;

class BagRule
{
	public:
		
	BagRule(std::string);

	void printBagData();

	std::vector<BagData> theContents;	
	std::string theBagColor;

};

BagRule::BagRule(std::string text)
{
	DEBUG << "BagRule(" << text << ")" << std::endl;

	std::vector<std::string> parts = stringSplit(text, ' ');

	theBagColor = parts[0] + " " + parts[1];

	// Term 4 is qty, 5 plus 6 is color, add 4 till next rule
	for(int i = 4; i < parts.size(); i += 4)
	{
		if (parts[i] == "no")
		{
			// There are no more bag rules
			break;
		}
		else
		{
			int qty = atoi(parts[i].c_str());
			std::string bagcolor = parts[i+1] + " " + parts[i + 2];

			theContents.push_back(std::make_pair(bagcolor, qty));
		}
	}
}

void BagRule::printBagData()
{
	DEBUG << theBagColor << " = ";
	for(auto c: theContents)
	{
		DEBUG << c.first << "(" << c.second << "), ";

	}

	DEBUG << std::endl;
}

class BagTreeNode
{
	public:

	BagTreeNode(BagRule* rules, std::string rootColor);

	std::map<std::string, int> dumpContents();

	void fillTree(std::map<std::string, BagTreeNode*> subtrees);


	std::string theColor;

	BagRule* theRules;

	std::map<BagTreeNode*, int> theChildren;
};

BagTreeNode::BagTreeNode(BagRule* rules, std::string rootColor)
{
	theColor = rootColor;
	theRules = rules;
}

void BagTreeNode::fillTree(std::map<std::string, BagTreeNode*> subtrees)
{
	for(auto childFromRules: theRules->theContents)
	{
		BagTreeNode* btn = subtrees[childFromRules.first];
		int qty = childFromRules.second;

		DEBUG << "filling tree for " << theColor << " with " << qty << " " << childFromRules.first << std::endl;
		theChildren[btn] = qty;
	}
}

std::map<std::string, int> BagTreeNode::dumpContents()
{
	std::map<std::string, int> retVal;
	DEBUG << "dumpContents of " << theColor << std::endl;

	for(auto curChild: theChildren)
	{
		std::map<std::string, int> childrenTimesQty = multiplyMap(curChild.first->dumpContents(), curChild.second);
		retVal = mergeMaps(retVal, childrenTimesQty);
	}

	std::map<std::string, int> curBag;
	curBag[theColor] = 1;

	retVal = mergeMaps(retVal, curBag);
	return retVal;
}
	

int main(int argc, char** argv)
{
	std::vector<BagRule*> rules;

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

		BagRule* bd = new BagRule(text);
		bd->printBagData();
		rules.push_back(bd);

	}

	// I did some weird stuff here because I wasn't sure if I was constructing one well
	// defined tree, or a bunch of trees that weren't connected at all.  So I kinda
	// made the initialization into a two step process.  It worked, but kinda horrible.

	// Create the trees
	std::map<std::string, BagTreeNode*> allBagTreeNodes;
	for(auto curRule: rules)
	{
		BagTreeNode* btn = new BagTreeNode(curRule, curRule->theBagColor);
		allBagTreeNodes[curRule->theBagColor] = btn;
		DEBUG << "Created a bag tree node for: " << curRule->theBagColor << std::endl;
	}

	for(auto curBtn: allBagTreeNodes)
	{
		(curBtn.second)->fillTree(allBagTreeNodes);
	}

	int numBagTypes = 0;
	for(auto curBtn: allBagTreeNodes)
	{
		std::map<std::string, int> contents = curBtn.second->dumpContents();

		if (contents.find("shiny gold") != contents.end())
		{
			numBagTypes += 1;
		}
	}

	std::cout << "Num Bag Types containing gold shiny " << numBagTypes - 1 << std::endl;

	std::map<std::string, int> results;
	results = allBagTreeNodes["shiny gold"]->dumpContents();
	
	DEBUG << "Results: ";
	printMap(results);
	DEBUG << std::endl;

	DEBUG << "Number of bag types in results: " << results.size() << std::endl;

	std::cout << "Number of bags in shiny bag (pt 2): " << sumMap(results) - 1 << std::endl;
	return 0;
}
