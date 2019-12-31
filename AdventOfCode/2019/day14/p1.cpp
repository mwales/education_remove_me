#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

std::vector<std::string> split(std::string data, std::string delimeter)
{
   std::vector<std::string> retVal;
   int pos= 0;
   while(true)
   {
      int delimPos = data.find(delimeter, pos);

      if (delimPos == std::string::npos)
      {
         // Add the last token to the list and return
         // std::cerr << "Token = " << data.substr(pos) << std::endl << std::endl;
         retVal.push_back(data.substr(pos));
         return retVal;
      }
      else
      {
         // We found a token and delimter
         // std::cerr << "Token = " << data.substr(pos, delimPos - pos) << std::endl;
         retVal.push_back(data.substr(pos, delimPos - pos));
         pos = delimPos + delimeter.size();
      }
   }

}

std::string strip(std::string text)
{
   std::string nowsbegin;
   bool goodAsciiFound = false;
   for(auto it = text.begin(); it != text.end(); it++)
   {
      if (goodAsciiFound)
      {
         nowsbegin += *it;
         continue;
      }

      if ( (*it == ' ') || (*it == '\t') || (*it == '\n') )
      {
         continue;
      }

      goodAsciiFound = true;
      nowsbegin += *it;
   }

   std::string retVal;

   // Now strip the ending white space...
   int numEndingWhitespace = 0;
   for(auto it = nowsbegin.rbegin(); it != nowsbegin.rend(); it++)
   {
      if ( (*it == ' ') || (*it == '\t') || (*it == '\n') )
      {
         numEndingWhitespace++;
      }
      else
      {
         break;
      }
   }

   return nowsbegin.substr(0, nowsbegin.size() - numEndingWhitespace);
}

std::vector<int> strListToInt(std::vector<std::string> const & data)
{
   std::vector<int> retVal;
   for(auto di = data.begin(); di != data.end(); di++)
   {
      int val = atoi(di->c_str());
      // std::cerr << "Converting item " << *di << " to " << val << std::endl;
      retVal.push_back(val);
   }

   return retVal;
}

void printIntList(std::vector<int> const & data)
{
   if (data.size() == 0)
   {
      std::cout << std::endl;
      return;
   }

   auto di = data.begin();
   while(true)
   {
      std::cout << *di;

      di++;
      if (di == data.end())
      {
         std::cout << std::endl;
         return;
      }
      else
      {
         std::cout << ", ";
      }
   }
}

typedef std::pair<std::string, int> Ingredient;

template<class T>
std::vector<T> mergeList(std::vector<T> a, std::vector<T> b)
{
   for(auto it = b.begin(); it != b.end(); it++)
   {
      a.push_back(*it);
   }

   return a;
}

void printIList(std::vector<Ingredient> iList)
{
   for(auto it = iList.begin(); it != iList.end(); it++)
   {
      std::cout << "   " << it->second << " " << it->first << std::endl;
   }
   std::cout << std::endl;

}

class Reaction
{
public:
   Reaction(std::string input);

   std::string toString();

   bool hasOutput(std::string out);

   int numOutput();

   std::vector<std::string> requiresIngredients();

   int numOfIngredient(std::string i);

   std::vector<Ingredient> produce(int numOuptut, bool forceReaction);



private:

   std::map<std::string, int> theInputs;

   Ingredient theOutputs;
};

Reaction::Reaction(std::string input)
{
   std::vector<std::string> iando = split(input, "=>");

   if (iando.size() != 2)
   {
      std::cerr << "input: " << input << "split into " << iando.size()
                << " parts!" << std::endl;
      exit(1);
   }

   // How many inputs?
   std::vector<std::string> inputText = split(iando[0], ",");

   for(auto it = inputText.begin(); it != inputText.end(); it++)
   {
      // Each *it is something like qty name
      std::vector<std::string> ingreds = split(strip(*it), " ");

      if (ingreds.size() < 2)
      {
         std::cerr << "Issue parsing ingred: " << *it << std::endl;
         break;
      }

      int qty = atoi(ingreds[0].c_str());
      theInputs[ingreds[1]] = qty;
   }

   std::cout << "Found " << theInputs.size() << " inputs in " << input << std::endl;

   // Parse the ouput components of the reaction
   std::vector<std::string> outputText = split(strip(iando[1]), " ");
   if (outputText.size() != 2)
   {
      std::cerr << "Error parsing outputs: " << iando[1] << std::endl;
      exit(1);
   }

   theOutputs = std::make_pair(outputText[1], atoi(outputText[0].c_str()));
}

std::string Reaction::toString()
{
   std::string retVal;
   retVal += "IN: ";
   for(auto it = theInputs.begin(); it != theInputs.end(); it++)
   {
      retVal += it->first;
      retVal += "(";
      retVal += std::to_string(it->second);
      retVal += ") ";
   }

   retVal += "===> ";

   retVal += theOutputs.first;
   retVal += "(";
   retVal += std::to_string(theOutputs.second);
   retVal += ")";

   return retVal;
}

bool Reaction::hasOutput(std::string out)
{
   return theOutputs.first == out;
}

int Reaction::numOutput()
{
   return theOutputs.second;
}

std::vector<std::string> Reaction::requiresIngredients()
{
   std::vector<std::string> retVal;
   for(auto it = theInputs.begin(); it != theInputs.end(); it++)
   {
      retVal.push_back(it->first);
   }
   return retVal;
}

int Reaction::numOfIngredient(std::string i)
{
   if (theInputs.find(i) == theInputs.end())
   {
      std::cerr << "Asked for numIngredients for non ingred: " << i << std::endl;
      exit(1);
   }

   return theInputs[i];
}

std::vector<Ingredient> Reaction::produce(int numOutputReqd, bool forceReaction)
{
   std::vector<Ingredient> retVal;
   int numReactions = numOutputReqd / numOutput();

   if ( (numOutputReqd % numOutput()) != 0)
   {
      std::cout << "Following reaction can't produce numOuptut = " << numOutputReqd << std::endl;
      std::cout << toString() << std::endl;

      if (forceReaction)
      {
         std::cout << "Forcing the output even though there is waste!" << std::endl;
         numReactions++;
      }
      else
      {
         // Return output as ingredient
         retVal.push_back(std::make_pair(theOutputs.first, numOutputReqd));
         return retVal;
      }
   }

   for(auto it = theInputs.begin(); it != theInputs.end(); it++)
   {
      Ingredient curI = std::make_pair(it->first, it->second * numReactions);
      retVal.push_back(curI);
   }

   return retVal;
}

class ChainReaction
{
public:

   ChainReaction(std::vector<Reaction>);

   std::vector<Ingredient> oneReaction(int numOutput, std::string output, bool forceReaction = false);

   Ingredient allReactions(int numOutput, std::string output);

private:

   std::vector<Ingredient> reduceIngredientList(std::vector<Ingredient> il);

   std::vector<Reaction> theReactions;
};

ChainReaction::ChainReaction(std::vector<Reaction> rlist)
{
   theReactions = rlist;
}

std::vector<Ingredient> ChainReaction::oneReaction(int numOutput, std::string output, bool forceReaction)
{
   std::cout << "oneReaction(" << numOutput << "," << output << ")" << std::endl;
   for(auto it = theReactions.begin(); it != theReactions.end(); it++)
   {
      if (it->hasOutput(output))
      {
         // This is the reaction we want to use!
         std::cout << "Going to use R: " << it->toString() << std::endl;
         std::vector<Ingredient> retVal = it->produce(numOutput, forceReaction);

         std::cout << "To produce " << numOutput << " " << output << " we require:" << std::endl;
         printIList(retVal);

         return retVal;
      }
   }

   std::cout << "Error, no reaction found to produce " << numOutput << " " << output << std::endl;
   std::vector<Ingredient> retVal;
   retVal.push_back(std::make_pair(output, numOutput));
   return retVal;
}

Ingredient ChainReaction::allReactions(int numOutput, std::string output)
{
   std::vector<Ingredient> iList = oneReaction(numOutput, output);
   bool forceReactions = false;
   while(iList.size() > 1)
   {
      std::cout << "After one phase of reactions:" << std::endl;
      printIList(iList);

      std::vector<Ingredient> newIList;
      for(auto it = iList.begin(); it != iList.end(); it++)
      {
         std::vector<Ingredient> rList = oneReaction(it->second, it->first, forceReactions);
         newIList = mergeList(newIList, rList);

      }

      std::cout << " ******** Ran through entire loop of reactions ********* " << std::endl;

      std::cout << "Reducing: " << std::endl;
      printIList(newIList);

      std::vector<Ingredient> reducedList;
      reducedList = reduceIngredientList(newIList);

      std::cout << "Down to:" << std::endl;
      printIList(reducedList);

      if (reducedList == iList)
      {
         // Try forcing some reactions?
         std::cout << "Since ingredients are still the same after running through all the reactions, going to force them!" << std::endl;
         forceReactions = true;
      }
      else
      {
         std::cout << "List of ingredients is different than what we started this phase" << std::endl;
         iList = reducedList;
         forceReactions = false;
      }
   }

   return iList[0];
}

std::vector<Ingredient> ChainReaction::reduceIngredientList(std::vector<Ingredient> il)
{
   std::map<std::string, int> workingData;
   for(auto it = il.begin(); it != il.end(); it++)
   {
      if (workingData.find(it->first) == workingData.end() )
      {
         // New ingredient
         workingData[it->first] = it->second;
      }
      else
      {
         // Add to existing ingredient
         workingData[it->first] = workingData[it->first] + it->second;
      }
   }

   // Convert working map back into std::vector
   std::vector<Ingredient> retVal;
   for(auto it = workingData.begin(); it != workingData.end(); it++)
   {
      Ingredient curI = std::make_pair(it->first, it->second);
      retVal.push_back(curI);
   }

   return retVal;
}

int main(int, char**)
{
   std::string text;
   std::vector<Reaction> rList;
   while(true)
   {
      std::getline(std::cin, text);

      if (std::cin.eof())
      {
         break;
      }

      std::cout << "out: " << text << std::endl;

      Reaction r(text);
      rList.push_back(r);

      std::cout << "R: " << r.toString() << std::endl;
   }

   ChainReaction cr(rList);

   cr.allReactions(1, "FUEL");


   return 0;
}

