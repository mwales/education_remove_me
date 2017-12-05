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
 * The spreadsheet consists of rows of apparently-random numbers. To make sure the recovery process is on the right track, they need you to calculate the spreadsheet's checksum. For each row, determine the difference between the largest value and the smallest value; the checksum is the sum of all of these differences.

For example, given the following spreadsheet:

5 1 9 5
7 5 3
2 4 6 8
The first row's largest and smallest values are 9 and 1, and their difference is 8.
The second row's largest and smallest values are 7 and 3, and their difference is 4.
The third row's difference is 6.
In this example, the spreadsheet's checksum would be 8 + 4 + 6 = 18.
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

     int min = 0;
     int max = 0;
     minMax(thisRow, min, max);

     printf("For row %d, min = %d, max = %d\n", i, min, max);
     checksum += max - min;

     printf("Checksum = %d\n", checksum);

  }

  return 0;

}
