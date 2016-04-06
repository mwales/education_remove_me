#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

void SolveProblem(int caseNum, int credit, std::vector<int> & itemList)
{
   int numItems = itemList.size();

   for(int i = 0; i < numItems; i++)
   {
      for(int j = i + 1; j < numItems; j++)
      {
         if (itemList[i] + itemList[j] == credit)
         {
            // Solution found!
            std::cout << "Case #" << caseNum << ": " << i+1 << " " << j+1 << std::endl;
            return;
         }
      }
   }

   std::cout << "Case #" << caseNum << ": No solution" << std::endl;

}

void ReadCase()
{
   int credit;
   std::cin >> credit;

   int numItems;
   std::cin >> numItems;

   std::cerr << "Num items = " << numItems << " with " << credit << " dollars to spend" << std::endl;

   std::vector<int> itemCosts;
   for(int i = 0; i < numItems; i++)
   {
      int temp;
      std::cin >> temp;
      itemCosts.push_back(temp);
   }

   static int caseNum = 1;
   SolveProblem(caseNum++, credit, itemCosts);

}

int main(int argc, char** argv)
{
   int numProblems = 0;
   std::cin >> numProblems;

   for(int i = 0; i < numProblems; i++)
   {
      ReadCase();
   }

   std::cerr << "Done!" << std::endl;
   return 0;


}
