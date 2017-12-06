#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

/*
Now, the jumps are even stranger: after each jump, if the offset was three or more, instead decrease it by 1. Otherwise, increase it by 1 as before.

Using this rule with the above example, the process now takes 10 steps, and the offset values after finding the exit are left as 2 3 2 3 -1.

How many steps does it now take to reach the exit?
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
      int newIp = ip + oldValue;

      if ( (newIp < 0) || (newIp > lastAddress))
      {
         std::cout << "Instruction at " << ip << " jumps us out of bounds to " << newIp << std::endl;
         break;
      }

      //std::cout << "Executing " << oldValue << " at " << ip << ", jumping to " << newIp << std::endl;

      // Modified post-increment/decrement behavior for part2
      if (codeList[ip] >= 3)
      {
         codeList[ip] -= 1;
      }
      else
      {
         codeList[ip] += 1;
      }

      ip = newIp;
   }

  std::cout << "Done, CPU clocks = " << cpuClocks << std::endl;
  return 0;
}
