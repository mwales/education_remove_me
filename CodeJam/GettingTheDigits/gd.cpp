#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <vector>

std::string removeAllScrambled(char clue, std::string const & removeWord, std::string inputWord, int* numFound)
{
   if (inputWord.find(clue) == std::string::npos)
   {
      // We didn't find any occurunces of the clue
      std::cerr << "Didn't find any more of " << clue << " in inputWord " << inputWord << std::endl;
      return inputWord;
   }

   // We found the clue, remove one removeWord and then call self recursively
   for(char curChar : removeWord)
   {
      inputWord.replace(inputWord.find(curChar), 1, "");
   }

   *numFound += 1;
   std::cerr << "Found " << removeWord << ", left with " << inputWord << ", num found = " << *numFound << std::endl;


   return removeAllScrambled(clue, removeWord, inputWord, numFound);
}

std::string numberWord(int n)
{
   switch(n)
   {
      case 0:
         return "ZERO";
      case 1:
         return "ONE";
      case 2:
         return "TWO";
      case 3:
         return "THREE";
      case 4:
         return "FOUR";
      case 5:
         return "FIVE";
      case 6:
         return "SIX";
      case 7:
         return "SEVEN";
      case 8:
         return "EIGHT";
      case 9:
         return "NINE";
      default:
         std::cerr << "Whoa, WTF" << std::endl;
         return "";
   }
}


std::string solveCase()
{
   std::string userInput;
   std::cin >> userInput;

   // keys are ordered, which is important
   std::map<int, int> solution;

   // You can search for all the number words one-by-one in the following order
   // 0(z), 2(w), 4(u), 6(x), 8(g), 3(h), 1(o), 7(s), 5(v), 9(i)
   std::vector<std::pair<int, char> > clues;
   clues.push_back(std::pair<int, char>(0, 'Z'));
   clues.push_back(std::pair<int, char>(2, 'W'));
   clues.push_back(std::pair<int, char>(4, 'U'));
   clues.push_back(std::pair<int, char>(6, 'X'));
   clues.push_back(std::pair<int, char>(8, 'G'));
   clues.push_back(std::pair<int, char>(3, 'H'));
   clues.push_back(std::pair<int, char>(1, 'O'));
   clues.push_back(std::pair<int, char>(7, 'S'));
   clues.push_back(std::pair<int, char>(5, 'V'));
   clues.push_back(std::pair<int, char>(9, 'I'));

   for(std::pair<int,char> curClue: clues)
   {
      int numFound = 0;
      userInput = removeAllScrambled(curClue.second, numberWord(curClue.first), userInput, &numFound);

      std::cerr << "Searching for " << curClue.first << ", and found " << numFound << std::endl;
      solution[curClue.first] = numFound;
   }

   std::ostringstream oss;
   for(auto&& curDigit : solution)
   {
      for(int i = 0; i < curDigit.second; i++)
         oss << curDigit.first;
   }

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
