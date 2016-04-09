#include <iostream>
#include <sstream>
#include <string>

int findFlipPoint(std::string const & stack)
{
   for(int i = stack.size() - 1; i >= 0; i--)
   {
      if (stack[i] == '-')
      {
         return i;
      }
   }

   return -1;
}

std::string flipCakes(std::string const & stack, int pos)
{
   std::string retVal;

   for(int i = 0; i < stack.size(); i++)
   {
      if (i <= pos)
      {
         // flip
         retVal += ( stack[i] == '+' ? '-' : '+' );
      }
      else
      {
         // No flip
         retVal += stack[i];
      }
   }

   return retVal;
}


std::string solveCase()
{
   std::string pStack;
   std::cin >> pStack;

   int flipIndex = 0;
   int numFlips = 0;
   do
   {
      flipIndex = findFlipPoint(pStack);
      std::cerr << "For " << pStack << " Flip pos " << findFlipPoint(pStack) << std::endl;

      if (flipIndex != -1)
      {
         numFlips++;
         pStack = flipCakes(pStack, flipIndex);
      }

   } while (flipIndex != -1);

   std::ostringstream oss;
   oss << numFlips;
   return oss.str();
}

int main(int, char**)
{
   int numCases = 0;
   std::cin >> numCases;

   for(int i = 0; i < numCases; i++)
   {
      std::cout << "Case #" << i+1 << ": " << solveCase() << std::endl;
   }

   return 0;
}
