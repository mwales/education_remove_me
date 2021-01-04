#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

//#define AOC_DEBUG 1
#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

typedef struct GciStruct
{
	std::string theOpCode;

	int theOperand;

	int theExecCount;
} Gci;

class GameConsole
{
public:
	GameConsole();

	void addInstruction(std::string const & newIns);

	void printState();

	void runMachine();

	bool singleStep();

	void resetState();

	void disassemble();

	int thePc;

	int theAccum;

	std::vector<Gci*> theProgram;

	// 0 = not terminated, 1 = normal, 2 = loop
	int theTerminationCode;
};

GameConsole::GameConsole()
{
	thePc = 0;
	theAccum = 0;
}

void GameConsole::addInstruction(std::string const & newIns)
{
	std::vector<std::string> parts = stringSplit(newIns, ' ');

	Gci* instruction = new Gci();
	instruction->theOpCode = parts[0];
	instruction->theOperand = atoi(parts[1].c_str());
	instruction->theExecCount = 0;

	theProgram.push_back(instruction);
}

void GameConsole::printState()
{
	DEBUG << "PC    = " << thePc << std::endl;;
	DEBUG << "ACCUM = " << theAccum << std::endl;
}

void GameConsole::runMachine()
{
	bool keepExec = true;
	while(keepExec)
	{
		printState();
		keepExec = singleStep();
	}

}

bool GameConsole::singleStep()
{
	if ( (thePc < 0) || (thePc >= theProgram.size()))
	{
		DEBUG << "Program PC = " << thePc << " out of prgoram space" << std::endl;
		theTerminationCode = 1;
		return false;
	}

	Gci* curIns = theProgram[thePc];
	
	if (curIns->theExecCount == 1)
	{
		DEBUG << "Program loop detected, exec halted" << std::endl;
		theTerminationCode = 2;
		return false;
	}

	curIns->theExecCount++;

	if (curIns->theOpCode == "nop")
	{
		DEBUG << thePc << "] NOP" << std::endl;
		thePc += 1;
	}

	if (curIns->theOpCode == "acc")
	{
		DEBUG << thePc << "] acc " << curIns->theOperand << std::endl;
		theAccum += curIns->theOperand;
		thePc += 1;
	}
	
	if (curIns->theOpCode == "jmp")
	{
		DEBUG << thePc << "] jmp " << curIns->theOperand << std::endl;
		thePc += curIns->theOperand;
	}

	return true;
}

void GameConsole::resetState()
{
	thePc = 0;
	theAccum = 0;
	for(auto curIns: theProgram)
	{
		curIns->theExecCount = 0;
	}
	theTerminationCode = 0;
}

void GameConsole::disassemble()
{
	int i = 0;
	for(auto curIns: theProgram)
	{
		if (i == thePc)
		{
			DEBUG << "==> \t" << curIns->theOpCode << "\t" << curIns->theOperand << std::endl;
		}
		else
		{
			DEBUG << i << "\t" << curIns->theOpCode << "\t" << curIns->theOperand << std::endl;
		}
		i++;
	}
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	GameConsole gc;

	for(auto curLine: fileData)
	{
		gc.addInstruction(curLine);
	}

	gc.runMachine();

	gc.printState();
	std::cout << "Part 1 ACCUM = " << gc.theAccum << std::endl;

	for(int i = 0; i < gc.theProgram.size(); i++)
	{
		gc.resetState();
		std::string restoreOpCode;

		DEBUG << "Corrupting instruction " << i << ": " << gc.theProgram[i]->theOpCode << " " 
		      << gc.theProgram[i]->theOperand << std::endl;

		if (gc.theProgram[i]->theOpCode == "acc")
		{
			DEBUG << "Skipping the corrupt iteration, acc instruction" << std::endl;
			continue;
		}

		if (gc.theProgram[i]->theOpCode == "nop")
		{
			DEBUG << "Corrupting a nop into a jmp" << std::endl;
			gc.theProgram[i]->theOpCode = "jmp";
			restoreOpCode = "nop";
		}
		else
		{
			DEBUG << "Corrupting a jmp into a nop" << std::endl;
			gc.theProgram[i]->theOpCode = "nop";
			restoreOpCode = "jmp";
		}

		gc.disassemble();

		gc.runMachine();

		if (gc.theTerminationCode == 1)
		{
			std::cout << "Part 2: normal exit with accum = " << gc.theAccum << std::endl;
			break;
		}

		gc.theProgram[i]->theOpCode = restoreOpCode;
	}

	return 0;
}
