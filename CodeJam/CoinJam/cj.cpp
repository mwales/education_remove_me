#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

std::string iGotASolution(std::string const & yourADick, std::vector<long> const & southCarolinaWhatsUp)
{
   std::ostringstream oss;
   oss << yourADick;

   for(auto const & singBase : southCarolinaWhatsUp)
   {
      oss << " " << singBase;
   }

   return oss.str();
}

long nonTrivialDivisor(long num)
{
   for (int i = 2; i < sqrtl(num) + 1; i++)
   {
      if (num % i == 0)
         return i;
   }

   return -1;
}

std::vector<long> convertToAllBases(std::string binary)
{
   std::vector<long> retVals;
   for(int i = 2; i <= 10; i++)
   {
      retVals.push_back(strtol(binary.c_str(), NULL, i));
      std::cerr << "The number " << binary << " in base " << i << " is " << strtol(binary.c_str(), NULL, i) << std::endl;
   }

   return retVals;
}

std::string toBinaryString(long num)
{
   std::string retVal;
   while (num != 0)
   {
      int digit = num % 2;
      num >>= 1;
      retVal.insert(0, (digit == 0 ? "0" : "1"));

   }

   return retVal;
}

std::string frontPaddedNumber(long num, int width)
{
   std::ostringstream oss;
   oss << toBinaryString(num);
   std::string bareNumber = oss.str();

   int frontPaddingReq = width - bareNumber.size();
   std::string retVal = bareNumber;
   for(int i = 0; i < frontPaddingReq; i++)
   {
      retVal = "0" + retVal;
   }

   return retVal;
}

std::vector<std::string> solveCase()
{
   std::vector<std::string> retVal;

   int digitsWide;
   int coinsRequired;
   std::cin >> digitsWide;
   std::cin >> coinsRequired;

   // First and last digits have to be 1
   long maxNum = pow(2, digitsWide - 2);
   std::cerr << "Number of trials possible " << maxNum << std::endl;

   for(int i = 0; i < maxNum; i++)
   {
      std::string possibleNumber = "1";
      possibleNumber += frontPaddedNumber(i, digitsWide - 2);
      possibleNumber += "1";

      std::cerr << "CJ trial " << possibleNumber << std::endl;

      std::vector<long> diffBases = convertToAllBases(possibleNumber);

      bool isPrime = false;
      std::vector<long> divisors;
      for(auto&& singleBase : diffBases)
      {
         long ntd = nonTrivialDivisor(singleBase);
         if (ntd == -1)
         {
            // This shit is prime, wtf!!!!
            std::cerr << "Found a prime " << singleBase << std::endl;
            isPrime = true;
            continue;
         }
         else
         {
            std::cerr << "NTD of " << singleBase << " is " << ntd << std::endl;
         }

         divisors.push_back(ntd);
      }

      if (!isPrime)
      {
         std::cerr << "Found a jam coin " << possibleNumber << "!!!!" << std::endl;
         retVal.push_back(iGotASolution(possibleNumber, divisors));
         coinsRequired--;
      }

      if (!coinsRequired)
      {
         // We are done, yeah!
         std::cerr << "Found enough jam coins!" << std::endl;
         break;
      }
   }


   return retVal;
}

int main(int, char**)
{
   int numCases = 0;
   std::cin >> numCases;

   for(int i = 0; i < numCases; i++)
   {
      std::cout << "Case #" << i+1 << ":" << std::endl;
      std::vector<std::string> solutions = solveCase();
      for (auto&& solLine : solutions)
      {
         std::cout << solLine << std::endl;
      }
   }

   return 0;
}
