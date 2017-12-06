#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdlib.h>

// Compilation: g++ --std=c++14 p1.cpp

/*
--- Day 6: Memory Reallocation ---

A debugger program here is having an issue: it is trying to repair a memory reallocation routine,
but it keeps getting stuck in an infinite loop.

In this area, there are sixteen memory banks; each memory bank can hold any number of blocks. The
goal of the reallocation routine is to balance the blocks between the memory banks.

The reallocation routine operates in cycles. In each cycle, it finds the memory bank with the most
blocks (ties won by the lowest-numbered memory bank) and redistributes those blocks among the banks. To do this, it removes all of the blocks from the selected bank, then moves to the next (by index) memory bank and inserts one of the blocks. It continues doing this until it runs out of blocks; if it reaches the last memory bank, it wraps around to the first one.

The debugger would like to know how many redistributions can be done before a blocks-in-banks
configuration is produced that has been seen before.

For example, imagine a scenario with only four memory banks:

- The banks start with 0, 2, 7, and 0 blocks. The third bank has the most blocks, so it is chosen for
  redistribution.

- Starting with the next bank (the fourth bank) and then continuing to the first
  bank, the second bank, and so on, the 7 blocks are spread out over the memory banks. The fourth,
  first, and second banks get two blocks each, and the third bank gets one back. The final result
  looks like this: 2 4 1 2.

- Next, the second bank is chosen because it contains the most blocks (four). Because there are four
  memory banks, each gets one block. The result is: 3 1 2 3.
- Now, there is a tie between the first and fourth memory banks, both of which have three blocks.
  The first bank wins the tie, and its three blocks are distributed evenly over the other three
  banks, leaving it with none: 0 2 3 4.
- The fourth bank is chosen, and its four blocks are distributed such that each of the four banks
  receives one: 1 3 4 1.
- The third bank is chosen, and the same thing happens: 2 4 1 2.

At this point, we've reached a state we've seen before: 2 4 1 2 was already seen. The infinite loop
is detected after the fifth block redistribution cycle, and so the answer in this example is 5.

Given the initial block counts in your puzzle input, how many redistribution cycles must be
completed before a configuration is produced that has been seen before?

Your puzzle answer was 6681.

--- Part Two ---

Out of curiosity, the debugger would also like to know the size of the loop: starting from a state
that has already been seen, how many block redistribution cycles must be performed before that same
state is seen again?

In the example above, 2 4 1 2 is seen again after four cycles, and so the answer in that example
would be 4.

How many cycles are in the infinite loop that arises from the configuration in your puzzle input?

Your puzzle answer was 2392.
*/

std::string listToString(std::vector<int> const & nums)
{
   std::string retVal;
   for(std::vector<int>::const_iterator it = nums.begin(); it != nums.end(); it++)
   {
      retVal += std::to_string(*it);
      retVal += ",";
   }

   return retVal;
}

int findMaxIndex(std::vector<int> const & nums)
{
   int retVal = 0;
   int curIndex = 0;
   int curMax = 0;
   for(std::vector<int>::const_iterator it = nums.begin(); it != nums.end(); it++)
   {
      if (*it > curMax)
      {
         curMax = *it;
         retVal = curIndex;
      }

      curIndex++;
   }

   return retVal;
}

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
      return 1;
   }

   std::ifstream dataFile;
   dataFile.open(argv[1], std::ios::in);

   if (!dataFile.is_open())
   {
      std::cerr << "Error opening the file " << argv[1] << std::endl;
      return 1;
   }

   std::vector<int> numberList;

   while (!dataFile.eof())
   {
      std::string nextNum;
      dataFile >> nextNum;



      if (nextNum == "")
      {
         continue;
      }

      std::cout << "Number " << nextNum << std::endl;

      numberList.push_back(atoi(nextNum.c_str()));
   }

   dataFile.close();

   std::vector<std::string> history;
   int iterations = 1;
   history.push_back(listToString(numberList));
   std::cout << "History << " << listToString(numberList) << std::endl;

   while(1)
   {
      // Redistribute the list
      int redistIndex = findMaxIndex(numberList);
      int redistValue = numberList[redistIndex];
      numberList[redistIndex] = 0;
      for(int i = 1; i <= redistValue; i++)
      {
         numberList[(redistIndex + i) % numberList.size()]++;
      }

      // Determine new string to add
      std::string newHistVal = listToString(numberList);
      std::cout << iterations << " History " << newHistVal << std::endl;

      // Is it a dupe
      if (std::find(history.begin(), history.end(), newHistVal) != history.end())
      {
         std::cout << "Uh oh, this iteration caused a dupe!" << std::endl;
         int repeatOf = std::find(history.begin(), history.end(), newHistVal) - history.begin();
         std::cout << "It is a repeat of " << repeatOf << ", which was " << iterations - repeatOf
                   << " iterations ago" << std::endl;
         break;
      }

      // Add it to the list
      history.push_back(newHistVal);

      iterations++;
   }

   std::cout << "Done" << std::endl;

   return 0;
}
