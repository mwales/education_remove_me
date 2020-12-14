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

class MaskComputer
{
public:
	MaskComputer();

	void processInput(std::string inputData);

	uint64_t sumAllMemory();

	void dumpAllMemory();

	std::string theCurrentMask;

	std::map<uint64_t, uint64_t> theMemory;

	void memoryWrite(uint64_t addr, uint64_t value, std::string mask);
};

MaskComputer::MaskComputer()
{
}

void MaskComputer::processInput(std::string inputData)
{
	std::vector<std::string> parts = stringSplit(inputData, ' ');


	if (parts[0] == "mask")
	{
		// Updataing mask
		theCurrentMask = parts[2];
	}
	else
	{
		// This is a mem command
		std::vector<std::string> memParts = stringSplit(parts[0], '[');
		std::vector<std::string> memParts2 = stringSplit(memParts[1], ']');

		uint64_t addr = strtoull(memParts2[0].c_str(), NULL, 10);

		uint64_t value = strtoull(parts[2].c_str(), NULL, 10);
		DEBUG << "Setting mem " << addr << " to " << value << std::endl;

		memoryWrite(addr, value, theCurrentMask);
	}

	dumpAllMemory();

}


void MaskComputer::memoryWrite(uint64_t addr, uint64_t value, std::string mask)
{
	DEBUG << "memoryWrite(" << addr << "," << value << "," << mask << ")" << std::endl;
	for(int curLetter = 0; curLetter < mask.size(); curLetter++)
	{
		if (mask[curLetter] == 'X')
		{
			uint64_t bitVal = 1;
		  	bitVal <<= (mask.size() - curLetter - 1);

			std::string mask0 = mask;
			std::string mask1 = mask;
			mask0[curLetter] = '0';
			mask1[curLetter] = '1';
			memoryWrite(addr & ~bitVal, value, mask0);
			memoryWrite(addr, value, mask1);
			return;
		}
	}

	// If we made it this far, there are no floaters in the mask anymore
			
	uint64_t i = 1;
	uint64_t correctAddr = addr;
	for(auto rit = mask.rbegin(); rit != mask.rend(); rit++)
	{
		if (*rit == '0')
		{
			//correctAddr &= ~i;
		}

		if (*rit == '1')
		{
			correctAddr |= i;
		}

		i *= 2;				
	}

	DEBUG << "Writing to " << correctAddr << " val " << value << std::endl;
	theMemory[correctAddr] = value;
}

uint64_t MaskComputer::sumAllMemory()
{
	uint64_t retVal = 0;
	for(auto curMem: theMemory)
	{
		retVal += curMem.second;
	}
	return retVal;
}

void MaskComputer::dumpAllMemory()
{
	uint64_t retVal;
	for(auto curMem: theMemory)
	{
		DEBUG << curMem.first << " = " << curMem.second << std::endl;
	}

	DEBUG << "MASK = " << theCurrentMask << std::endl;
}


int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);
	MaskComputer mc;

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
		mc.processInput(text);
	}

	infile.close();

	uint64_t part1 = mc.sumAllMemory();
	std::cout << "Sum All = " << part1 << std::endl;
	

	return 0;
}
