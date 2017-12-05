#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>

/*
It sounds like the goal is to find the only two numbers in each row where one evenly divides the other - that is, where the result of the division operation is a whole number. They would like you to find those numbers on each line, divide them, and add up each line's result.

For example, given the following spreadsheet:

5 9 2 8
9 4 7 3
3 8 6 5
In the first row, the only two numbers that evenly divide are 8 and 2; the result of this division is 4.
In the second row, the two numbers are 9 and 3; the result is 3.
In the third row, the result is 2.
In this example, the sum of the results would be 4 + 3 + 2 = 9.

What is the sum of each row's result in your puzzle input?
*/

void minMax(std::vector<int>& data, int& min, int& max)
{
   min = data.front();
   max = data.front();

   for(std::vector<int>::const_iterator it = data.begin();
       it != data.end();
       it++)
   {
      if (*it < min)
         min = *it;

      if (*it > max)
         max = *it;
   }
}

int dividingPair(std::vector<int>& data)
{
   int numItems = data.size();

   for(int i = 0; i < numItems; i++)
   {
      for(int j = 0; j < numItems; j++)
      {
         // Can't do i == j, it will always == 1
         if (i == j)
            continue;

         int a = data[i];
         int b = data[j];

         if (a > b)
         {
           if (a % b == 0)
           {
              printf("Dividiing pair on this row is %d and %d = %d\n",
                     a, b, a/b);
              return a/b;
           }
         }
         else
         {
            if (b % a == 0)
            {
               printf("Dividiing pair on this row is %d and %d = %d\n",
                      b, a, b/a);
               return b/a;
            }
         }

      }
   }

   printf("Bad program input, didn't find a diving pair!\n");
   return -1;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);

  if (fd <= 0)
  {
    printf("Error opening file!\n");
    return 1;
  }

  int fileSize = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  printf("File size = %d\n", fileSize);
  
  const int BUF_SIZE = 2000;
  char giantBuf[BUF_SIZE];
  memset(giantBuf, 0, BUF_SIZE);

  int bytesRead = 0;
  int row = 0;
  int currentWordPos = 0;

  std::vector< std::vector<int> > spreadsheet;
  std::vector<int> currentRow;

  while(bytesRead < fileSize)
  {
    // Read a single character
    int status = read(fd, giantBuf + currentWordPos, 1);
    if (status != 1)
    {
      // We are done processing the file early, this is an error
      printf("Early failed read at byte %d\n", bytesRead);
      close(fd);
      return 1;
    }

    bytesRead++;

    // What did we read?

    // A newline is end of a line
    if (giantBuf[currentWordPos] == '\n')
    {
      // Were we already processing a word
      if (currentWordPos > 0)
      {
        int currentNumber = atoi(giantBuf);
        currentRow.push_back(currentNumber);
        printf("End of row, but we found a number = %d\n", currentNumber);
      }
      else
      {
        printf("We ended a row, but we were not in the process of parsing a number\n");
      }
     
      // Add the row to the spreadsheet
      spreadsheet.push_back(currentRow);
	
      // Clear the buffer, and the row now
      memset(giantBuf, 0, BUF_SIZE);
      currentWordPos = 0;
      currentRow.clear();

      continue;
    }

    // A number goes on the end of the current number
    if ( (giantBuf[currentWordPos] >= '0') && (giantBuf[currentWordPos] <= '9') )
    {
       currentWordPos++;
       continue;
    }

    // We assume anything else is whitespace

    if (currentWordPos == 0)
    {
      // Haven't read start of a number yet, must be between numbers
      continue;
    }

    // Must be the end of a number, add it to the list!
    int currentNumber = atoi(giantBuf);
    currentRow.push_back(currentNumber);

    // Clear the buffer
    memset(giantBuf, 0, BUF_SIZE);
    currentWordPos = 0;

    printf("We read a number on this row: %d\n", currentNumber);

  }

  close(fd);

  printf("Done reading the file\n");

  int checksum = 0;
  for(int i = 0; i < spreadsheet.size(); i++)
  {
     std::vector<int> thisRow = spreadsheet[i];

     checksum += dividingPair(thisRow);

     printf("Checksum = %d\n", checksum);

  }

  return 0;

}
