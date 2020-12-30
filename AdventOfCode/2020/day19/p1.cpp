#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <assert.h>
#include <algorithm>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

class Rule;

class RuleDB
{
public:
	void addRule(std::string text);

	Rule* getRule(int num);

	std::map<int, Rule*> theRules;

	void dump();

};


Rule* RuleDB::getRule(int num)
{
	return theRules[num];
}

class Rule
{
public:

	Rule(RuleDB* theDb, int ruleNum);

	virtual bool matchRule(std::string msg, int& matchPos) = 0;

	virtual std::string ruleToString() = 0;

	RuleDB* theDb;

	int theRuleNumber;



};

Rule::Rule(RuleDB* db, int ruleNum):
	theDb(db),
	theRuleNumber(ruleNum)
{
	// nothing
}

class SingleLetterRule: public Rule
{
public:
	SingleLetterRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText);

	virtual bool matchRule(std::string msg, int& matchPos);
	virtual std::string ruleToString();

	char theLetter;
};

SingleLetterRule::SingleLetterRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText):
	Rule(theDb, ruleNum)
{
	DEBUG << "Creating SingleLetterRule: " << ruleText << std::endl;
	assert(ruleText[0].size() == 3);

	theLetter = ruleText[0][1];
}

bool SingleLetterRule::matchRule(std::string msg, int& matchPos)
{
	if (matchPos >= msg.size())
	{
		DEBUG << "SingleLetterRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return false;
	}

	bool retVal = msg[matchPos] == theLetter;

	DEBUG << "SingleLetterRule::matchRule (" << ruleToString() << ") at pos "
	      << matchPos << " matching against " << theLetter << " = "
	      << (retVal ? "success" : "fail" ) << std::endl;

	matchPos++;
	return retVal;
}

std::string SingleLetterRule::ruleToString()
{
	std::string retVal;
	retVal += std::to_string(theRuleNumber);
	retVal += ": \"";
	retVal += theLetter;
	retVal += "\"";

	return retVal;
}

class SubListRule: public Rule
{
public:

	SubListRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText);

	virtual bool matchRule(std::string msg, int& matchPos);
	virtual std::string ruleToString();

	std::vector<int> theList;
};

SubListRule::SubListRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText):
	Rule(theDb, ruleNum)
{
	DEBUG << "Creating SubListRule: " << ruleText << std::endl;

	for(auto eachSub: ruleText)
	{
		int ruleNum = atoi(eachSub.c_str());
		theList.push_back(ruleNum);
	}
}

bool SubListRule::matchRule(std::string msg, int& matchPos)
{
	if (matchPos >= msg.size())
	{
		DEBUG << "SubListRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return false;
	}


	for(auto ruleNum: theList)
	{
		Rule* r = theDb->getRule(ruleNum);
		
		DEBUG << "SubListRule::matchRule (" << ruleToString() << ") going to match pos="
		      << matchPos << " against sub rule " << ruleNum << std::endl;

		if(r->matchRule(msg, matchPos) == false)
		{
			DEBUG << "SubListRule::matchRule (" << ruleToString()
			      << ") going to match pos=" << matchPos
		              << " against sub rule " << ruleNum << " failed" << std::endl;

			return false;
		}
		else
		{
			DEBUG << "SubListRule::matchRule (" << ruleToString()
			      << ") going to match pos=" << matchPos
		              << " against sub rule " << ruleNum << " success" << std::endl;
		}
	}

	DEBUG << "SubListRule::matchRule(" << ruleToString() << ") all subrules success" << std::endl;
	return true;
}

std::string SubListRule::ruleToString()
{
	std::string retVal;
	retVal += std::to_string(theRuleNumber);
	retVal += ": ";

	retVal += toString(theList);
	return retVal;
}



class OrRule: public Rule
{
public:
	OrRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText);

	virtual bool matchRule(std::string msg, int& matchPos);
	virtual std::string ruleToString();

	std::vector<Rule*> theList;
	std::vector<int> theFirstTerms;
	std::vector<int> theSecondTerms;

	bool theLoopingFlag;
};

OrRule::OrRule(RuleDB* theDb, int ruleNum, std::vector<std::string> ruleText):
	Rule(theDb, ruleNum)
{
	DEBUG << "Creating OrRule: " << ruleText << std::endl;

	int pipePos = -1;
	for(int i = 0; i < ruleText.size(); i++)
	{
		if (ruleText[i] == "|")
		{
			pipePos = i;
		}
	}

	assert(pipePos != -1);

	std::vector<std::string> firstPart = subVec(ruleText, 0, pipePos);
	std::vector<std::string> endPart = subVec(ruleText, pipePos + 1);

	DEBUG << "  Or Rule First Parts: " << firstPart << std::endl;
	DEBUG << "  Or Rule End Parts: " << endPart << std::endl;

	theList.push_back(new SubListRule(theDb, -1, firstPart));
	theList.push_back(new SubListRule(theDb, -1, endPart));

	for(auto curNumStr: firstPart)
	{
		theFirstTerms.push_back(atoi(curNumStr.c_str()));
	}

	for(auto curNumStr: endPart)
	{
		theSecondTerms.push_back(atoi(curNumStr.c_str()));
	}

	if ( std::find(theSecondTerms.begin(), theSecondTerms.end(), ruleNum) != theSecondTerms.end())
	{
		// This rule is a looping rule
		DEBUG << "Rule " << ruleNum << " is a looping rule!" << std::endl;
		theLoopingFlag = true;
	}
	else
	{
		theLoopingFlag = false;
	}
}

bool OrRule::matchRule(std::string msg, int& matchPos)
{
	if (matchPos >= msg.size())
	{
		DEBUG << "OrRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return false;
	}

	if (theLoopingFlag)
	{
		DEBUG << "Looping rule " << ruleToString() << " at pos " << matchPos << std::endl;
	}

	int matchPos1 = matchPos;
	int matchPos2 = matchPos;

	DEBUG << "  OrRule::matchRule(" << ruleToString() << "): matching against first sub rule at "
	      << matchPos1 << std::endl;
	bool m1 = theList[0]->matchRule(msg, matchPos1);
	
	DEBUG << "  OrRule::matchRule(" << ruleToString() << "): matching against second sub rule at "
	      << matchPos2 << std::endl;
	bool m2 = theList[1]->matchRule(msg, matchPos2);

	if (m1)
	{
		DEBUG << "Rule (" << ruleToString() << ") Match 1 success at " << matchPos << "!"
		      << std::endl;
		matchPos = matchPos1;
		return true;
	}

	if (m2)
	{
		DEBUG << "Rule (" << ruleToString() << ") Match 2 success at " << matchPos << "!"
		      << std::endl;
		matchPos = matchPos2;
		return true;
	}

	DEBUG << "Neither part of OR rule (" << ruleToString() << ") matched!" << std::endl;
	return false;
	
}

std::string OrRule::ruleToString()
{
	std::string retVal;
	retVal += std::to_string(theRuleNumber);
	retVal += ": \"";

	retVal += toString(theFirstTerms);
	retVal += " | ";
	retVal += toString(theSecondTerms);

	return retVal;

}



void RuleDB::addRule(std::string text)
{
	std::vector<std::string> parts = stringSplit(text, {':', ' '});

	DEBUG << "Rule parts: " << parts << std::endl;

	assert(parts.size() > 1);

	int ruleNum = atoi(parts[0].c_str());
	parts.erase(parts.begin());

	if (parts[0][0] == '"')
	{
		Rule* r = new SingleLetterRule(this, ruleNum, parts);
		theRules[ruleNum] = r;
		return;
	}

	if (std::find(parts.begin(), parts.end(), "|") != parts.end())
	{
		Rule* r = new OrRule(this, ruleNum, parts);
		theRules[ruleNum] = r;
		return;
	}

	Rule* r = new SubListRule(this, ruleNum, parts);
	theRules[ruleNum] = r;
}

void RuleDB::dump()
{
	for(auto singleRule: theRules)
	{
		DEBUG << singleRule.second->ruleToString() << std::endl;
	}
}



int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	RuleDB rdb;
	int part1Sol = 0;
	bool loadingRulesMode = true;
	for(auto curLine: fileData)
	{
		if (curLine == "")
		{
			DEBUG << "End of rules" << std::endl << std::endl;;
			loadingRulesMode = false;

			DEBUG << "Dump:" << std::endl;
			rdb.dump();

			continue;
		}

		if (loadingRulesMode)
		{
			// Loading rules
			rdb.addRule(curLine);
		}
		else
		{
			DEBUG << "Match on message: " << curLine << std::endl;
			// Loading messages to check
			int mp = 0;
			Rule* r = rdb.getRule(0);
			
			if (r->matchRule(curLine, mp))
			{
				if (mp != curLine.size())
				{
					DEBUG << "FINAL: Match fail (line size) " << curLine << std::endl;
				}
				else
				{
					DEBUG << "FINAL: Match success! " << curLine << std::endl;
					part1Sol++;
				}
			}
			else
			{
				DEBUG << "FINAL: Match fail " << curLine << std::endl;
			}
		}
	}

	std::cout << "Part 1 Sol = " << part1Sol << std::endl;
	return 0;
}
