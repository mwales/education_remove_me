#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <assert.h>

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
	
	for(int i = 0; i < parts.size(); i++)
	{
		//DEBUG << "mathSplit, i=" << i << ", parts[i]=" << parts[i] << std::endl;

		if (parts[i] == "(")
		{
			curString += parts[i];
			int parensLevel = 1;
			for(int subindex = i + 1; (subindex < parts.size()) && parensLevel; subindex++)
			{
				
				//DEBUG << "mathSplit, i=" << i << "si=" << subindex << ",pl="
				//      << parensLevel << ", parts[i]=" << parts[i] << std::endl;

				curString += parts[subindex];
				
				if (parts[subindex] == "(")
				{
					parensLevel++;
				}

				if (parts[subindex] == ")")
				{
					parensLevel--;
				}


				i++;
			}

			retVal.push_back(curString);
			curString = "";

		}
		else
		{
			//DEBUG << "Adding as simple term" << std::endl;
			retVal.push_back(parts[i]);
		}
	}

	return retVal;
}

std::string stripSurroundingParens(std::string text)
{
	std::string retVal = text;
	DEBUG << "Stripping:[" << text << "]" << std::endl;

	if (text[0] != '(')
	{
		return retVal;
	}

	int parensLevel = 1;
	for(int i = 1; i < text.size() - 1; i++)
	{
		if (text[i] == '(')
		{
			parensLevel++;
			DEBUG << "  stripper parens level ++ at i = " << i << " to " << parensLevel << std::endl;
		}

		if (text[i] == ')')
		{
			parensLevel--;
			DEBUG << "  stripper parens level -- at i = " << i << " to " << parensLevel << std::endl;
		}

		if (parensLevel == 0)
		{
			// premature early parensLevel == 0.
			// maybe it looks like (1 + 2) * (3 + 4).  Can't trim
			DEBUG << "striper special case: " << text << std::endl;
			return text;
		}
	}

	if( (retVal.front() == '(') && (retVal.back() == ')') )
	{
		retVal.erase(retVal.begin());
		retVal.pop_back();

		DEBUG << "Popping off one set of parens:[" << retVal << "], calling self recursively" << std::endl;
		return stripSurroundingParens(retVal);
	}

	DEBUG << "strip " << text << "   =>   " << retVal << std::endl;
	return retVal;
}


class MathExpression
{
public:
	MathExpression(std::string text);

	uint64_t solve();

	void printExpression();

	std::string toString();
	
	std::vector<MathExpression*> theSubExpn;
	std::vector<std::string> theOperators;

	std::string theValue;
};

std::string MathExpression::toString()
{
	std::string retVal;

	if (theOperators.size() == 0)
	{
		return theValue;
	}

	retVal += "(";

	for(int i = 0; i < theOperators.size(); i++)
	{
		retVal += theSubExpn[i]->toString();
		retVal += theOperators[i];
	}

	retVal += theSubExpn.back()->toString();

	retVal += ")";

	return retVal;
}

void MathExpression::printExpression()
{
	// We are a simple node
	if (theOperators.size() == 0)
	{
		DEBUG << theValue;
		return;
	}

	for(int i = 0; i < theOperators.size(); i++)
	{
		theSubExpn[i]->printExpression();
		
		DEBUG << theOperators[i];
	}

	theSubExpn.back()->printExpression();

}

MathExpression::MathExpression(std::string text)
{
	DEBUG << "MathExpression( " << text << " )" << std::endl;

	std::vector<std::string> basicParts = smartSplit(text);
	
	DEBUG << "PARTS = ";
	debugPrintVector(basicParts);
	DEBUG << std::endl;

	std::vector<std::string> mathParts = mathSplit(basicParts);
	
	// Reverse it cause i did right-left instead of left-right

	DEBUG << "MATH = ";
	debugPrintVector(mathParts);
	DEBUG << std::endl;

	if (mathParts.size() == 1)
	{
		theValue = mathParts[0];
	}
	else
	{
		for(auto it = mathParts.begin(); it != mathParts.end(); it++)
		{
			theSubExpn.push_back(new MathExpression(stripSurroundingParens(*it)));
			it++;

			if (it == mathParts.end())
				break;

			theOperators.push_back(*it);
		}
	}

	/*theLeft = nullptr;
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
	}while(theOperators.size())

	theLeft = new MathExpression(mathParts[0]);
	theOpVal = mathParts[1];
	theRight = new MathExpression(mathParts[2]);
*/	
}

uint64_t MathExpression::solve()
{
	// We are a simple node
	if (theOperators.size() == 0)
	{
		DEBUG << "Simple solve = " << atoi(theValue.c_str()) << std::endl;
		return strtoull(theValue.c_str(), NULL, 10);
	}

	std::string origSolve = toString();
	DEBUG << "****** solving " << origSolve << std::endl;

	uint64_t solution;

	DEBUG << "thOp.size() = " << theOperators.size() << " and theSubExpn.size() = " << theSubExpn.size() << std::endl;
	assert(theOperators.size() + 1 == theSubExpn.size());

	for(int i = 0; i < theOperators.size();)
	{
		uint64_t lhs = theSubExpn[i]->solve();
		uint64_t rhs = theSubExpn[i+1]->solve();

		DEBUG << "Current expression = " << toString() << std::endl;
		DEBUG << "i=" << i << ", lhs=" << lhs << ", rhs=" << rhs << std::endl;

		if (theOperators[i] == "+")
		{
			solution = lhs + rhs;
			DEBUG << "lhs + rhs = " << lhs << " * " << rhs << " = " << solution << std::endl;

		}
		else
		{
			solution = lhs * rhs;
			DEBUG << "lhs * rhs IS GETTING SKIPPED FOR NOW" << std::endl;
			i++;
			continue;
		}


		// Erase the lhs, and operator.  replace rhs with soln
		MathExpression* lhsExp = theSubExpn[i];
		MathExpression* rhsExp = theSubExpn[i+1];
		DEBUG << "Deleting lhs and rhs" << std::endl;
		delete lhsExp;
		delete rhsExp;
		theSubExpn.erase(theSubExpn.begin() + i);
		theSubExpn.erase(theSubExpn.begin() + i);
		MathExpression* replace = new MathExpression(std::to_string(solution));
		theSubExpn.insert(theSubExpn.begin() + i, replace);

		theOperators.erase(theOperators.begin() + i);


		if (theOperators.size() == 0)
		{
			DEBUG << "No more terms in this expression, return solution" << std::endl;
			break;
		}
	}

	for(int i = 0; i < theOperators.size();)
	{
		uint64_t lhs = theSubExpn[i]->solve();
		uint64_t rhs = theSubExpn[i+1]->solve();

		DEBUG << "Current expression = " << toString() << std::endl;
		DEBUG << "i=" << i << ", lhs=" << lhs << ", rhs=" << rhs << std::endl;

		if (theOperators[i] == "+")
		{
			solution = lhs + rhs;
			DEBUG << "lhs + rhs = WTF they should be gone already" << std::endl;
			assert(0);
		}
		else
		{

			solution = lhs * rhs;
			DEBUG << "lhs * rhs = " << lhs << " * " << rhs << " = " << solution << std::endl;

		}


		// Erase the lhs, and operator.  replace rhs with soln
		MathExpression* lhsExp = theSubExpn[i];
		MathExpression* rhsExp = theSubExpn[i+1];
		DEBUG << "Deleting lhs and rhs" << std::endl;
		delete lhsExp;
		delete rhsExp;
		theSubExpn.erase(theSubExpn.begin() + i);
		theSubExpn.erase(theSubExpn.begin() + i);
		MathExpression* replace = new MathExpression(std::to_string(solution));
		theSubExpn.insert(theSubExpn.begin() + i, replace);

		theOperators.erase(theOperators.begin() + i);


		if (theOperators.size() == 0)
		{
			DEBUG << "No more terms in this expression, return solution" << std::endl;
			break;
		}
	}

	DEBUG << "***** solved " << origSolve << " = " << solution << std::endl;


	theValue = std::to_string(solution);
	return solution;
}


uint64_t doMath(std::string text)
{
	MathExpression me(text);
	uint64_t sol = me.solve();
	
	std::cout << text << " = " << sol << std::endl;
	return sol;
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);
	
	uint64_t finalSol = 0;

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

		uint64_t curSol = doMath(text);

		finalSol += curSol;
	}

	infile.close();

	std::cout << "Final sum of all solutions = " << finalSol << std::endl;

	return 0;
}
