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

	

std::vector<int> runProgram(std::vector<int> programData)
{
	int pc = 0;
	while(true)
	{
		std::cout << "State: ";
		printIntList(programData);

		if( (pc < 0) || (pc >= programData.size()) )
		{
			std::cout << "PC of " << pc << " is invalid location" << std::endl;
			return programData;
		}

		int opCode = programData[pc];

		switch(opCode)
		{
			case 1:
			case 2:
			{
				std::cout << "Op Code is " << opCode << ( opCode == 1 ? " (additon)" : " (muliplty)") << std::endl;
				
				// We need 3 more operands
				if ( (pc + 3) >= programData.size() )
				{
					std::cout << "Program data not long enough to store operands" << std::endl;
					return programData;
				}

				int operand1Pos = programData[pc + 1];
				int operand2Pos = programData[pc + 2];
				int resultPos = programData[pc + 3];

				if ( (operand1Pos >= programData.size()) ||
				     (operand2Pos >= programData.size()) ||
			             (resultPos >= programData.size()) )
				{
					std::cout << "One of the operand / result pos out of range of program data" << std::endl;
					return programData;
				}

				int op1 = programData[operand1Pos];
				int op2 = programData[operand2Pos];

				std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

				int resultVal;
				if (opCode == 1)
				{
					resultVal = op1 + op2;
					std::cout << "Addition result " << resultVal << std::endl;
				}
				else
				{
					resultVal = op1 * op2;
					std::cout << "Multiple result " << resultVal << std::endl;
				}

				programData[resultPos] = resultVal;

				pc += 4;
				break;
			}
			case 99:
				std::cout << "Op Code is 99, end program" << std::endl;
				return programData;
			default:
				std::cout << "Op Code is " << opCode << ", invalid!" << std::endl;
				return programData;
		}
	}





}


int main(int argc, char** argv)
{
	while(true)
	{
		int singleVal = 0;
		std::string stuff;

		std::cin >> stuff;


		if (std::cin.eof())
		{
			break;
		}


		std::vector<std::string> tokens = split(stuff, ",");
		std::vector<int> programData = strListToInt(tokens);
		
		// Day 1 special stuff
		programData[1] = 12;
		programData[2] = 2;
		runProgram(programData);

		std::cout << std::endl << std::endl;
	}





	return 0;
}
