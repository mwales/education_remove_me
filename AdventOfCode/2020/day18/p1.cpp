#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

std::vector<std::string> smartSplit(std::string const & text)
{
	std::vector<std::string> retVal;
	std::string curString;

	std::set<char> delimitersRetaining = {'(', ')', '+', '*'};

	// Remove all spaces.  Break strings on ( ) + *, but retain symbols
	for(auto curChar: text)
	{
		if (curChar == ' ')
		{
			if (curString.size() > 0)
			{
				// We don't keep any spaces
				retVal.push_back(curString);
			}

			curString = "";
			continue;
		}

		if (delimitersRetaining.find(curChar) != delimitersRetaining.end())
		{
		// We found a delimiter / keep it
				
			if (curString.size() > 0)
			{
				retVal.push_back(curString);
				curString = "";
			}

			curString += curChar;
			retVal.push_back(curString);
			curString = "";
			continue;
		}

		curString += curChar;
	}

	if (curString.size() > 0)
	{
		retVal.push_back(curString);
	}

	return retVal;
}

// Want just 3 terms from this
std::vector<std::string> mathSplit(std::vector<std::string> parts)
{
	std::vector<std::string> retVal;
	std::string curString;

	if (parts.size() == 1) 
	{
		return parts;
	}

	if (parts.size() <= 2)
	{
		std::cout << "ERROR: ";
		printVector(parts);
		std::cout << std::endl;

		retVal.push_back("0");
		retVal.push_back("+");
		retVal.push_back("0");
		return retVal;
	}

	if (parts[0] != "(")
	{
		retVal.push_back(parts[0]); // Better be a number
		retVal.push_back(parts[1]); // Better be a operation
		for(int i = 2; i < parts.size(); i++) // Don't care, but it is the rhs
		{
			curString += parts[i];
		}
		retVal.push_back(curString);
		return retVal;
	}

	// If we got here, we found a parens as first term.
	//   It's either: (1+2+3)
	//           or : (1+2)+3
	
	// Check for parens wraping all the parts
	if (parts.back() == ")")
	{
		DEBUG << "Found special case of () wrapped term" << std::endl;
		DEBUG << " Before: ";
		debugPrintVector(parts);
		DEBUG << std::endl;

		parts.erase(parts.begin());
		parts.pop_back();
	
		DEBUG << " After: ";
		debugPrintVector(parts);
		DEBUG << std::endl;

		return mathSplit(parts);
	}	
	
	int parensLevel = 1;
	for(int i = 1; i < parts.size(); i++)
	{
		if (parts[i] == "(")
		{
			parensLevel++;
		}
		if (parts[i] == ")")
		{
			parensLevel--;

			if(parensLevel == 0)
			{
				retVal.push_back(curString);
				
				// Is there any terms after this point
				//   Could have been (stuff)
				//   or (stuff) * (otherstuff)
				curString = "";
				i++;
				retVal.push_back(parts[i]);   // operand
				i++;
				for(; i < parts.size(); i++)
				{
					curString += parts[i];
				}
				retVal.push_back(curString);
				return retVal;
			}
		}

		curString += parts[i];
	}

	// If we get here, something messed up
	std::cout << "ERROR: ";
	printVector(parts);
	std::cout << std::endl;

	retVal.push_back("0");
	retVal.push_back("+");
	retVal.push_back("0");
	return retVal;
}


class MathExpression
{
public:
	MathExpression(std::string text);

	int solve();

	void printExpression();

	MathExpression* theLeft;
	MathExpression* theRight;

	std::string theOpVal;
};

void MathExpression::printExpression()
{
	if (theLeft)
	{
		theLeft->printExpression();
	}

	std::cout << " " << theOpVal << " ";

	if (theRight)
	{
		theRight->printExpression();
	}
}

MathExpression::MathExpression(std::string text)
{
	DEBUG << "MathExpression( " << text << " )" << std::endl;

	std::vector<std::string> basicParts = smartSplit(text);
	// Reverse it cause i did right-left instead of left-right
	basicParts = reverseVector(basicParts);
	
	DEBUG << "PARTS = ";
	debugPrintVector(basicParts);
	DEBUG << std::endl;

	std::vector<std::string> mathParts = mathSplit(basicParts);
	
	// Reverse it cause i did right-left instead of left-right

	DEBUG << "MATH = ";
	debugPrintVector(mathParts);
	DEBUG << std::endl;

	theLeft = nullptr;
	theRight = nullptr;

	if (mathParts.size() == 0)
	{
		std::cout << "ERROR: No math parts: " << text << std::endl;
		theOpVal = "0";
		return;
	}

	if (mathParts.size() == 1)
	{
		theOpVal = mathParts[0];
		return;
	}

	if (mathParts.size() == 2)
	{
		std::cout << "ERROR: 2 math parts: " << text << std::endl;
		theLeft = new MathExpression(mathParts[0]);
		theOpVal = "+";
		theRight = new MathExpression("0");
		return;
	}

	theLeft = new MathExpression(mathParts[0]);
	theOpVal = mathParts[1];
	theRight = new MathExpression(mathParts[2]);
	
}

int MathExpression::solve()
{
	if (theOpVal == "+")
	{
		int retVal = theLeft->solve() + theRight->solve();
		
		DEBUG << "Solving with add: ";
		printExpression();
		DEBUG << " = " << retVal << std::endl;

		return retVal;
	}

	if (theOpVal == "*")
	{
		int retVal = theLeft->solve() * theRight->solve();
		DEBUG << "Solving with multiply: ";
		printExpression();
		DEBUG << " = " << retVal << std::endl;

		return retVal;
	}

	return atoi(theOpVal.c_str());
}


void doMath(std::string text)
{
	MathExpression me(text);
	int sol = me.solve();
	std::cout << text << " = " << sol << std::endl;
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile,text);


		// out of output
check_for_eof:
      if (infile.eof())
		{
			break;
		}

		DEBUG << text << std::endl;

		doMath(text);
	}

	infile.close();

	

	return 0;
}
