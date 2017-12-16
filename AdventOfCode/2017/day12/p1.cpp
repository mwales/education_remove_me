#include<iostream>
#include<vector>
#include<fstream>
#include<set>
#include<map>
#include<sstream>
#include<stdlib.h>

/*
--- Day 12: Digital Plumber ---

Walking along the memory banks of the stream, you find a small village that is experiencing a little
confusion: some programs can't communicate with each other.

Programs in this village communicate using a fixed system of pipes. Messages are passed between
programs using these pipes, but most programs aren't connected to each other directly. Instead,
programs pass messages between each other until the message reaches the intended recipient.

For some reason, though, some of these messages aren't ever reaching their intended recipient, and
the programs suspect that some pipes are missing. They would like you to investigate.

You walk through the village and record the ID of each program and the IDs with which it can
communicate directly (your puzzle input). Each program has one or more programs with which it can
communicate, and these pipes are bidirectional; if 8 says it can communicate with 11, then 11 will
say it can communicate with 8.

You need to figure out how many programs are in the group that contains program ID 0.

For example, suppose you go door-to-door like a travelling salesman and record the following list:

0 <-> 2
1 <-> 1
2 <-> 0, 3, 4
3 <-> 2, 4
4 <-> 2, 3, 6
5 <-> 6
6 <-> 4, 5

In this example, the following programs are in the group that contains program ID 0:

Program 0 by definition.
Program 2, directly connected to program 0.
Program 3 via program 2.
Program 4 via program 2.
Program 5 via programs 6, then 4, then 2.
Program 6 via programs 4, then 2.
Therefore, a total of 6 programs are in this group; all but program 1, which has a pipe that
connects it to itself.

How many programs are in the group that contains program ID 0?

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---

There are more programs than just the ones in the group containing program ID 0. The rest of them
have no way of reaching that group, and still might have no way of reaching each other.

A group is a collection of programs that can all communicate via pipes either directly or
indirectly. The programs you identified just a moment ago are all part of the same group. Now, they
would like you to determine the total number of groups.

In the example above, there were 2 groups: one consisting of programs 0,2,3,4,5,6, and the other
consisting solely of program 1.

How many groups are there in total?
*/

std::set<int> joinSets(std::set<int> const & a, std::set<int> const & b)
{
   std::set<int> retVal;

   for(auto it = a.begin(); it != a.end(); it++)
   {
      retVal.insert(*it);
   }

   for(auto it = b.begin(); it != b.end(); it++)
   {
      retVal.insert(*it);
   }

   return retVal;
}

bool findAndMergeSets(std::vector<std::set<int> >& setList)
{
   for(int i = 0; i < setList.size(); i++)
   {
      for(int j = 0; j < setList.size(); j++)
      {
         if (i == j)
            continue;

         // i and j are two sets to compare

         // Lets look for all members of i in j
         for(auto it = setList[i].begin(); it != setList[i].end(); it++)
         {
            if (setList[j].find(*it) != setList[j].end())
            {
               // Found a member in both, need to merge these sets
               std::set<int> mergedSet = joinSets(setList[i], setList[j]);

               // Remove the higher set first, then lower set
               if (i > j)
               {
                  setList.erase(setList.begin() + i);
                  setList.erase(setList.begin() + j);
               }
               else
               {
                  setList.erase(setList.begin() + j);
                  setList.erase(setList.begin() + i);
               }

               // Add the merged set to the end of the vector
               setList.push_back(mergedSet);
               return true;
            }
         }
      }
   }

   // Found now sets to merge
   return false;
}

std::string dumpSet(std::set<int> vals)
{
   std::ostringstream oss;
   for(auto it = vals.begin(); it != vals.end(); it++)
   {
      oss << *it << ",";
   }

   return oss.str();
}

void dumpSets(std::vector<std::set<int> > const & setList)
{
   std::cout << "<setList>" << std::endl;
   for(auto it = setList.begin(); it != setList.end(); it++)
   {
      std::cout << "  " << dumpSet(*it) << "   LEN=" << it->size() << std::endl;
   }
   std::cout << "</setList>" << std::endl;
}

std::vector<std::string> splitString(std::string text, char delimeter)
{
   std::vector<std::string> retVal;

   std::string token;
   for(auto it = text.begin(); it != text.end(); it++)
   {
      if (*it == delimeter)
      {
         // End of token
         retVal.push_back(token);
         token = "";
         continue;
      }

      token += *it;
   }

   if (!token.empty())
   {
      retVal.push_back(token);
   }

   return retVal;
}

std::string replaceText(std::string source, std::string search, std::string replaceData)
{
   auto pos = source.find(search);
   if (pos == std::string::npos)
   {
      // We didn't find the string we were searching for
      return source;
   }

   std::string replaceSingle = source.replace(pos, search.size(), replaceData);

   // Recursively call replaceText again incase there is another string later
   return replaceText(replaceSingle, search, replaceData);
}

std::set<int> parseLine(std::string text)
{
   std::string arrowsRemoved = replaceText(text, "<->", ",");


   std::vector<std::string> tokens = splitString(arrowsRemoved, ',');

   std::set<int> retVal;
   for(auto it = tokens.begin(); it != tokens.end(); it++)
   {
      int val = atoi( it->c_str());

      //std::cout << "Token " << *it << " = " << val << std::endl;
      retVal.insert(val);
   }

   return retVal;
}

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
      return 1;
   }

   std::ifstream f;
   f.open(argv[1], std::ios::in);

   if (!f.is_open())
   {
      std::cerr << "Error opening file " << argv[1] << std::endl;
      return 1;
   }

   std::vector<std::set<int> > setList;
   while(!f.eof())
   {
      std::string text;
      std::getline(f, text);

      if (text.empty())
         continue;


      std::set<int> curGroup = parseLine(text);

      std::cout << dumpSet(curGroup) << std::endl;

      std::cout << std::endl;

      setList.push_back(curGroup);
   }

   f.close();

   while(findAndMergeSets(setList) == true)
   {
      // dumpSets(setList);
   }



   std::cout << "Done" << std::endl;

   dumpSets(setList);

   std::cout << "Number of groups = " << setList.size() << std::endl;

   return 0;
}

