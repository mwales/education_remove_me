#ifndef ELF_COMPUTER_H
#define ELF_COMPUTER_H

#include <iostream>
#include <vector>

//#define DEBUGGING

#ifdef DEBUGGING
   #define DEBUGOUT std::cout
#else
   #define DEBUGOUT if(0) std::cout
#endif

typedef std::vector<int64_t> ElfCode;

std::vector<std::string> split(std::string data, std::string delimeter);

std::vector<int64_t> strListToInt(std::vector<std::string> const & data);

void printIntList(std::vector<int64_t> const & data);

class ElfComputer
{
public:

   ElfComputer(std::vector<int64_t> const & pd, std::string const & name);

   void addInputData(int64_t singleItem);

   void addInputData(std::vector<int64_t> multiItems);

   void runIteration();

   void setInputDataQ(std::vector<int64_t>* inQ);

   void setOutputDataQ(std::vector<int64_t>* outQ);

   bool getOutputDataItem(int64_t* item);

   bool isHalted() { return theHaltFlag; };

   std::string disassemble(int address);

protected:

   int64_t getOperand(int64_t operandMode, int64_t operandRawVal);

   void writeOperand(int64_t operandMode, int64_t operandRawVal, int64_t writeVal);

   std::string disassembleOperand(int address, int operand);

   int64_t readDataAtAddress(int addr);

   void writeDataAtAddress(int addr, int64_t value);

   void expandMemory(int size);

   void printEmulatorState();

   std::vector<int64_t> theProgramData;

   std::vector<int64_t>* theInputDataQ;

   std::vector<int64_t>* theOutputDataQ;

   int thePc;

   int theBase;

   std::string theName;

   bool theHaltFlag;

   int theNumIterations;

};

#endif // ELF_COMPUTER_H
