#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

// 2017 Google Code Jam Round 1A - Ratatouille

// I didn't get any solution that would work for this problem.  I didn't like this Code Jam
// problem particularly.  It seemed a bit more confusing and complicated compared to many
// of the more simple and elegant problems.  

// The crux of my failure for this solution is that for a large package of an ingredient, it
// can have several possible serving sizes, not just 1.



// Problem
// 
// You've discovered it: the ultimate recipe for ratatouille, the famous French dish! You know which ingredients to use, and how many grams of each one to use, in order to make one serving of ratatouille. But you believe that anyone can cook, and so you want to share the recipe with the world... and make some money in the process!
// 
// You have ordered some ingredient packages that are easy to ship. Each package contains some amount of one ingredient; different packages may have different amounts even if they contain the same ingredient. For convenience, you ordered the same number of packages of each ingredient.
// 
// You would like to use these packages to form as many ratatouille kits as possible to send to customers. A kit consists of exactly one package of each ingredient, and a label with the integer number of servings of ratatouille that the kit makes. Since you do not want to shortchange customers or waste food, each package must contain between 90 and 110 percent (inclusive) of the amount of that ingredient that is actually needed to make the number of servings of ratatouille on the kit's label.
// 
// For example, suppose that one serving of ratatouille takes 500 g of tomato and 300 g of onion. Suppose that you have a 900 g package of tomato and a 660 g package of onion. You could form these into a kit that makes two servings of ratatouille. To make two servings, 1000 g of tomato and 600 g of onion are required. Since the 900 g of tomato you have is within [90, 110]% of the 1000 g of tomato required, and the 660 g of onion you have is within [90, 110]% of the 600 g of onion required, this is acceptable. However, you could not say that the kit makes one or three servings of ratatouille, nor could you say that it makes 1.999 servings (the number of servings must be an integer).
// 
// Note that there are some sets of packages that could never form a kit. Continuing with our recipe above, if you have a 1500 g package of tomato and an 809 g package of onion, for example, there is no amount of servings that you can make. Three servings would take 1500 g of tomato and 900 g of onion, and the amount of onion is not within the [90, 110]% range. No other integer amount of servings works, either.
// 
// You want to share your recipe with as many customers as possible, so you want to produce the maximum number of valid kits. (Of course, each package can be used in at most one kit.) What is the largest number of kits that you can form? Note that you are not required to maximize the total number of servings of ratatouille formed.
// 
// Input
// 
// The first line of the input gives the number of test cases, T. T test cases follow. Each case consists of the following:
// 
// One line with two integers N: the number of ingredients, and P, the number of packages of each ingredient.
// One line with N integers Ri. The i-th of these represents the number of grams of the i-th ingredient needed to make one serving of ratatouille.
// N more lines of P integers each. The j-th value on the i-th of these lines, Qij, represents the quantity, in grams, in the j-th package of the i-th ingredient.
// Output
// 
// For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is the maximum number of kits you can produce, as described above.

void shipNumServings(std::vector<std::vector<int>* >& ingreds, int numServings)
{
   for(auto curIngred = ingreds.begin(); curIngred != ingreds.end(); curIngred++)
   {
      if ((*curIngred)->front() == numServings)
      {
         (*curIngred)->erase((*curIngred)->begin());
      }
   }
}

bool allIngredientsCanServe(std::vector<std::vector<int>* > const & ingreds, int numServings)
{
   for(auto curIngred = ingreds.begin(); curIngred != ingreds.end(); curIngred++)
   {
      if ((*curIngred)->front() != numServings)
      {
         return false;
      }
   }

   return true;
}

int getSmallestServing(std::vector<std::vector< int >* > const & ingreds)
{
   int curSmall = ingreds.front()->front();
   for(auto curIngred = ingreds.begin(); curIngred != ingreds.end(); curIngred++)
   {
      if ((*curIngred)->front() < curSmall)
      {
         curSmall = (*curIngred)->front();
      }

   }

   return curSmall;
}

bool allIngredientsLeft(std::vector<std::vector< int >* > ingreds)
{
   for(auto curIngred = ingreds.begin(); curIngred != ingreds.end(); curIngred++)
   {
      if ((*curIngred)->empty())
         return false;
   }

   return true;
}

std::string solveCase()
{
   int numIng;
   std::cin >> numIng;

   int numPkgs;
   std::cin >> numPkgs;

   std::vector<int> singleServingAmt;
   for(int i = 0; i < numIng; i++)
   {
      int amt;
      std::cin >> amt;
      singleServingAmt.push_back(amt);
   }

   std::vector<std::vector<int>* > servingsPerIng;

   for(int i = 0; i < numIng; i++)
   {
      std::vector<int>* servingsForIng = new std::vector<int>();

      int amtForServing = singleServingAmt[i];
      std::cerr << "Number of grams for a serving of this ing = " << amtForServing << std::endl;

      for(int pkgNum = 0; pkgNum < numPkgs; pkgNum++)
      {
         int pkgSize;
         std::cin >> pkgSize;

         int numServings = (pkgSize + amtForServing/2) / amtForServing;
         int exactSizeForServings = amtForServing * numServings;

         std::cerr << "Trying to make " << numServings << " from pkgSize " << pkgSize << std::endl;

         double ingredRatio = (double) pkgSize / (double) exactSizeForServings;
         if ( (ingredRatio < 0.9) || (ingredRatio > 1.10))
         {
            std::cerr << "Ing " << i << ", Pkg " << pkgNum << " size " << pkgSize  << " is too wasteful to use! ratio=" << ingredRatio << std::endl;
            continue;
         }

         servingsForIng->push_back(numServings);



         std::cerr << "Ing " << i << ", Pkg " << pkgNum << " size " << pkgSize << " provides " << numServings << " servings" << std::endl;


      }

      std::sort(servingsForIng->begin(), servingsForIng->end());
      servingsPerIng.push_back(servingsForIng);
   }

   int customers = 0;
   while(allIngredientsLeft(servingsPerIng))
   {
      int smallest = getSmallestServing(servingsPerIng);

      if (allIngredientsCanServe(servingsPerIng, smallest))
      {
         std::cerr << "A customer will get " << smallest << " servings!" << std::endl;
         customers++;

         shipNumServings(servingsPerIng, smallest);
      }
      else
      {
         shipNumServings(servingsPerIng, smallest);
      }


   }

   std::ostringstream oss;
   oss << customers;






   return oss.str();

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
