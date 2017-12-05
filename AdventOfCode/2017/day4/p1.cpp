#include<iostream>
#include<fstream>
#include<vector>

/*
To ensure security, a valid passphrase must contain no duplicate words.

For example:

aa bb cc dd ee is valid.
aa bb cc dd aa is not valid - the word aa appears more than once.
aa bb cc dd aaa is valid - aa and aaa count as different words.
The system's full passphrase list is available as your puzzle input. How many passphrases are valid?
*/

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

         if (parts[i] == parts[j])
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
