#include <iostream>


std::string solveCase()
{
   std::string inputWord;
   std::cin >> inputWord;


   std::string solution;
   for(auto&& singleLetter : inputWord)
   {
      if (solution.empty())
      {
         solution += singleLetter;
         continue;
      }

      if (singleLetter < solution[0])
      {
         solution += singleLetter;
      }
      else
      {
         solution = singleLetter + solution;
      }
   }


   return solution;

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
