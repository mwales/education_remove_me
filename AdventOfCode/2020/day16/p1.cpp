#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

std::vector<char> RangeDelimeters = { ' ', ':', '-' };

class DataRange
{
public:
	DataRange(std::string const & rule);

	bool isValid(int);

	void dumpRule();

	std::string theName;

	int theStart1;
	int theEnd1;
	int theStart2;
	int theEnd2;
};

DataRange::DataRange(std::string const & rule)
{
	std::vector<std::string> ruleParts = stringSplit(rule, ':');

	theName = ruleParts[0];
	DEBUG << "New Range Rule: " << theName << std::endl;

	std::vector<std::string> rangeParts = stringSplit(ruleParts[1], RangeDelimeters);

	debugPrintVector(rangeParts);
	DEBUG << std::endl;

	theStart1 = atoi(rangeParts[0].c_str());
	theEnd1 = atoi(rangeParts[1].c_str());
	theStart2 = atoi(rangeParts[3].c_str());
	theEnd2 = atoi(rangeParts[4].c_str());

}

bool DataRange::isValid(int value)
{
	if ( (value >= theStart1) && (value <= theEnd1) )
		return true;
	if ( (value >= theStart2) && (value <= theEnd2) )
		return true;

	return false;
}

void DataRange::dumpRule()
{
	DEBUG << "Rule: " << theName << ", " << theStart1 << " - " << theEnd1 << " or " 
	      << theStart2 << " - " << theEnd2 << std::endl;
}

class TicketSystem
{
public:
	TicketSystem();

	bool isTicketValid(std::string const & ticket, int* violatingValue);
	bool isValueValidForAnyRule(int);

	void addDataRangeRule(std::string const & rule);

	void addTicketToCollection(std::string const & str);

	void discoverRangeLocations();

	void dumpTicketCollection();

	int getTicketField(std::string ticket, std::string fieldName);

	void analyzeRangeResults(std::vector<std::vector<int> > validRules);

	std::vector<DataRange> theRanges;
	std::map<int, DataRange*> theOrderedRanges;

	std::vector<std::vector<int> > theTicketCollection;
};

TicketSystem::TicketSystem()
{
	
}

int TicketSystem::getTicketField(std::string ticket, std::string fieldName)
{
	std::vector<std::string> ticketParts = stringSplit(ticket, ',');

	for(auto rangeField: theOrderedRanges)
	{
		if (rangeField.second->theName == fieldName)
		{
			int value = atoi(ticketParts[rangeField.first].c_str());
			return value;
		}
	}
	return -1;
			
}

void TicketSystem::dumpTicketCollection()
{
	DEBUG << "*************************************** TICKETS ***********************" << std::endl;
	for(auto t: theTicketCollection)
	{
		debugPrintVector(t);
		DEBUG << std::endl;
	}
	DEBUG << "*************************************** TICKETS ***********************" << std::endl;
}

void TicketSystem::analyzeRangeResults(std::vector<std::vector<int> > validRules)
{
	while(theOrderedRanges.size() < theRanges.size())
	{
		int colWithOneRule = -1;
		// Find the col with only 1 valid rule
		for(int i = 0; i < validRules.size(); i++)
		{
			if (validRules[i].size() == 1)
			{
				colWithOneRule = i;
				break;
			}
		}

		// Add that rule to map
		int ruleNumber = validRules[colWithOneRule][0];
		theOrderedRanges[colWithOneRule] = &theRanges[ruleNumber];
		DEBUG << "Col " << colWithOneRule << " is for range " << theOrderedRanges[colWithOneRule]->theName << std::endl;

		// Remove that rule from all other columns
		for(int i = 0; i < validRules.size(); i++)
		{
			for(int j = 0; j < validRules[i].size(); j++)
			{
				if (validRules[i][j] == ruleNumber)
				{
					validRules[i].erase(validRules[i].begin() + j);
				}
			}
		}
	}
}


void TicketSystem::discoverRangeLocations()
{
	std::vector<std::vector<int> > validRulesForTicketCol;
	for(int ticketCol = 0; ticketCol < theRanges.size(); ticketCol++)
	{
		DEBUG << "Discovering Column " << ticketCol << std::endl;

		std::vector<int> validRulesForCol;

		for(int ruleNum = 0; ruleNum < theRanges.size(); ruleNum++)
		{
			bool ruleValid = true;
			DataRange dr = theRanges[ruleNum];
			DEBUG << "  Checking against rule " << ruleNum << ": ";
		 	dr.dumpRule();

			for(int tn = 0; tn < theTicketCollection.size(); tn++)
			{
				int curVal = theTicketCollection[tn][ticketCol];
				if (!dr.isValid(curVal))
				{
					DEBUG << "    Rule failed for ticket " << tn << ", value "
					      << curVal << std::endl;

					ruleValid = false;
					tn = theTicketCollection.size();
				}
				else
				{
					DEBUG << "    Ticket " << tn << " has valid value " << curVal << std::endl;
				}
			}

			if (ruleValid)
			{
				validRulesForCol.push_back(ruleNum);
			}

		}

		validRulesForTicketCol.push_back(validRulesForCol);
		DEBUG << "Valid Rules For Col " << ticketCol << " are ";
		debugPrintVector(validRulesForCol);
		DEBUG << std::endl;

	}

	analyzeRangeResults(validRulesForTicketCol);
}

void TicketSystem::addTicketToCollection(std::string const & ticket)
{
	std::vector<int> ticketVec;
	
	std::vector<std::string> ticketParts = stringSplit(ticket, ',');
	for(auto curField: ticketParts)
	{
		ticketVec.push_back(atoi(curField.c_str()));
	}

	theTicketCollection.push_back(ticketVec);
}	

void TicketSystem::addDataRangeRule(std::string const & rule)
{
	DataRange dr(rule);
	dr.dumpRule();
	theRanges.push_back(dr);
}

bool TicketSystem::isValueValidForAnyRule(int value)
{
	for(auto curRule: theRanges)
	{
		if (curRule.isValid(value))
		{
			return true;
		}
	}
	return false;
}


bool TicketSystem::isTicketValid(std::string const & ticket, int* violatingValue)
{
	std::vector<std::string> ticketParts = stringSplit(ticket, ',');
	std::vector<int> ticketFields;
	for(auto tpStr: ticketParts)
	{
		ticketFields.push_back(atoi(tpStr.c_str()));
	}

	DEBUG << "TICKET: ";
	debugPrintVector(ticketFields);
	DEBUG << std::endl;

	for(auto curField: ticketFields)
	{
		if (!isValueValidForAnyRule(curField))
		{
			DEBUG << "Ticket Invalid, bad value: " << curField << std::endl;
			
			if (violatingValue)
			{
				*violatingValue = curField;
			}
			return false;
		}
	}

	DEBUG << "Ticket valid for all rules" << std::endl;
	return true;
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);

	TicketSystem ts;

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile,text);

		if (text == "")
		{
			DEBUG << "Done reading rules" << std::endl;
			break;
		}

		ts.addDataRangeRule(text);

	}

	std::string trash1, myticket;
	std::getline(infile, trash1); // your ticket:
	std::getline(infile, myticket);
	std::getline(infile, trash1); // blank line
	std::getline(infile, trash1); // nearby tickets

	std::vector<std::string> tickets;

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile, text);


		// out of output
check_for_eof:
      if (infile.eof())
		{
			break;
		}

		DEBUG << text << std::endl;
		tickets.push_back(text);
	}

	infile.close();

	int sumViolations = 0;
	int curViolation = 0;
	
	ts.addTicketToCollection(myticket);
	for(auto curTicket: tickets)
	{
		if (!ts.isTicketValid(curTicket, &curViolation))
		{
			sumViolations += curViolation;
			DEBUG << "Cur Violation = " << curViolation << " brings us to " << sumViolations << std::endl;
		}
		else
		{
			ts.addTicketToCollection(curTicket);
		}
	}

	ts.dumpTicketCollection();

	ts.discoverRangeLocations();

	std::vector<std::string> departureFields;
	for(auto dr: ts.theRanges)
	{
		if (dr.theName.find("departure") != std::string::npos)
		{
			departureFields.push_back(dr.theName);
		}
	}

	DEBUG << "Departure field list: ";
	debugPrintVector(departureFields);
	DEBUG << std::endl;

	uint64_t departureProduct = 1;
	for(auto depField: departureFields)
	{
		uint64_t val = ts.getTicketField(myticket, depField);
		departureProduct *= val;
	}

	std::cout << "Pt1: Error Scan Rate = " << sumViolations << std::endl;
	std::cout << "Pt2: Product = " << departureProduct << std::endl;
	return 0;
}
