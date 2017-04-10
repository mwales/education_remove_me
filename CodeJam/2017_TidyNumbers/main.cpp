#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>

// 2017 Google Code Jam Qualification Round - Tidy Numbers

// Problem
// 
// Tatiana likes to keep things tidy. Her toys are sorted from smallest to largest, her pencils are sorted from shortest to longest and her computers from oldest to newest. One day, when practicing her counting skills, she noticed that some integers, when written in base 10 with no leading zeroes, have their digits sorted in non-decreasing order. Some examples of this are 8, 123, 555, and 224488. She decided to call these numbers tidy. Numbers that do not have this property, like 20, 321, 495 and 999990, are not tidy.
// 
// She just finished counting all positive integers in ascending order from 1 to N. What was the last tidy number she counted?
// 
// Input
// 
// The first line of the input gives the number of test cases, T. T lines follow. Each line describes a test case with a single integer N, the last number counted by Tatiana.
// 
// Output
// 
// For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is the last tidy number counted by Tatiana.

// When I first looked at the problem I thought I would have to use a big number library to
// complete it, which is kinda a PITA for me.  I don't usually use big numbers when I'm doing C++
// development, so it is extra time-consuming for me.

// Fortunately, given the simplicity of the math operations involved, I was able to create my own
// functions to help my efforts here and operate on the base-10 strings

std::string removeLeadingZeros(std::string val)
{
   std::string retVal;
   for(auto it = val.begin(); it != val.end(); it++)
   {
      if (*it != '0')
      {
         retVal += *it;
      }
   }

   return retVal;
}

std::string subtractAtPos(std::string origVal, int pos)
{
   if (origVal[pos] == '0')
   {
      // Subtract 1 from previous digit, and then convert everything after to 9's
      origVal = subtractAtPos(origVal, pos - 1);

   }
   else
   {
      // Subtract 1 from this digit since it is 1 or greater, then convert everything after to 9's
      origVal[pos] = origVal[pos] - 1;
   }

   int i = pos + 1;
   while(i < origVal.length())
   {
      origVal[i++] = '9';
   }

   return origVal;
}


bool isTidy(std::string val)
{
   char largestSoFar = '9';

   for(auto curLetter = val.rbegin(); curLetter != val.rend(); curLetter++)
   {
      if (*curLetter > largestSoFar)
      {
         return false;
      }

      largestSoFar = *curLetter;
   }

   return true;
}

int positionNotTidy(std::string val)
{
   char smallestSoFar = '0';

   for(int i = 0; i < val.length(); i++)
   {
      if (val[i] >= smallestSoFar)
      {
         smallestSoFar = val[i];
      }
      else
      {
         return i;
      }
   }

   return -1;
}

std::string solveCase()
{
   std::string largeNumber;
   std::cin >> largeNumber;

   //std::cerr << largeNumber << " is " << (isTidy(largeNumber) ? "tidy" : "NOT tidy") << " at " << positionNotTidy(largeNumber) << std::endl;

   while (!isTidy(largeNumber))
   {
      largeNumber = subtractAtPos(largeNumber, positionNotTidy(largeNumber) - 1);
   }

   return removeLeadingZeros(largeNumber);
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
