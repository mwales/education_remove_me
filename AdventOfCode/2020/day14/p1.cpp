#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>

#include "../customstuff.h"

#define AOC_DEBUG 1
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

	uint64_t theSetters;
	uint64_t theClearers;

	std::map<uint64_t, uint64_t> theMemory;
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
		theSetters = 0;
		theClearers = 0;
		
		uint64_t i = 1;
		for(auto rit = parts[2].rbegin(); rit != parts[2].rend(); rit++)
		{
			if (*rit == '1')
			{
				theSetters |= i;
			}

			if (*rit == '0')
			{
				theClearers |= i;
			}

			i *= 2;				
		}
	}
	else
	{
		// This is a mem command
		std::vector<std::string> memParts = stringSplit(parts[0], '[');
		std::vector<std::string> memParts2 = stringSplit(memParts[1], ']');

		uint64_t addr = strtoull(memParts2[0].c_str(), NULL, 10);

		uint64_t value = strtoull(parts[2].c_str(), NULL, 10);
		DEBUG << "Setting mem " << addr << " to " << value << std::endl;

		value |= theSetters;
		value &= ~theClearers;

		DEBUG << "mem " << addr << " is now " << value << std::endl;

		theMemory[addr] = value;
	}

	dumpAllMemory();

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
	DEBUG << "SETR = " << theSetters << std::endl;
	DEBUG << "CLRR = " << theClearers << std::endl;
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
