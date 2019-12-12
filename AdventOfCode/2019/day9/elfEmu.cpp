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

class ElfComputer
{
public:

   ElfComputer(std::vector<int> const & pd, std::string const & name);

   void addInputData(int singleItem);

   void addInputData(std::vector<int> multiItems);

   void runIteration();

   void setInputDataQ(std::vector<int>* inQ);

   void setOutputDataQ(std::vector<int>* outQ);

   bool getOutputDataItem(int* item);

   bool isHalted() { return theHaltFlag; };

protected:

   int getOperand(int operandMode, int operandRawVal) const;

   int readDataAtAddress(int addr) const;

   void writeDataAtAddress(int addr, int value);

   std::vector<int> theProgramData;

   std::vector<int>* theInputDataQ;

   std::vector<int>* theOutputDataQ;

   int thePc;

   std::string theName;

   bool theHaltFlag;

};

ElfComputer::ElfComputer(std::vector<int> const & pd, std::string const & name):
   thePc(0),
   theInputDataQ(nullptr),
   theOutputDataQ(nullptr),
   theName(name),
   theHaltFlag(false)
{
   theProgramData = pd;
}

void ElfComputer::addInputData(int singleItem)
{
   if (theInputDataQ == nullptr)
   {
      std::cerr << "Never setup the input Q on " << theName << std::endl;
      exit(1);
   }

   theInputDataQ->push_back(singleItem);
}

void ElfComputer::addInputData(std::vector<int> multiItems)
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

void ElfComputer::setInputDataQ(std::vector<int>* inQ)
{
   theInputDataQ = inQ;
}

void ElfComputer::setOutputDataQ(std::vector<int>* outQ)
{
   theOutputDataQ = outQ;
}

int ElfComputer::getOperand(int operandMode, int operandRawVal) const
{
   if (operandMode == 0)
   {
      // Position mode
      if ( theProgramData.size() < operandRawVal)
      {
         // Positional parameter with value out-of-range
         std::cerr << "ERROR! Position param " << operandRawVal << " out of range";
         return -1;
      }

      return theProgramData[operandRawVal];
   }
   else
   {
      // Immediate mode
      return operandRawVal;
   }
}

int ElfComputer::readDataAtAddress(int addr) const
{
   if (addr > theProgramData.size())
   {
      // We have to add memory to program storage!
      std::cerr << "Address " << addr << " is out of range of program data (read)" << std::endl;
      exit(1);
   }

   return theProgramData[addr];
}

void ElfComputer::writeDataAtAddress(int addr, int value)
{
   if (addr > theProgramData.size())
   {
      // Address out of range
      std::cerr << "Address " << addr << " is out of range of program data (write)" << std::endl;
      exit(1);
   }

   theProgramData[addr] = value;
}



void ElfComputer::runIteration()
{
   // std::cout << "State: ";
   // printIntList(theProgramData);

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

   int opCode = theProgramData[thePc];

   int instructionDE = opCode %100;
   int operandCMode = (opCode / 100) % 10;
   int operandBMode = (opCode / 1000) % 10;
   int operandAMode = (opCode / 10000) % 10;

   std::cout << theName << " PC=" << thePc << " opCode=" << opCode << " OpModeC=" << operandCMode
             << ", OpModeB=" << operandBMode << ", OpModeA=" << operandAMode << std::endl;

   switch(instructionDE)
   {
      case 1:  // addition
      case 2:  // multiply
      {
         std::cout << "Op Code is " << instructionDE << ( instructionDE == 1 ? " (additon)" : " (multiply)") << std::endl;

         int op1 = getOperand(operandCMode, readDataAtAddress(thePc+1));
         int op2 = getOperand(operandBMode, readDataAtAddress(thePc+2));

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
         int resultPos = readDataAtAddress(thePc+3);
         writeDataAtAddress(resultPos, resultVal);

         thePc += 4;
         break;
      }

      case 3: // input
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is input" << std::endl;

         if (theInputDataQ == nullptr)
         {
            std::cerr << "Input instruction found, but Q not set on " << theName << std::endl;
            theHaltFlag = true;
            return;
         }

         int writeLocation = readDataAtAddress(thePc + 1);

         if (theInputDataQ->size() > 0)
         {
            int value = theInputDataQ->at(0);
            theInputDataQ->erase(theInputDataQ->begin());

            std::cout << "Read input value " << value << " for " << theName << std::endl;
            writeDataAtAddress(writeLocation, value);

            thePc += 2;
         }
         else
         {
            std::cout << "Input instruction, but no data ready for " << theName << std::endl;
         }

         break;
      }

      case 4: // output
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is output" << std::endl;

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

         int outputData = getOperand(operandCMode, theProgramData[thePc+1]);
         std::cout << "OUTPUT: " << outputData << std::endl;
         theOutputDataQ->push_back(outputData);
         thePc += 2;
         break;
      }

      case 5: // jump-if-true
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is jump-if-true" << std::endl;
         if (theProgramData.size() < thePc + 2)
         {
            std::cout << "Program data not long enough to store operands " << theName << std::endl;
            theHaltFlag = true;
            return;
         }

         int op1 = getOperand(operandCMode, theProgramData[thePc + 1]);
         int op2 = getOperand(operandBMode, theProgramData[thePc + 2]);

         std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (op1)
         {
            std::cout << "We are jumping to " << thePc << "!" << std::endl;
            thePc = op2;
         }
         else
         {
            std::cout << "We are not jumping!" << std::endl;
            thePc += 3;
         }
         break;
      }

      case 6: // jump-if-false
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is jump-if-false" << std::endl;
         if (theProgramData.size() < thePc + 2)
         {
            std::cout << "Program data not long enough to store operands " << theName << std::endl;
            theHaltFlag = true;
            return;
         }

         int op1 = getOperand(operandCMode, theProgramData[thePc + 1]);
         int op2 = getOperand(operandBMode, theProgramData[thePc + 2]);

         std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << std::endl;

         if (!op1)
         {
            std::cout << theName << " jumping to " << thePc << "!" << std::endl;
            thePc = op2;
         }
         else
         {
            std::cout << theName << " not jumping!" << std::endl;
            thePc += 3;
         }
         break;
      }

      case 7: // less-than
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is less-than" << std::endl;
         if (theProgramData.size() < thePc + 3)
         {
            std::cout << "Program data not long enough to store operands " << theName << std::endl;
            theHaltFlag = true;
            return;
         }

         int op1 = getOperand(operandCMode, theProgramData[thePc + 1]);
         int op2 = getOperand(operandBMode, theProgramData[thePc + 2]);
         int op3 = theProgramData[thePc + 3];

         std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << " and Op 3 = "
                   << op3 << std::endl;

         if (op1 < op2)
         {
            std::cout << "op1<op2, storing 1 in op3 " << theName << std::endl;
            writeDataAtAddress(op3, 1);
         }
         else
         {
            writeDataAtAddress(op3, 0);
         }
         thePc += 4;
         break;
      }

      case 8: // equal-to
      {
         // Verify program data has arg data
         std::cout << "Opcode " << opCode << " is equal-to" << std::endl;
         if (theProgramData.size() < thePc + 3)
         {
            std::cout << "Program data not long enough to store operands" << std::endl;
            theHaltFlag = true;
            return;
         }

         int op1 = getOperand(operandCMode, theProgramData[thePc + 1]);
         int op2 = getOperand(operandBMode, theProgramData[thePc + 2]);
         int op3 = theProgramData[thePc + 3];

         std::cout << "Op 1 = " << op1 << " and Op 2 = " << op2 << " and Op 3 = "
                   << op3 << std::endl;

         if (op1 == op2)
         {
            std::cout << "op1==op2, storing 1 in op3 " << theName << std::endl;
            writeDataAtAddress(op3, 1);
         }
         else
         {
            writeDataAtAddress(op3, 0);
         }

         thePc += 4;
         break;
      }

      case 99:
         std::cout << "Op Code is 99, end program" << std::endl;
         theHaltFlag = true;
         return;

      default:
         std::cout << "Op Code is " << opCode << ", invalid!" << std::endl;
         theHaltFlag = true;
         return;
   }
}

std::vector<std::vector<int> > getAllCombos(int num)
{
   std::vector<std::vector<int> > retVal;
   std::vector<int> singleEntry;

   if (num == 1)
   {
      singleEntry.push_back(0);
      retVal.push_back(singleEntry);
      return retVal;
   }

   std::vector<std::vector<int> > recursiveEntries = getAllCombos(num - 1);
   for(int i = 0; i < num; i++)
   {
      for(auto entryIt = recursiveEntries.begin(); entryIt != recursiveEntries.end(); entryIt++)
      {
         // For every entry, insert our number in every position from beginning to end
         singleEntry = *entryIt;
         singleEntry.insert(singleEntry.begin() + i, num - 1);
         retVal.push_back(singleEntry);
      }
   }

   return retVal;
}


int main(int argc, char** argv)
{
   std::vector<std::vector<int> > vals = getAllCombos(5);

   std::string programString;
   std::cin >> programString;
   std::vector<std::string> tokens = split(programString, ",");
   std::vector<int> programData = strListToInt(tokens);

   int maxThrust = 0;
   std::vector<int> maxThrustInputs;

   for(auto entryIt = vals.begin(); entryIt != vals.end(); entryIt++)
   {

      std::cout << "Amp configuration: ";
      printIntList(programData);

      ElfComputer ampA(programData, "AmpA");
      ElfComputer ampB(programData, "AmpB");
      ElfComputer ampC(programData, "AmpC");
      ElfComputer ampD(programData, "AmpD");
      ElfComputer ampE(programData, "AmpE");

      std::vector<int> inputA;
      inputA.push_back(entryIt->at(0));
      inputA.push_back(0);    // amplifier initial value

      std::vector<int> inputB;
      inputB.push_back(entryIt->at(1));

      std::vector<int> inputC;
      inputC.push_back(entryIt->at(2));

      std::vector<int> inputD;
      inputD.push_back(entryIt->at(3));

      std::vector<int> inputE;
      inputE.push_back(entryIt->at(4));

      std::vector<int> outputQ;

      // Plumb all the amps together
      ampA.setInputDataQ(&inputA);
      ampA.setOutputDataQ(&inputB);

      ampB.setInputDataQ(&inputB);
      ampB.setOutputDataQ(&inputC);

      ampC.setInputDataQ(&inputC);
      ampC.setOutputDataQ(&inputD);

      ampD.setInputDataQ(&inputD);
      ampD.setOutputDataQ(&inputE);

      ampE.setInputDataQ(&inputE);
      ampE.setOutputDataQ(&outputQ);

      bool totalHaltFlag = false;
      while(!totalHaltFlag)
      {
         ampA.runIteration();
         ampB.runIteration();
         ampC.runIteration();
         ampD.runIteration();
         ampE.runIteration();

         totalHaltFlag = ampA.isHalted() && ampB.isHalted() && ampC.isHalted() &&
                         ampD.isHalted() && ampE.isHalted();
      }

      std::cout << "Output: ";
      printIntList(outputQ);
      std::cout << std::endl << std::endl;

      if (outputQ.size() == 0)
      {
         std::cerr << "No output to be found" << std::endl;
         break;
      }

      if (maxThrust < outputQ[0])
      {
         maxThrust = outputQ[0];
         maxThrustInputs = *entryIt;

         std::cout << "Thrust " << maxThrust << " generated from ";
         printIntList(maxThrustInputs);
      }
   }


   std::cout << "Thrust " << maxThrust << " generated from ";
   printIntList(maxThrustInputs);




	return 0;
}
