#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<stdlib.h>

/*
--- Part Two ---

The logic you've constructed forms a single round of the Knot Hash algorithm; running the full thing
requires many of these rounds. Some input and output processing is also required.

First, from now on, your input should be taken not as a list of numbers, but as a string of bytes
instead. Unless otherwise specified, convert characters to bytes using their ASCII codes. This will
allow you to handle arbitrary ASCII strings, and it also ensures that your input lengths are never
larger than 255. For example, if you are given 1,2,3, you should convert it to the ASCII codes for
each character: 49,44,50,44,51.

Once you have determined the sequence of lengths to use, add the following lengths to the end of the
sequence: 17, 31, 73, 47, 23. For example, if you are given 1,2,3, your final sequence of lengths
should be 49,44,50,44,51,17,31,73,47,23 (the ASCII codes from the input string combined with the
standard length suffix values).

Second, instead of merely running one round like you did above, run a total of 64 rounds, using the
same length sequence in each round. The current position and skip size should be preserved between
rounds. For example, if the previous example was your first round, you would start your second round
with the same length sequence (3, 4, 1, 5, 17, 31, 73, 47, 23, now assuming they came from ASCII
codes and include the suffix), but start with the previous round's current position (4) and skip
size (4).

Once the rounds are complete, you will be left with the numbers from 0 to 255 in some order, called
the sparse hash. Your next task is to reduce these to a list of only 16 numbers called the dense
hash. To do this, use numeric bitwise XOR to combine each consecutive block of 16 numbers in the
sparse hash (there are 16 such blocks in a list of 256 numbers). So, the first element in the dense
hash is the first sixteen elements of the sparse hash XOR'd together, the second element in the
dense hash is the second sixteen elements of the sparse hash XOR'd together, etc.

For example, if the first sixteen elements of your sparse hash are as shown below, and the XOR
operator is ^, you would calculate the first output number like this:

65 ^ 27 ^ 9 ^ 1 ^ 4 ^ 3 ^ 40 ^ 50 ^ 91 ^ 7 ^ 6 ^ 0 ^ 2 ^ 5 ^ 68 ^ 22 = 64

Perform this operation on each of the sixteen blocks of sixteen numbers in your sparse hash to
determine the sixteen numbers in your dense hash.

Finally, the standard way to represent a Knot Hash is as a single hexadecimal string; the final
output is the dense hash in hexadecimal notation. Because each number in your dense hash will be
between 0 and 255 (inclusive), always represent each number as two hexadecimal digits (including a
leading zero as necessary). So, if your first three numbers are 64, 7, 255, they correspond to the
hexadecimal numbers 40, 07, ff, and so the first six characters of the hash would be 4007ff. Because
every Knot Hash is sixteen such numbers, the hexadecimal representation is always 32 hexadecimal
digits (0-f) long.

Here are some example hashes:

The empty string becomes a2582a3a0e66e6e86e3812dcb672a272.
AoC 2017 becomes 33efeb34ea91902bb2f59c9920caa6cd.
1,2,3 becomes 3efbe78a8d82f29979031a4aa0b16a9d.
1,2,4 becomes 63960835bcdc130f0b66d7ff4f6a5a8e.

Treating your puzzle input as a string of ASCII characters, what is the Knot Hash of your puzzle
input? Ignore any leading or trailing whitespace you might encounter.
*/

void sparseToDenseHash(std::vector<int> const & sparse, std::vector<int> & dense)
{
   int curPos = 0;
   for(int i = 0; i < 16; i++)
   {
      uint8_t val = 0;
      for(int j = 0; j < 16; j++)
      {
         val ^= sparse[curPos++];
      }

      dense.push_back(val);
   }
}

std::string intToHex(uint8_t val)
{
   std::string retVal;
   if (val / 16 >= 10)
   {
      retVal += 'a' + val/16 - 10;
   }
   else
   {
      retVal += '0' + val/16;
   }

   if (val % 16 >= 10)
   {
      retVal += 'a' + val%16 - 10;
   }
   else
   {
      retVal += '0' + val%16;
   }

   return retVal;
}

std::string numListToHexString(std::vector<int> const & list)
{
   std::string retVal;
   for(std::vector<int>::const_iterator it = list.begin(); it != list.end(); it++)
   {
      retVal += intToHex(*it);
   }

   return retVal;
}

std::string numListToString(std::vector<int> const & list)
{
   std::ostringstream oss;
   for(std::vector<int>::const_iterator it = list.begin(); it != list.end(); it++)
   {
      oss << *it << ", ";
   }

   return oss.str();
}

std::vector<int> stringToNumList(std::string text)
{
   std::vector<int> retVal;
   for(std::string::iterator it = text.begin(); it != text.end(); it++)
   {
      retVal.push_back( (uint8_t) *it);
   }

   return retVal;
}

void reverseSubList(std::vector<int> & numList, int pos, int revSize)
{
   std::vector<int> oldOrder;
   for(int i = pos; i < (pos + revSize); i++)
   {
      oldOrder.push_back(numList[i % numList.size()]);
   }

   std::vector<int>::reverse_iterator revIt = oldOrder.rbegin();
   for(int i = pos; i < (pos + revSize); i++)
   {
      numList[i % numList.size()] = *revIt;
      revIt++;
   }
}

std::vector<int> stringToIntList(std::string listText)
{
   std::vector<int> retVal;
   std::string numString;
   for(std::string::iterator it = listText.begin(); it != listText.end(); it++)
   {
      if (*it == ',')
      {
         // end of the number
         retVal.push_back(atoi(numString.c_str()));
         numString = "";
         continue;
      }

      numString += *it;
   }

   if (!numString.empty())
   {
      retVal.push_back(atoi(numString.c_str()));
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

   std::ifstream f;
   f.open(argv[1], std::ios::in);
   if (!f.is_open())
   {
      std::cerr << "Error opening file " << argv[1] << std::endl;
      return 1;
   }

   int sequenceLength = 256;
   std::cout << "Sequence Length = " << sequenceLength << std::endl;

   std::string inputString;
   std::getline(f, inputString);

   f.close();

   std::vector<int> revList = stringToNumList(inputString);
   std::cout << "Number List: " << numListToString(revList) << std::endl;

   revList.push_back(17);
   revList.push_back(31);
   revList.push_back(73);
   revList.push_back(47);
   revList.push_back(23);

   std::cout << "Number List (inc. magic): " << numListToString(revList) << std::endl;

   std::vector<int> mainSeq;
   for(int i = 0; i < sequenceLength; i++)
   {
      mainSeq.push_back(i);
   }

   std::cout << "Number List: " << numListToString(mainSeq) << std::endl;

   int curPos = 0;
   int skipSize = 0;
   for(int numRounds = 0; numRounds < 64; numRounds++)
   {
      for(int i = 0; i < revList.size(); i++)
      {
         reverseSubList(mainSeq, curPos, revList[i]);
         // std::cout << "Number List: " << numListToString(mainSeq) << std::endl;

         curPos = (curPos + skipSize + revList[i]) % sequenceLength;
         skipSize++;
      }
   }

   std::cout << "Sparse Hash Nums: " << numListToString(mainSeq) << std::endl;

   std::vector<int> denseHash;
   sparseToDenseHash(mainSeq, denseHash);

   std::cout << "Hash = " << numListToHexString(denseHash) << std::endl;

}
