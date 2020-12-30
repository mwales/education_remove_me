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

	virtual std::vector<std::string> matchRule(std::string msg, std::string matchProgress) = 0;

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

	virtual std::vector<std::string> matchRule(std::string msg, std::string matchProgress);
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

std::vector<std::string> SingleLetterRule::matchRule(std::string msg, std::string matchProgress)
{
	std::vector<std::string> retVal;

	if (matchProgress.size() >= msg.size())
	{
		DEBUG << "SingleLetterRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return retVal;
	}

	bool success = msg[matchProgress.size()] == theLetter;

	DEBUG << "SingleLetterRule::matchRule (" << ruleToString() << ") with progress "
	      << matchProgress << " matching against " << theLetter << " = "
	      << (success ? "success" : "fail" ) << std::endl;

	if (!success)
	{
		return retVal;
	}

	matchProgress += theLetter;
	retVal.push_back(matchProgress);
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

	virtual std::vector<std::string> matchRule(std::string msg, std::string matchProgress);
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

std::vector<std::string> SubListRule::matchRule(std::string msg, std::string matchProgress)
{
	std::vector<std::string> retVal;
	if (matchProgress.size() >= msg.size())
	{
		DEBUG << "SubListRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return retVal;
	}

	retVal.push_back(matchProgress);

	for(auto ruleNum: theList)
	{
		std::vector<std::string> nextRetVal;
		DEBUG << "SubListRule::matchRule going to next rule in loop.  ruleNum = "
		      << ruleNum << " with retVal= " << retVal << std::endl;

		// Each sub rule could return multiple results, need to test those results with
		// subrules after they occur for matching too
		int i = 0;
		while(i < retVal.size())
		{
			std::string curProgress = retVal[i];
			DEBUG << "SubListRule::matchRule (" << ruleToString() << ") checking " << curProgress
			      << " of current result set " << retVal << " at rule " << ruleNum
			      << std::endl;

			Rule* r = theDb->getRule(ruleNum);
		
			DEBUG << "SubListRule::matchRule (" << ruleToString() << ") going to match pos="
			      << curProgress << " against sub rule " << ruleNum << std::endl;
			
			std::vector<std::string> newResult = r->matchRule(msg, curProgress);

			if (newResult.size() == 0)
			{
				// Match failed for this one
				// Eliminate curProgress from retVal result
				DEBUG << "SubListRule::matchRule (" << ruleToString()
				      << ") sub rule " << ruleNum << " match fail.  Progress="
				      << curProgress << ", removing " << retVal[i] << std::endl;

				retVal.erase(retVal.begin() + i);
			}
			else
			{
				// Match was successful
				
				DEBUG << "SubListRule::matchRule (" << ruleToString()
				      << ") sub rule " << ruleNum << " match success.  Progress="
				      << curProgress << ", result = " << newResult << std::endl;
				
				// merge some stuff
				nextRetVal = append(nextRetVal, newResult);
				i++;
			}

		}

		if (nextRetVal.size() == 0)
		{
			// All cases must have failed this subrule, time to exit
			DEBUG << "SubListRule::matchRule (" << ruleToString()
			      << ") failed all cases at subrule " << ruleNum << ", progress="
			      << matchProgress << std::endl;
			
			retVal.clear();
			return retVal;
		}

		retVal = nextRetVal;
	}

	DEBUG << "SubListRule::matchRule(" << ruleToString() << ") all subrules success: " 
	      << retVal << std::endl;
	return retVal;
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

	virtual std::vector<std::string> matchRule(std::string msg, std::string matchProgress);
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

std::vector<std::string> OrRule::matchRule(std::string msg, std::string matchProgress)
{
	std::vector<std::string> retVal;

	if (matchProgress.size() >= msg.size())
	{
		DEBUG << "OrRule::matchRule failing cause pos past end of message"
		      << std::endl;
		return retVal;
	}

	if (theLoopingFlag)
	{
		DEBUG << "Looping rule " << ruleToString() << " at pos " << matchProgress << std::endl;
	}

	DEBUG << "  OrRule::matchRule(" << ruleToString() << "): matching against first sub rule at "
	      << matchProgress << std::endl;
	std::vector<std::string> m1 = theList[0]->matchRule(msg, matchProgress);
	
	DEBUG << "  OrRule::matchRule(" << ruleToString() << "): matching against second sub rule at "
	      << matchProgress << std::endl;
	std::vector<std::string>  m2 = theList[1]->matchRule(msg, matchProgress);

	if (m1.size() > 0)
	{
		DEBUG << "Rule (" << ruleToString() << ") Match 1 success at " << m1 << "!"
		      << std::endl;
	}

	if (m2.size() > 0)
	{
		DEBUG << "Rule (" << ruleToString() << ") Match 2 success at " << m2 << "!"
		      << std::endl;
	}

	return append(m1, m2);
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
		
			std::vector<std::string> matchResult = r->matchRule(curLine, "");	
			if (matchResult.size() && matchResult[0] == curLine)
			{
				DEBUG << "FINAL: Match success! " << curLine << " = "
				      << matchResult << std::endl;
				part1Sol++;
			}
			else
			{
				DEBUG << "FINAL: Match fail     " << curLine << std::endl;
			}
		}
	}

	std::cout << "Part 1 Sol = " << part1Sol << std::endl;
	return 0;
}
