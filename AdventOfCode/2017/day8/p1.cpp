#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>

class RegisterSet
{
public:
   int getRegisterValue(std::string regName);

   int addToRegister(std::string regName, int val);

   void dumpRegisters();

   int getLargestRegisterValue();

   int getLargestRegisterValueEver();

   std::map<std::string, int> theRegisters;

   int allTimeMax;

};

int RegisterSet::getRegisterValue(std::string regName)
{
   if (theRegisters.find(regName) == theRegisters.end())
   {
      // New register found
      theRegisters[regName] = 0;
      return 0;
   }
   else
   {
      return theRegisters.at(regName);
   }
}

int RegisterSet::addToRegister(std::string regName, int val)
{
   if (theRegisters.find(regName) == theRegisters.end())
   {
      theRegisters[regName] = val;
   }
   else
   {
      int newVal = theRegisters.at(regName) + val;
      theRegisters[regName] = newVal;
   }

   int currentLargestVal = getLargestRegisterValue();
   if (currentLargestVal > allTimeMax)
   {
      allTimeMax = currentLargestVal;
   }
}

void RegisterSet::dumpRegisters()
{
   for(std::map<std::string, int>::iterator it = theRegisters.begin();
       it != theRegisters.end();
       it++)
   {
      std::cout << "RD: " << it->first << " = " << it->second << std::endl;
   }
}

int RegisterSet::getLargestRegisterValue()
{
   bool anyRead = false;
   int max;
   std::string maxName;

   for(std::map<std::string, int>::iterator it = theRegisters.begin();
       it != theRegisters.end();
       it++)
   {
      if (!anyRead)
      {
         max = it->second;
         maxName = it->first;
         anyRead = true;
      }
      else
      {
         if (it->second > max)
         {
            max = it->second;
            maxName = it->first;
         }
      }
   }

   // std::cout << "Max in " << maxName << " = " << max << std::endl;
   return max;
}

int RegisterSet::getLargestRegisterValueEver()
{
   return allTimeMax;
}

std::vector<std::string> split(std::string text, char delimeter)
{
   std::vector<std::string> retVal;
   std::string curToken;
   for(std::string::const_iterator it = text.begin();
       it != text.end();
       it++)
   {
      if (*it == delimeter)
      {
         // Found a delimeter
         if (!curToken.empty())
         {
            retVal.push_back(curToken);
            curToken = "";
         }

         curToken = "";
         continue;
      }

      curToken += *it;
   }

   if (!curToken.empty())
   {
      retVal.push_back(curToken);
      curToken = "";
   }

   return retVal;
}


void processInstruction(std::string ins, RegisterSet & rs)
{
   std::vector<std::string> parts = split(ins, ' ');

   if (parts.size() != 7)
   {
      std::cerr << "Error tokenizing the following ins (" << parts.size() << " tokens): "
                << ins << std::endl;
      return;
   }

   std::string condReg = parts[4];
   int condRegVal = rs.getRegisterValue(condReg);
   int compareVal = atoi(parts[6].c_str());
   std::string conditional = parts[5];
   int incrementAmount = atoi(parts[2].c_str());
   std::string regToModify = parts[0];
   std::string modifyOperation = parts[1];

   /*
   std::cout << "Debug Process: " << ins << std::endl;
   std::cout << "Debug: Mod=" << regToModify << ", Op=" << modifyOperation << ", amt="
             << incrementAmount << ", condReg=" << condReg << ", cond=" << conditional
             << ", Compare=" << compareVal << std::endl;
   */

   // Check the conditional
   if ( (conditional == ">") && (condRegVal <= compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }
   if ( (conditional == "<") && (condRegVal >= compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }
   if ( (conditional == ">=") && (condRegVal < compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }
   if ( (conditional == "<=") && (condRegVal > compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }
   if ( (conditional == "==") && (condRegVal != compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }
   if ( (conditional == "!=") && (condRegVal == compareVal))
   {
      std::cout << "Condition failed on: " << ins << std::endl;
      return;
   }

   // If we got to this point, the conditional was evaluated to true!


   if (modifyOperation == "dec")
   {
      incrementAmount *= -1;
   }

   rs.addToRegister(regToModify, incrementAmount);

   std::cout << std::endl << "EXECUTED: " << ins << std::endl;
   //rs.dumpRegisters();
}



int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cerr << "Usage " << argv[0] << " filename" << std::endl;
      return 1;
   }

   std::ifstream instructionFile;
   instructionFile.open(argv[1], std::ios::in);
   if (!instructionFile.is_open())
   {
      std::cerr << "Error opening file " << argv[1] << std::endl;
      return 1;
   }

   RegisterSet rs;
   rs.allTimeMax = 0;

   while (!instructionFile.eof())
   {
      std::string lineOfText;
      std::getline(instructionFile, lineOfText);

      if (lineOfText.empty())
      {
         std::cerr << "Read an empty line" << std::endl;
         continue;
      }

      processInstruction(lineOfText, rs);
   }

   instructionFile.close();

   std::cout << "All code processed" << std::endl;
   rs.dumpRegisters();

   int maxVal = rs.getLargestRegisterValue();
   std::cout << std::endl;
   std::cout << "Max = " << maxVal << std::endl;
   std::cout << "Max all time = " << rs.getLargestRegisterValueEver() << std::endl;

   return 0;
}
