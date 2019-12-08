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

int getOperand(std::vector<int> const & programData,
               int operandMode, int operandRawVal)
{
   if (operandMode == 0)
   {
      // Position mode
      if ( programData.size() < operandRawVal)
      {
         // Positional parameter with value out-of-range
         std::cerr << "ERROR! Position param " << operandRawVal << " out of range";
         return -1;
      }

      return programData[operandRawVal];
   }
   else
   {
      // Immediate mode
      return operandRawVal;
   }
}

int readDataAtAddress(std::vector<int> & programData, int addr)
{
   if (addr > programData.size())
   {
      // We have to add memory to program storage!
      std::cerr << "Address " << addr << " is out of range of program data (read)" << std::endl;
      exit(1);
   }

   return programData[addr];
}

void writeDataAtAddress(std::vector<int> & programData, int addr, int value)
{
   if (addr > programData.size())
   {
      // Address out of range
      std::cerr << "Address " << addr << " is out of range of program data (write)" << std::endl;
      exit(1);
   }

   programData[addr] = value;
}



std::vector<int> runProgram(std::vector<int> programData)
{
	int pc = 0;
	while(true)
	{
      // std::cout << "State: ";
      // printIntList(programData);

		if( (pc < 0) || (pc >= programData.size()) )
		{
			std::cout << "PC of " << pc << " is invalid location" << std::endl;
			return programData;
		}

      /**
       ABCDE
       1002

      DE - two-digit opcode,      02 == opcode 2
       C - mode of 1st parameter,  0 == position mode
       B - mode of 2nd parameter,  1 == immediate mode
       A - mode of 3rd parameter,  0 == position mode,
                                        omitted due to being a leading zero
      */

		int opCode = programData[pc];

      int instructionDE = opCode %100;
      int operandCMode = (opCode / 100) % 10;
      int operandBMode = (opCode / 1000) % 10;
      int operandAMode = (opCode / 10000) % 10;

      std::cout << "PC=" << pc << " opCode=" << opCode << " OpModeC=" << operandCMode << ", OpModeB="
                << operandBMode << ", OpModeA=" << operandAMode << std::endl;

      switch(instructionDE)
		{
         case 1:  // addition
         case 2:  // multiply
			{
            std::cout << "Op Code is " << instructionDE << ( instructionDE == 1 ? " (additon)" : " (multiply)") << std::endl;
				            
            int op1 = getOperand(programData, operandCMode, readDataAtAddress(programData, pc+1));
            int op2 = getOperand(programData, operandBMode, readDataAtAddress(programData, pc+2));

				std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

				int resultVal;
            if (instructionDE == 1)
				{
               resultVal = op1 + op2;
					std::cout << "Addition result " << resultVal << std::endl;
				}
				else
				{
					resultVal = op1 * op2;
               std::cout << "Multiply result " << resultVal << std::endl;
				}

            // Last parameter must be positional
            int resultPos = readDataAtAddress(programData, pc+3);
            writeDataAtAddress(programData, resultPos, resultVal);

				pc += 4;
				break;
			}

         case 3: // input
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is input" << std::endl;
            if (programData.size() < pc + 1)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int writeLocation = programData[pc + 1];

            if (writeLocation > programData.size())
            {
               std::cout << "Program data not long enough to store result of input" << std::endl;
               return programData;
            }

            int value;
            std::cin >> value;
            std::cout << "Read input value: " << value << std::endl;
            programData[writeLocation] = value;

            pc += 2;
            break;
         }

         case 4: // output
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is output" << std::endl;
            if (programData.size() < pc + 1)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int outputData = getOperand(programData, operandCMode, programData[pc+1]);
            std::cout << "OUTPUT: " << outputData << std::endl;
            pc += 2;
            break;
         }

         case 5: // jump-if-true
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is jump-if-true" << std::endl;
            if (programData.size() < pc + 2)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int op1 = getOperand(programData, operandCMode, programData[pc + 1]);
            int op2 = getOperand(programData, operandBMode, programData[pc + 2]);

            std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

            if (op1)
            {
               std::cout << "We are jumping to " << pc << "!" << std::endl;
               pc = op2;
            }
            else
            {
               std::cout << "We are not jumping!" << std::endl;
               pc += 3;
            }
            break;
         }

         case 6: // jump-if-false
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is jump-if-false" << std::endl;
            if (programData.size() < pc + 2)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int op1 = getOperand(programData, operandCMode, programData[pc + 1]);
            int op2 = getOperand(programData, operandBMode, programData[pc + 2]);

            std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

            if (!op1)
            {
               std::cout << "We are jumping to " << pc << "!" << std::endl;
               pc = op2;
            }
            else
            {
               std::cout << "We are not jumping!" << std::endl;
               pc += 3;
            }
            break;
         }

         case 7: // less-than
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is less-than" << std::endl;
            if (programData.size() < pc + 3)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int op1 = getOperand(programData, operandCMode, programData[pc + 1]);
            int op2 = getOperand(programData, operandBMode, programData[pc + 2]);
            int op3 = programData[pc + 3];

            std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << " and Op 3 = "
                      << op3 << std::endl;

            if (op1 < op2)
            {
               std::cout << "op1<op2, storing 1 in op3 " << std::endl;

               if (op3 > programData.size())
               {
                  std::cout << "Can't store at address out of range" << std::endl;
                  return programData;
               }

               programData[op3] = 1;
            }
            else
            {
               programData[op3] = 0;
            }
            pc += 4;
            break;
         }

         case 8: // equal-to
         {
            // Verify program data has arg data
            std::cout << "Opcode " << opCode << " is equal-to" << std::endl;
            if (programData.size() < pc + 3)
            {
               std::cout << "Program data not long enough to store operands" << std::endl;
               return programData;
            }

            int op1 = getOperand(programData, operandCMode, programData[pc + 1]);
            int op2 = getOperand(programData, operandBMode, programData[pc + 2]);
            int op3 = programData[pc + 3];

            std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << " and Op 3 = "
                      << op3 << std::endl;

            if (op1 == op2)
            {
               std::cout << "op1==op2, storing 1 in op3 " << std::endl;

               if (op3 > programData.size())
               {
                  std::cout << "Can't store at address out of range" << std::endl;
                  return programData;
               }

               programData[op3] = 1;
            }
            else
            {
               programData[op3] = 0;
            }

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
		runProgram(programData);

		std::cout << std::endl << std::endl;
	}





	return 0;
}
