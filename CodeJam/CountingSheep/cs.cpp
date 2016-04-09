#include <iostream>
#include <sstream>


std::string numToString(long long val)
{
   std::ostringstream oss;
   oss << val;
   return oss.str();
}

std::string solveCase()
{
   int iterationLimit = 10000;
   std::string solution;

   long long startingNum;
   std::cin >> startingNum;
   long long currentNum = 0;

   std::string allDigits = "0123456789";
   while(!allDigits.empty())
   {
      currentNum += startingNum;
      std::string currentNumString = numToString(currentNum);

      for(auto it = allDigits.begin(); it != allDigits.end(); it++)
      {
         char searchChar = *it;
         if ( currentNumString.find(searchChar) != std::string::npos)
         {
            // Found the character in the string
            std::cerr << "Found the " << *it << " in " << currentNumString << " (" << allDigits << ")" << std::endl;
            it = allDigits.erase(it);
            it--;


         }
      }

      if (iterationLimit-- == 0)
      {
         return "INSOMNIA";
      }


   }

   std::cerr << "Solution (" << currentNum << ") found after " << 10000 - iterationLimit << " numbers" << std::endl;
   return numToString(currentNum);

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
