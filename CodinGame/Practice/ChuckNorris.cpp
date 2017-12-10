#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
using namespace std;

// Codin Game
// Chuck Norris

/*
The Goal

Binary with 0 and 1 is good, but binary with only 0, or almost, is even better! Originally, this is
a concept designed by Chuck Norris to send so called unary messages.

Write a program that takes an incoming message as input and displays as output the message encoded
using Chuck Norris’ method.

Rules

Here is the encoding principle:

The input message consists of ASCII characters (7-bit)
The encoded output message consists of blocks of 0
A block is separated from another block by a space
Two consecutive blocks are used to produce a series of same value bits (only 1 or 0 values):
- First block: it is always 0 or 00. If it is 0, then the series contains 1, if not, it contains 0
- Second block: the number of 0 in this block is the number of bits in the series

Example

Let’s take a simple example with a message which consists of only one character: Capital C. C in
binary is represented as 1000011, so with Chuck Norris’ technique this gives:

0 0 (the first series consists of only a single 1)
00 0000 (the second series consists of four 0)
0 00 (the third consists of two 1)

So C is coded as: 0 0 00 0000 0 00
*/

string AsciiToBinaryString(char c)
{
   uint8_t val = c;
   string retVal;
   
   int maskVal = 0x40;
   for(int i = 0; i < 7; i++)
   {
      if (val & maskVal)
      {
         retVal += '1';
      }
      else
      {
          retVal += '0';
      }

      maskVal >>= 1;
   }
   
   cout << "Converting " << c << " (" << (int) val << ") to " << retVal << endl;
   return retVal;
}

string BinaryToNorris(string binary)
{
  char lastCharFound = ' ';
  string retVal;
  bool isFirst = true;

  for(string::iterator it = binary.begin(); it != binary.end(); it++)
  {
    if (*it == lastCharFound)
    {
      retVal += '0';
    }
    else
    {
      if (isFirst)
      {
        isFirst = false;
      }
      else
      {
        retVal += ' ';
      }

      if (*it == '0')
      {
        retVal += "00 0";
      }
      else
      {
        retVal += "0 0";
      }

      lastCharFound = *it;
    }
  }

  return retVal;
}



/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    string MESSAGE;
    getline(cin, MESSAGE);

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    
    std::string binaryText;
    for(string::iterator singleChar = MESSAGE.begin(); singleChar != MESSAGE.end(); singleChar++)
    {
        binaryText += AsciiToBinaryString(*singleChar);
    }

    cout << "Binary: " << binaryText << endl;

    cout << "Norris: " << BinaryToNorris(binaryText) << endl;
}

