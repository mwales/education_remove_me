#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>
#include <stdlib.h>

// Problem from Google Code Jam 2017 Qualification Round

// Problem
// 
// Last year, the Infinite House of Pancakes introduced a new kind of pancake. It has a happy face made of chocolate chips on one side (the "happy side"), and nothing on the other side (the "blank side").
// 
// You are the head cook on duty. The pancakes are cooked in a single row over a hot surface. As part of its infinite efforts to maximize efficiency, the House has recently given you an oversized pancake flipper that flips exactly K consecutive pancakes. That is, in that range of K pancakes, it changes every happy-side pancake to a blank-side pancake, and vice versa; it does not change the left-to-right order of those pancakes.

// You cannot flip fewer than K pancakes at a time with the flipper, even at the ends of the row (since there are raised borders on both sides of the cooking surface). For example, you can flip the first K pancakes, but not the first K - 1 pancakes.
// 
// Your apprentice cook, who is still learning the job, just used the old-fashioned single-pancake flipper to flip some individual pancakes and then ran to the restroom with it, right before the time when customers come to visit the kitchen. You only have the oversized pancake flipper left, and you need to use it quickly to leave all the cooking pancakes happy side up, so that the customers leave feeling happy with their visit.
// 
// Given the current state of the pancakes, calculate the minimum number of uses of the oversized pancake flipper needed to leave all pancakes happy side up, or state that there is no way to do it.
// 
// Input
// 
// The first line of the input gives the number of test cases, T. T test cases follow. Each consists of one line with a string S and an integer K. S represents the row of pancakes: each of its characters is either + (which represents a pancake that is initially happy side up) or - (which represents a pancake that is initially blank side up).
// 
// Output
// 
// For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is either IMPOSSIBLE if there is no way to get all the pancakes happy side up, or an integer representing the the minimum number of times you will need to use the oversized pancake flipper to do it.

/**
 * Method that flips the pancakes in a string of pancake states
 *
 * @param pos Position of the flipper (left edge)
 * @param flipper Width of the pancake flipper
 * @param stateBefore This is an input and output.  Its a string of the state of all the pancakes
 *                    passed by reference, and they get flipped during call.
 */
void flipCakes(int pos, int flipper, std::string* stateBefore)
{
   if ( (pos < 0) || ((pos + flipper) > stateBefore->length()) )
   {
      std::cerr << "Illegal cake flipping!" << std::endl;
      exit(1);
   }

   for (int i = 0; i < flipper; i++)
   {
      if ((*stateBefore)[pos] == '-')
      {
         (*stateBefore)[pos] = '+';
      }
      else
      {
         (*stateBefore)[pos] = '-';
      }

      pos++;
   }
}

/**
 * Returns the string of numPancakes in the happy state
 */
std::string happyDay(int numCakes)
{
   std::string retVal;
   for(int i = 0; i < numCakes; i++)
   {
      retVal += '+';
   }

   return retVal;
}

std::string solveCase()
{
   std::string pancakeStates;
   std::cin >> pancakeStates;

   int flipperSize;
   std::cin >> flipperSize;

   std::cerr << "State=" << pancakeStates << " and size=" << flipperSize << std::endl;

   // Just start on the left side of the row of pancakes, if the left most pancake needs to be
   // flipped, we flip it with the left edge of the flipper.
   int numFlips = 0;
   for(int i = 0; i <= pancakeStates.length() - flipperSize; i++)
   {
      if (pancakeStates[i] == '-')
      {
         // Need to flip!
         flipCakes(i, flipperSize, &pancakeStates);
         numFlips++;
      }
   }

   // After all flipping done, they will either be all happy side up, or cooking impossible
   if (pancakeStates == happyDay(pancakeStates.length()))
   {
      std::ostringstream oss;
      oss << numFlips;
      return oss.str();
   }
   else
   {
      return "IMPOSSIBLE";
   }


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
