#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

/*
--- Day 5: A Maze of Twisty Trampolines, All Alike ---

An urgent interrupt arrives from the CPU: it's trapped in a maze of jump instructions, and it would
like assistance from any programs with spare cycles to help find the exit.

The message includes a list of the offsets for each jump. Jumps are relative: -1 moves to the
previous instruction, and 2 skips the next one. Start at the first instruction in the list. The goal
is to follow the jumps until one leads outside the list.

In addition, these instructions are a little strange; after each jump, the offset of that
instruction increases by 1. So, if you come across an offset of 3, you would move three instructions
 forward, but change it to a 4 for the next time it is encountered.

For example, consider the following list of jump offsets:

0
3
0
1
-3
Positive jumps ("forward") move downward; negative jumps move upward. For legibility in this example, these offset values will be written all on one line, with the current instruction marked in parentheses. The following steps would be taken before an exit is found:

(0) 3  0  1  -3  - before we have taken any steps.
(1) 3  0  1  -3  - jump with offset 0 (that is, don't jump at all). Fortunately, the instruction is then incremented to 1.
 2 (3) 0  1  -3  - step forward because of the instruction we just modified. The first instruction is incremented again, now to 2.
 2  4  0  1 (-3) - jump all the way to the end; leave a 4 behind.
 2 (4) 0  1  -2  - go back to where we just were; increment -3 to -2.
 2  5  0  1  -2  - jump 4 steps forward, escaping the maze.
In this example, the exit is reached in 5 steps.

How many steps does it take to reach the exit?
*/

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
      return 1;
   }

   std::ifstream instructions;
   instructions.open(argv[1], std::ios::in);

   if (!instructions.is_open())
   {
      std::cerr << "Error opening file " << argv[1] << std::endl;
      return 1;
   }

   std::vector<int> codeList;

   while(!instructions.eof())
   {
      std::string textData;
      std::getline(instructions, textData);

      if (textData == "")
        continue;

      codeList.push_back(atoi(textData.c_str()));

      std::cout << ">" << textData << std::endl;
   }

   instructions.close();

   int ip = 0;
   int lastAddress = codeList.size() - 1;
   int cpuClocks = 0;

   while(1)
   {
      cpuClocks++;

      int oldValue = codeList[ip];
      int newIp = oldValue + ip;

      if ( (newIp < 0) || (newIp > lastAddress))
      {
         std::cout << "Instruction at " << ip << " jumps us out of bounds to " << newIp << std::endl;
         break;
      }

      //std::cout << "Executing " << oldValue << " at " << ip << ", jumping to " << newIp << std::endl;
      codeList[ip] += 1;
      ip = newIp;
   }

  std::cout << "Done, CPU clocks = " << cpuClocks << std::endl;
  return 0;
}
