#include<iostream>
#include<vector>

class OrbitNode
{
public:

   OrbitNode(std::string name);

   void setParent(OrbitNode* parent);

   OrbitNode* getParent() { return theParentNode; }

   int getNumOrbitsAbove();

   void addChildOrbit(OrbitNode* childNode);

   int getNumOrbitsBelow();

   int getNumNodes();

   OrbitNode* findChild(std::string name);

   void printNodeInfo();

   std::string getName() { return theName; }

   int getLevel();

protected:

   std::string theName;

   OrbitNode* theParentNode;

   std::vector<OrbitNode*> theChildren;

};

OrbitNode::OrbitNode(std::string name)
{
   theName = name;
   theParentNode = nullptr;
}

void OrbitNode::setParent(OrbitNode* parent)
{
   theParentNode = parent;
}

int OrbitNode::getNumOrbitsAbove()
{
   if (theParentNode == nullptr)
   {
      // std::cerr << theName << " has not parents to orbit" << std::endl;
      return 0;
   }

   int retVal = theParentNode->getNumOrbitsAbove() + 1;
   // std::cerr << theName << " has " << retVal << " parent orbits" << std::endl;
   return retVal;
}

int OrbitNode::getNumNodes()
{
   int retVal = 1;
   for(auto childIt = theChildren.begin(); childIt != theChildren.end(); childIt++)
   {
      retVal += (*childIt)->getNumNodes();
   }
   return retVal;
}

void OrbitNode::addChildOrbit(OrbitNode* childNode)
{
   theChildren.push_back(childNode);
}

int OrbitNode::getNumOrbitsBelow()
{
   int retVal = getNumOrbitsAbove();
   for(auto childIt = theChildren.begin(); childIt != theChildren.end(); childIt++)
   {
      OrbitNode* curChild = *childIt;
      retVal += curChild->getNumOrbitsBelow();
   }

   //std::cerr << "Node " << theName << " has children with " << retVal << " orbits" << std::endl;
   return retVal;
}

OrbitNode* OrbitNode::findChild(std::string name)
{
   // Am I the node in question?
   if (theName == name)
   {
      // std::cerr << "Found child node " << name << std::endl;
      return this;
   }

   for(auto childIt = theChildren.begin(); childIt != theChildren.end(); childIt++)
   {
      OrbitNode* curChild = *childIt;

      OrbitNode* searchNode = curChild->findChild(name);
      if (searchNode != nullptr)
      {
         // std::cerr << name << " was a descendent of " << theName << std::endl;
         return searchNode;
      }
   }

   // std::cerr << theName << " has no child named " << name << std::endl;
   return nullptr;
}

void OrbitNode::printNodeInfo()
{
   std::string nodeInfo;
   nodeInfo += theName;
   nodeInfo += " (above=";
   nodeInfo += std::to_string(getNumOrbitsAbove());
   nodeInfo += ", below=";
   nodeInfo += std::to_string(getNumOrbitsBelow());
   nodeInfo += ") has children ";

   bool isFirst = true;
   for(auto childIt = theChildren.begin(); childIt != theChildren.end(); childIt++)
   {
      if (isFirst)
      {
         isFirst = false;
      }
      else
      {
         nodeInfo += ", ";
      }

      OrbitNode* curChild = *childIt;
      nodeInfo += curChild->theName;
   }

   std::cout << nodeInfo << std::endl;

   for(auto childIt = theChildren.begin(); childIt != theChildren.end(); childIt++)
   {
      (*childIt)->printNodeInfo();
   }
}

int OrbitNode::getLevel()
{
   if (theParentNode == nullptr)
   {
      return 0;
   }
   else
   {
      return theParentNode->getLevel() + 1;
   }
}


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
         pos = delimPos + 1;
      }
   }

}

typedef struct orbitdef
{
   std::string parent;
   std::string child;
} OrbitDef;

OrbitNode* addNodeToRoot(OrbitNode* oldRoot, OrbitDef od)
{
   /* if (oldRoot)
   {
      std::cout << "addNodeToRoot(root=" << oldRoot->getName() << "[" << oldRoot->getNumNodes()
                << "], od{p=" << od.parent << ",c=" << od.child << "}" << std::endl;
   }
   else
   {
      std::cout << "addNodeToRoot(root=NULL, od{p=" << od.parent
                << ",c=" << od.child << "}" << std::endl;

   } */

   //OrbitNode* curNode = new OrbitNode(od.child);

   // Do we have a root node at all?
   if (oldRoot == nullptr)
   {
      OrbitNode* curNode = new OrbitNode(od.child);
      OrbitNode* rootNode = new OrbitNode(od.parent);
      rootNode->addChildOrbit(curNode);
      curNode->setParent(rootNode);
      return rootNode;
   }

   // Should this node be the new root node?
   if (oldRoot->getName() == od.child)
   {
      OrbitNode* curNode = new OrbitNode(od.parent);
      curNode->addChildOrbit(oldRoot);
      oldRoot->setParent(curNode);
      return curNode;
   }

   // Should this node be a child of another?
   OrbitNode* futureParent = oldRoot->findChild(od.parent);
   if (futureParent == nullptr)
   {
      // std::cout << "WTF, invalid parent node " << od.parent << ", completely unrelated to all other nodes" << std::endl;

      // Signal to caller that we have no idea what to do with this
      return nullptr;
   }
   else
   {
      OrbitNode* curNode = new OrbitNode(od.child);
      futureParent->addChildOrbit(curNode);
      curNode->setParent(futureParent);

      // Root node hasn't changed
      return oldRoot;
   }

}



int main(int, char**)
{
   OrbitNode* rootNode = nullptr;

   // Read in input
   std::vector<OrbitDef> odList;
   while(true)
   {
      std::string curLine;
      std::cin >> curLine;

      if (std::cin.eof())
      {
         break;
      }

      std::vector<std::string> parts = split(curLine, ")");

      if (parts.size() != 2)
      {
         std::cout << "Error on line: " << curLine << std::endl;
         return 1;
      }

      std::cout << parts[1] << " orbiting " << parts[0] << std::endl;
      OrbitDef od;
      od.parent = parts[0];
      od.child = parts[1];

      odList.push_back(od);
   }

   ;
   while(odList.size() > 0)
   {
      for(int i = 0; i < odList.size(); i++)
      {
         OrbitNode* rn = addNodeToRoot(rootNode, odList[i]);
         if (rn == nullptr)
         {
            // nowhere to put it, save it for later
            continue;
         }

         rootNode = rn;

         // Print the tree for debugging purposes
         //std::cout << std::endl;
         //std::cout << "*** ROOT ***" << std::endl;
         //rootNode->printNodeInfo();

         // std::cout << std::endl;

         odList.erase(odList.begin()+i);
      }


   }

   rootNode->printNodeInfo();

   // For part 2, find the YOU node
   OrbitNode* youNode = rootNode->findChild("YOU");
   OrbitNode* santaNode = rootNode->findChild("SAN");

   if ( (youNode == nullptr) || (santaNode == nullptr))
   {
      std::cout << "Sorry, didn't find either you or santa" << std::endl;
      return 0;
   }

   // Need to find the common node between them
   OrbitNode* commonNode = youNode;
   while(true)
   {
      commonNode = commonNode->getParent();

      if (commonNode->findChild("SAN"))
      {
         // We have the common node!!!
         std::cout << "Common node at level " << commonNode->getLevel() << std::endl;
         break;
      }
   }

   std::cout << "Santa at level " << santaNode->getLevel() << std::endl;
   std::cout << "You at level " << youNode->getLevel() << std::endl;

   // Answer is (CL - YL) + (CL - SL) = 2CL -YL - SL
   int answer =  santaNode->getLevel() + youNode->getLevel() - 2 * commonNode->getLevel() - 2;
   std::cout << "Distance = " << answer << std::endl;


   return 0;
}





