#include <iostream>
#include <vector>
#include <ncurses.h>

#include "ElfComputer.h"

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

std::vector<int64_t> strListToInt(std::vector<std::string> const & data)
{
   std::vector<int64_t> retVal;
	for(auto di = data.begin(); di != data.end(); di++)
	{
      int64_t val = atol(di->c_str());
		// std::cerr << "Converting item " << *di << " to " << val << std::endl;
		retVal.push_back(val);
	}

	return retVal;
}

void printIntList(std::vector<int64_t> const & data)
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


ElfComputer::ElfComputer(std::vector<int64_t> const & pd, std::string const & name):
   thePc(0),
   theBase(0),
   theInputDataQ(nullptr),
   theOutputDataQ(nullptr),
   theName(name),
   theHaltFlag(false),
   theNumIterations(0)
{
   theProgramData = pd;
}

void ElfComputer::addInputData(int64_t singleItem)
{
   if (theInputDataQ == nullptr)
   {
      std::cerr << "Never setup the input Q on " << theName << std::endl;
      exit(1);
   }

   theInputDataQ->push_back(singleItem);
}

void ElfComputer::addInputData(std::vector<int64_t> multiItems)
{
   if (theInputDataQ == nullptr)
   {
      std::cerr << "Never setup the input Q on " << theName << std::endl;
      exit(1);
   }

   for(auto it = multiItems.begin(); it != multiItems.end(); it++)
   {
      theInputDataQ->push_back(*it);
   }
}

void ElfComputer::setInputDataQ(std::vector<int64_t>* inQ)
{
   theInputDataQ = inQ;
}

void ElfComputer::setOutputDataQ(std::vector<int64_t>* outQ)
{
   theOutputDataQ = outQ;
}

int64_t ElfComputer::getOperand(int64_t operandMode, int64_t operandRawVal)
{
   if (operandMode == 0)
   {
      // Position mode
      if ( theProgramData.size() < operandRawVal)
      {
         // Positional parameter with value out-of-range
         expandMemory(operandRawVal);
      }

      return readDataAtAddress(operandRawVal);
   }
   else if (operandMode == 1)
   {
      // Immediate mode
      return operandRawVal;
   }
   else
   {
      // Relative (to base) parameter
      int index = theBase + operandRawVal;
      DEBUGOUT << "Base + op = " << theBase << " + " << operandRawVal << " = " << index << std::endl;

      if(index < 0)
      {
         std::cerr << "ERROR! Relative param created negative index " << index << " " << theName << std::endl;
         return -1;
      }

      return readDataAtAddress(index);
   }

}

void ElfComputer::writeOperand(int64_t operandMode, int64_t operandRawVal, int64_t writeVal)
{
   // Writes can't be immediate mode, only positional or relative
   int64_t writeLocation = operandRawVal;
   if (operandMode == 2)
   {
      // Relative mode
      writeLocation += theBase;
   }

   writeDataAtAddress(writeLocation, writeVal);
}

int64_t ElfComputer::readDataAtAddress(int addr)
{
   DEBUGOUT << "readDataAtAddress(" << addr << ")" << std::endl;
   if (addr >= theProgramData.size())
   {
      // We have to add memory to program storage!
      DEBUGOUT << "Address " << addr << " is out of range of program data (read)" << std::endl;
      expandMemory(addr);
   }

   DEBUGOUT << "readDataAtAddress(" << addr << ") = " << theProgramData[addr] << std::endl;
   return theProgramData[addr];
}

void ElfComputer::writeDataAtAddress(int addr, int64_t value)
{
   DEBUGOUT << "writeDataAtAddress(" << addr << ", " << value << ")" << std::endl;

   if (addr >= theProgramData.size())
   {
      // Address out of range
      DEBUGOUT << "Address " << addr << " is out of range of program data (write)" << std::endl;
      expandMemory(addr);
   }

   theProgramData[addr] = value;
}

void ElfComputer::expandMemory(int size)
{
   DEBUGOUT << "Expanding memory to " << size << std::endl;
   while(theProgramData.size() < size + 1)
   {
      theProgramData.push_back(0);
   }
}

void ElfComputer::printEmulatorState()
{
   std::cout << std::endl << std::endl << "PC = " << thePc << "\tBase = " << theBase
             << "\tIter = " << theNumIterations << std::endl;
   for(int i = 0; i < theProgramData.size(); i++)
   {
      if ( (i % 10) == 0)
      {
         std::cout << i << ")\t\t";
      }

      std::cout << theProgramData[i] << "\t";

      if ( (i % 10) == 9)
      {
         std::cout << std::endl;
      }
   }

   std::cout << std::endl << std::endl;
}

std::string ElfComputer::disassemble(int address)
{
   int64_t op1 = readDataAtAddress(address);

   int instructionType = op1 % 100;

   std::string retVal;
   retVal += std::to_string(address);
   retVal += ": \t\t";

   switch(instructionType)
   {
   case 1: // addition
   case 2: // multiply
      retVal += disassembleOperand(address, 1);
      retVal += (instructionType == 1 ? " + " : " * ");
      retVal += disassembleOperand(address, 2);
      retVal += " => ";
      retVal += disassembleOperand(address, 3);
      return retVal;

   case 3: // input
      retVal += "INPUT => ";
      retVal += disassembleOperand(address, 1);
      return retVal;

   case 4: // output
      retVal += disassembleOperand(address, 1);
      retVal += " => OUTPUT";
      return retVal;

   case 5: // jump if true: if [x] the jump [y]
   case 6: // jump if false
      retVal += "if ";
      if (instructionType == 6)
      {
         retVal += "! ";
      }
      retVal += disassembleOperand(address, 1);
      retVal += " then ";
      retVal += disassembleOperand(address, 2);
      retVal += " => PC";
      return retVal;


   case 7: // less than
   case 8: // equal to
      retVal += "(";
      retVal += disassembleOperand(address, 1);
      retVal += (instructionType == 7 ? " < " : " == ");
      retVal += disassembleOperand(address, 2);
      retVal += ") => ";
      retVal += disassembleOperand(address, 3);
      return retVal;

   case 9: // set base addr
      retVal += disassembleOperand(address, 1);
      retVal += " => BASE";
      return retVal;

   case 99:
      retVal += "HALT";
      return retVal;
   }
}

std::string ElfComputer::disassembleOperand(int address, int operand)
{
   if ( (operand < 1) || (operand > 3) )
   {
      std::cerr << "Operand invalid: disassembleOperand(" << address << ", " << operand << ")"
                << std::endl;
      exit(1);
   }

   int operandDivisor = operand * 10;
   int addressingMode = ( readDataAtAddress(address) / operandDivisor ) % 10;
   int operandValue = readDataAtAddress(address + operand);

   std::string retVal;
   switch(addressingMode)
   {
   case 0:  // position mode
      retVal += "[";
      retVal += std::to_string(operandValue);
      retVal += "]";
      return retVal;

   case 1:  // immediate mode
      retVal += "#";
      retVal += std::to_string(operandValue);
      return retVal;

   case 2:
      retVal += "[base +";
      retVal += std::to_string(operandValue);;
      return retVal;
   }

}

void ElfComputer::runIteration()
{
// #ifdef DEBUGGING
   //printEmulatorState();
   mvprintw(25,0,"DISASS: %s                                ", disassemble(thePc).c_str());
   refresh();
// #endif

   theNumIterations++;

   if( (thePc < 0) || (thePc >= theProgramData.size()) )
   {
      std::cerr << "PC of " << thePc << " is invalid location" << std::endl;
      exit(1);
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

   int64_t opCode = theProgramData[thePc];

   int64_t instructionDE = opCode %100;
   int64_t operandCMode = (opCode / 100) % 10;
   int64_t operandBMode = (opCode / 1000) % 10;
   int64_t operandAMode = (opCode / 10000) % 10;

   DEBUGOUT << theName << " PC=" << thePc << " opCode=" << opCode << " OpModeC=" << operandCMode
             << ", OpModeB=" << operandBMode << ", OpModeA=" << operandAMode << std::endl;

   switch(instructionDE)
   {
      case 1:  // addition
      case 2:  // multiply
      {
         DEBUGOUT << "Op Code is " << instructionDE << ( instructionDE == 1 ? " (additon)" : " (multiply)") << std::endl;

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int64_t op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));
         int64_t op3 = getOperand(operandAMode, readDataAtAddress(thePc+3));

         DEBUGOUT << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         int64_t resultVal;
         if (instructionDE == 1)
         {
            resultVal = op1 + op2;
            DEBUGOUT << "Addition result " << resultVal << std::endl;
         }
         else
         {
            resultVal = op1 * op2;
            DEBUGOUT << "Multiply result " << resultVal << std::endl;
         }

         writeOperand(operandAMode, readDataAtAddress(thePc+3), resultVal);

         thePc += 4;
         break;
      }

      case 3: // input
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is input" << std::endl;

         if (theInputDataQ == nullptr)
         {
            std::cerr << "Input instruction found, but Q not set on " << theName << std::endl;
            theHaltFlag = true;
            return;
         }


         if (theInputDataQ->size() > 0)
         {
            int64_t value = theInputDataQ->at(0);
            theInputDataQ->erase(theInputDataQ->begin());

            DEBUGOUT << "Read input value " << value << " for " << theName << std::endl;
            writeOperand(operandCMode, readDataAtAddress(thePc+1), value);

            thePc += 2;
         }
         else
         {
            DEBUGOUT << "Input instruction, but no data ready for " << theName << std::endl;
         }

         break;
      }

      case 4: // output
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is output" << std::endl;

         if (theOutputDataQ == nullptr)
         {
            std::cerr << "Output data Q not set for " << theName << std::endl;
            theHaltFlag = true;
            return;
         }

         if (theProgramData.size() < thePc + 1)
         {
            std::cout << "Program data not long enough to store operands" << std::endl;
            theHaltFlag = true;
            return;
         }

         int64_t outputData = getOperand(operandCMode, readDataAtAddress(thePc+1));
         DEBUGOUT << "OUTPUT: " << outputData << std::endl;
         theOutputDataQ->push_back(outputData);
         thePc += 2;
         break;
      }

      case 5: // jump-if-true
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is jump-if-true" << std::endl;

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int64_t op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));

         DEBUGOUT << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (op1)
         {
            DEBUGOUT << "We are jumping to " << thePc << "!" << std::endl;
            thePc = op2;
         }
         else
         {
            DEBUGOUT << "We are not jumping!" << std::endl;
            thePc += 3;
         }
         break;
      }

      case 6: // jump-if-false
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is jump-if-false" << std::endl;

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int64_t op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));

         DEBUGOUT << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (!op1)
         {
           thePc = op2;
           DEBUGOUT << theName << " jumping to " << thePc << "!" << std::endl;
         }
         else
         {
            DEBUGOUT << theName << " not jumping!" << std::endl;
            thePc += 3;
         }
         break;
      }

      case 7: // less-than
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is less-than" << std::endl;

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int64_t op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));

         DEBUGOUT << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (op1 < op2)
         {
            DEBUGOUT << "op1<op2, storing 1 in op3 " << theName << std::endl;
            writeOperand(operandAMode, readDataAtAddress(thePc+3), 1);
         }
         else
         {
            DEBUGOUT << "op1>=op2, storing 1 in op3 " << theName << std::endl;
            writeOperand(operandAMode, readDataAtAddress(thePc+3), 0);
         }

         thePc += 4;
         break;
      }

      case 8: // equal-to
      {
         // Verify program data has arg data
         DEBUGOUT << "Opcode " << opCode << " is equal-to" << std::endl;
         if (theProgramData.size() < thePc + 3)
         {
            std::cout << "Program data not long enough to store operands" << std::endl;
            theHaltFlag = true;
            return;
         }

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int64_t op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));


         DEBUGOUT << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (op1 == op2)
         {
            DEBUGOUT << "op1==op2, storing 1 in op3 " << theName << std::endl;
            writeOperand(operandAMode, readDataAtAddress(thePc+3), 1);
         }
         else
         {
            DEBUGOUT << "op1!=op2, storing 0 in op3 " << theName << std::endl;
            writeOperand(operandAMode, readDataAtAddress(thePc+3), 0);
         }

         thePc += 4;
         break;
      }

      case 9:
      {
         DEBUGOUT << "Opcode " << opCode << " is set base register" << std::endl;

         int64_t op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));

         DEBUGOUT << "Op 1 = " << op1 << std::endl;

         DEBUGOUT << "Base register set to " << op1 << std::endl;
         theBase += op1;

         thePc += 2;
         break;
      }

      case 99:
         DEBUGOUT << "Op Code is 99, end program" << std::endl;
         theHaltFlag = true;
         return;

      default:
         std::cout << "Op Code is " << opCode << ", invalid!" << std::endl;
         theHaltFlag = true;
         return;
   }
}


