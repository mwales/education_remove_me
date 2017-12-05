#include<iostream>
#include<fstream>
#include<vector>

/*
For added security, yet another system policy has been put in place. Now, a valid passphrase must
contain no two words that are anagrams of each other - that is, a passphrase is invalid if any
word's letters can be rearranged to form any other word in the passphrase.

For example:

abcde fghij is a valid passphrase.
abcde xyz ecdab is not valid - the letters from the third word can be rearranged to form the first word.
a ab abc abd abf abj is a valid passphrase, because all letters need to be used when forming another word.
iiii oiii ooii oooi oooo is valid.
oiii ioii iioi iiio is not valid - any of these words can be rearranged to form any other word.
Under this new system policy, how many passphrases are valid?
*/

bool isAnagram(std::string a, std::string b)
{
   // We will just iterate through string a, while subtracting each letter from b
   // If search in b fails, return false
   // If b is "" at the end, return true
   // else, b had extra characters, return false
   for(std::string::const_iterator it = a.begin(); it != a.end(); it++)
   {
      std::string::size_type pos = b.find(*it);
      if (pos == std::string::npos)
      {
         return false;
      }

      b.erase(b.begin() + pos, b.begin() + pos + 1);
   }

   if (b == "")
   {
      return true;
   }
   else
   {
      return false;
   }
}

std::vector<std::string> splitOnWs(std::string s)
{
   std::vector<std::string> returnVal;

   std::string currentWord;
   for(std::string::const_iterator it = s.begin(); it != s.end(); it++)
   {
      if (*it != ' ')
      {
         currentWord += *it;
         continue;
      }

      if (currentWord.length() > 0)
      {
         returnVal.push_back(currentWord);
         currentWord = "";
      }
   }

   if (currentWord.length() > 0)
   {
      returnVal.push_back(currentWord);
      currentWord = "";
   }

   return returnVal;

}

bool isPassphraseValid(std::string pp)
{
   std::vector<std::string> parts = splitOnWs(pp);

   for(int i = 0; i < parts.size(); i++)
   {
      for(int j = 0; j < parts.size(); j++)
      {
         if (i == j)
         {
            // Don't match words against themselves
            continue;
         }

         if (isAnagram(parts[i], parts[j]))
         {
            return false;
         }
      }
   }

   // If we haven't returned yet, then the passphrase is valid
   return true;
}

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cout << "Usage: " << argv[0] << " passphraseFile" << std::endl;
      return 1;
   }

   std::ifstream fileData;
   fileData.open(argv[1]);

   if (!fileData.is_open())
   {
      std::cerr << "Failed to open the file " << argv[1] << std::endl;
      return 1;
   }

   int validPassPhrases = 0;

   while(!fileData.eof())
   {
      std::string fileContents;
      std::getline(fileData, fileContents);

      if (fileContents.length() == 0)
      {
         break;
      }



      bool isValid = isPassphraseValid(fileContents);

      std::cout << (isValid ? "TRUE " : "FALSE") << "> " << fileContents << std::endl;

      if (isPassphraseValid(fileContents))
         validPassPhrases++;


   }

   std::cout << "Number Valid Passphrases = " << validPassPhrases << std::endl;

   fileData.close();

   return 0;
}
