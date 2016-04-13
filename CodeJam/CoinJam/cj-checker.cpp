/**
 * This short program was created to check the answers to the CoinJam Code Jam problem
 *
 * It only works on the small data set, not the large data set.  10^32 is really large
 */

#include <iostream>
#include <string>
#include <math.h>

long binaryToBaseN(std::string const & binaryNum, long base)
{
   long retVal = 0;
   int iteration = 0;
   for(auto digit = binaryNum.rbegin(); digit != binaryNum.rend(); digit++)
   {
      std::cerr << "Digit " << *digit << std::endl;
      if (*digit == '1')
      {
         retVal += powl(base, iteration);
      }

      iteration++;
   }

   return retVal;
}

void checkAnswer()
{
   std::string coinJam;
   std::cin >> coinJam;

   bool answerOk = true;
   for(int base = 2; base <= 10; base++)
   {
      int divisor;
      std::cin >> divisor;

      long baseNumber = binaryToBaseN(coinJam, base);

      std::cerr << "CJ of " << coinJam << " in base " << base << " equals " << baseNumber << std::endl;

      if ((baseNumber % divisor) == 0)
      {
         std::cerr << "Divisor " << divisor << " is valid for " << baseNumber << std::endl;
      }
      else
      {
         std::cout << "DIVISOR " << divisor << " IS NOT VALID FOR " << baseNumber << std::endl;
         answerOk = false;

      }
   }

   if (answerOk)
   {
      std::cout << "CJ of " << coinJam << " is valid answer!" << std::endl;
   }
   else
   {
      std::cout << "CJ of " << coinJam << " is WRONG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
   }


}

int main(int argc, char** argv)
{
   // Read the Case #XXX:
   std::string throwAway;

   if (std::cin.eof())
   {
      std::cerr << "First exit" << std::endl;
      return 0;
   }
   std::cin >> throwAway;


   if (std::cin.eof())
   {
      std::cerr << "Second exit" << std::endl;
      return 0;
   }
   std::cin >> throwAway;

   int maxIter = 1000;
   while(maxIter--)
      {
      if (std::cin.eof())
      {
         std::cerr << "Third exit" << std::endl;
         return 0;
      }

      checkAnswer();
   }

   std::cerr << "Fell out of the loop due to max iterations" << std::endl;
   return 0;
}
