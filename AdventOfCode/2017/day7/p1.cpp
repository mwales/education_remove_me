#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>
#include<stdlib.h>

/*
This one was the hardest challenge yet.  This includes the code for part 2 of the challenge. Part
2 is not solved purely in code.  I still have to grep the logs for the offending nodes, and then
figure out the size they should have been
*/

/**
Part 1

One program at the bottom supports the entire tower. It's holding a large disc, and on the disc are
balanced several more sub-towers. At the bottom of these sub-towers, standing on the bottom disc,
are other programs, each holding their own disc, and so on. At the very tops of these
sub-sub-sub-...-towers, many programs stand simply keeping the disc below them balanced but with no
disc of their own.

You offer to help, but first you need to understand the structure of these towers. You ask each
program to yell out their name, their weight, and (if they're holding a disc) the names of the
programs immediately above them balancing on that disc. You write this information down (your puzzle
input). Unfortunately, in their panic, they don't do this in an orderly fashion; by the time you're
done, you're not sure which program gave which information.

For example, if your list is the following:

pbga (66)
xhth (57)
ebii (61)
havc (66)
ktlj (57)
fwft (72) -> ktlj, cntj, xhth
qoyq (66)
padx (45) -> pbga, havc, qoyq
tknk (41) -> ugml, padx, fwft
jptl (61)
ugml (68) -> gyxo, ebii, jptl
gyxo (61)
cntj (57)
...then you would be able to recreate the structure of the towers that looks like this:

                gyxo
              /
         ugml - ebii
       /      \
      |         jptl
      |
      |         pbga
     /        /
tknk --- padx - havc
     \        \
      |         qoyq
      |
      |         ktlj
       \      /
         fwft - cntj
              \
                xhth
In this example, tknk is at the bottom of the tower (the bottom program), and is holding up ugml,
padx, and fwft. Those programs are, in turn, holding up other programs; in this example, none of
those programs are holding up any other programs, and are all the tops of their own towers. (The
actual tower balancing in front of you is much larger.)

Before you're ready to help them, you need to make sure your information is correct. What is the
name of the bottom program?

Part 2

For any program holding a disc, each program standing on that disc forms a sub-tower. Each of those
sub-towers are supposed to be the same weight, or the disc itself isn't balanced. The weight of a
tower is the sum of the weights of the programs in that tower.

In the example above, this means that for ugml's disc to be balanced, gyxo, ebii, and jptl must all
have the same weight, and they do: 61.

However, for tknk to be balanced, each of the programs standing on its disc and all programs above
it must each match. This means that the following sums must all be the same:

ugml + (gyxo + ebii + jptl) = 68 + (61 + 61 + 61) = 251
padx + (pbga + havc + qoyq) = 45 + (66 + 66 + 66) = 243
fwft + (ktlj + cntj + xhth) = 72 + (57 + 57 + 57) = 243
As you can see, tknk's disc is unbalanced: ugml's stack is heavier than the other two. Even though
the nodes above ugml are balanced, ugml itself is too heavy: it needs to be 8 units lighter for its
stack to weigh 243 and keep the towers balanced. If this change were made, its weight would be 60.

Given that exactly one program is the wrong weight, what would its weight need to be to balance the
entire tower?
 */

class node
{
public:
   std::string theName;
   int theWeight;
   std::vector<std::string> theChildren;
   std::vector<node*> theChildrenPtrs;

   node(std::string);

   bool hasChild(std::string childName);

   int weightCheck();

   int numInBranch();
};

node::node(std::string line)
{

   std::cout << "D: Parsing a new node: " << line << std::endl;

   std::string::iterator singleChar;
   for(singleChar = line.begin(); singleChar != line.end(); singleChar++)
   {
      if (*singleChar == ' ')
      {
         // We are done processing this line!
         break;
      }

      theName += *singleChar;
   }

   // std::cout << "D: name = " << theName << std::endl;

   // Next characters is open parens, number, close parens, space
   std::string weightText;

   for(singleChar++ ; singleChar != line.end(); singleChar++)
   {
      if ( (*singleChar == '(') || (*singleChar == ')') )
      {
         continue;
      }

      if (*singleChar == ' ')
      {
         // We are done parsing the weight number
         // std::cout << "D: WStr = " << weightText << std::endl;
         break;
      }

      // std::cout << "D: Wc[" << *singleChar << "]" << std::endl;
      weightText += *singleChar;
   }

   theWeight = atoi(weightText.c_str());

   // std::cout << "D: weight = " << theWeight << std::endl;

   if (singleChar == line.end())
   {
      std::cout << "NODE: " << theName << ", W=" << theWeight << ", C= <none>" << std::endl;
      return;
   }

   // Skip the arrow in the middle
   for(int i = 0; i < 4; i++)
   {

      singleChar++;

      if (singleChar == line.end())
      {
         // ooops, this node has no children
         std::cout << "NODE: " << theName << ", W=" << theWeight << ", C= <none>" << std::endl;
         return;
      }

      // std::cout << "D: Skipping a char (" << i << "): " << *singleChar << std::endl;
   }

   // std::cout << "D: Now lets parse some kids" << std::endl;

   // Now parse out all the children
   std::string children;
   for(; singleChar != line.end(); singleChar++)
   {
      if (*singleChar == ' ')
      {
         // Ignore spaces
         continue;
      }

      if (*singleChar == ',')
      {
         // That was the end of one of the children
         theChildren.push_back(children);
         children = "";
         continue;
      }

      children += *singleChar;
   }

   if (!children.empty())
   {
      theChildren.push_back(children);
   }

   // std::cout << "D: Done parsing kids" << std::endl;

   // Summarize and debug what we found on this line
   std::cout << "NODE: " << theName << ", W=" << theWeight << ", C=";
   for(std::vector<std::string>::iterator it = theChildren.begin();
       it != theChildren.end();
       it++)
   {
      std::cout << *it << " ";
   }

   std::cout << std::endl;

}

bool node::hasChild(std::string childName)
{
   if (std::find(theChildren.begin(), theChildren.end(), childName) == theChildren.end())
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool areAllSameValue(std::vector<int> const & nums)
{
   if (nums.size() <= 1)
   {
      // std::cout << "Not enough kids to verify all same" << std::endl;
      return true;
   }

   bool firstRead = false;
   int value;
   for(std::vector<int>::const_iterator it = nums.begin(); it != nums.end(); it++)
   {
      // std::cout << "areAllSame ? " << *it << std::endl;

      if (!firstRead)
      {
         value = *it;
         firstRead = true;
      }
      else
      {
         if (*it != value)
         {
            return false;
         }
      }
   }

   return true;
}

int node::weightCheck()
{
   int myTotalWeight = theWeight;

   std::vector<int> kidWeights;
   for(std::vector<node*>::iterator kidIt = theChildrenPtrs.begin();
       kidIt != theChildrenPtrs.end();
       kidIt++)
   {
      int curKidWeight = (*kidIt)->weightCheck();
      myTotalWeight += curKidWeight;
      kidWeights.push_back(curKidWeight);
   }

   if (!areAllSameValue(kidWeights))
   {
      std::cout << theName << " needs to send one of his kids to fat camp" << std::endl;

      for(int i = 0; i < kidWeights.size(); i++)
      {
         std::cout << "Kid " << theChildrenPtrs[i]->theName << " weighs " << kidWeights[i] << std::endl;
      }
   }

   std::cout << theName << " weighs " << myTotalWeight << std::endl;
   return myTotalWeight;
}

int node::numInBranch()
{
   int retVal = 1;

   for(std::vector<node*>::iterator it = theChildrenPtrs.begin(); it != theChildrenPtrs.end(); it++)
   {
      retVal += (*it)->numInBranch();
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

   std::ifstream inputData;
   inputData.open(argv[1], std::ios::in);

   if (!inputData.is_open())
   {
      std::cerr << "Error opening file " << argv[1] << std::endl;
      return 1;
   }

   std::map<std::string, std::string> treeData;
   std::vector<node*> nodeList;

   while (!inputData.eof())
   {
      std::string singleLine;
      std::getline(inputData, singleLine);

      if (singleLine.empty())
      {
         std::cout << "Skipping a blank line" << std::endl;
         continue;
      }

      node* n = new node(singleLine);
      nodeList.push_back(n);
   }

   inputData.close();

   // Organize the tree
   // For every node, search for all their children, and then assign ptrs of children to parents
   for(std::vector<node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++)
   {
      // Get current nodes children names
      std::vector<std::string> kids = (*it)->theChildren;
      for(std::vector<std::string>::iterator kidIt = kids.begin(); kidIt != kids.end(); kidIt++)
      {
         // Search for this child node
         for(std::vector<node*>::iterator sit = nodeList.begin(); sit != nodeList.end(); sit++)
         {
            if ((*sit)->theName == *kidIt)
            {
               (*it)->theChildrenPtrs.push_back(*sit);
            }
         }
      }
   }

   std::string curNodeName;
   curNodeName = nodeList[0]->theName;

   node* lastParentFound = NULL;

   bool solved = false;
   while(!solved)
   {
      bool parentFound = false;
      for(std::vector<node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++)
      {

         if ((*it)->hasChild(curNodeName))
         {
            // We Found a new parent
            std::cout << "New parent = " << (*it)->theName << std::endl;
            curNodeName = (*it)->theName;
            lastParentFound = (*it);
            parentFound = true;

            break;
         }

      }

      if (!parentFound)
      {
         std::cout << "Solved!" << std::endl;
         solved = true;
      }


   }

   std::cout << "Weight Check Time!" << std::endl;
   lastParentFound->weightCheck();

   std::cout << "Number of nodes in tree " << lastParentFound->numInBranch() << std::endl;

   return 0;

}
