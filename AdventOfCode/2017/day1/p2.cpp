#include <stdio.h>
#include <string.h>

/*
Now, instead of considering the next digit, it wants you to consider the digit halfway around the circular list. That is, if your list contains 10 items, only include a digit in your sum if the digit 10/2 = 5 steps forward matches it. Fortunately, your list has an even number of elements.

For example:

1212 produces 6: the list contains 4 items, and all four digits match the digit 2 items ahead.
1221 produces 0, because every comparison is between a 1 and a 2.
123425 produces 4, because both 2s match each other, but no other digit has a match.
123123 produces 12.
12131415 produces 4.
*/

int charToNum(char x)
{
   return x - '0';
}

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      printf("Usage: %s number\n", argv[0]);
      return 1;
   }

   int numLen = strlen(argv[1]);
   int halfway = numLen / 2;

   int checksum = 0;
   for(int i = 0; i < numLen; i++)
   {
      int otherIndex = (i + halfway) % numLen;

      printf("Comparing positions %d and %d, which have %c and %c\n",
             i, otherIndex, argv[1][i], argv[1][otherIndex]);

      if (argv[1][i] == argv[1][otherIndex])
      {
         checksum += charToNum(argv[1][i]);
      }

   }

   printf("Checksum = %d\n", checksum);

   return 0;
}
