#include <iostream>
#include <vector>
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
         pos = delimPos + 1;
      }
   }

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

class Planet
{
public:
   Planet(std::string data);

   std::string toString();

   void computeGravity(std::vector<Planet> allPlanets);

   int computeEnergy();

   bool comparePlanetTelemetry(Planet const & other) const;

   // I initially tried to brute force this whole thing, it takes many hours
   // just for the sample, the real problem would have been impossible without
   // this optimization of finding out how long each axis takes to repeat. I
   // read about this optimization on reddit, not my original idea...
   bool compareXTelemetry(Planet const & other) const;
   bool compareYTelemetry(Planet const & other) const;
   bool compareZTelemetry(Planet const & other) const;


   friend bool operator==(Planet const & lhs, Planet const & rhs);
   friend bool operator!=(Planet const & lhs, Planet const & rhs);
private:

   int theX;
   int theY;
   int theZ;

   int theVx;
   int theVy;
   int theVz;

   int theId;

   static int theUniqueIdCounter;
};

int Planet::theUniqueIdCounter = 0;

Planet::Planet(std::string data)
{
   std::vector<std::string> components = split(data, ",");

   if (components.size() != 3)
   {
      std::cerr << "Not enough parts in: " << data << std::endl;
      exit(1);
   }

   std::vector<int*> valPtrs;
   valPtrs.push_back(&theX);
   valPtrs.push_back(&theY);
   valPtrs.push_back(&theZ);

   for(int i = 0; i < 3; i++)
   {
      std::vector<std::string> compParts = split(components.at(i), "=");

      if (compParts.size() != 2)
      {
         std::cerr << "Error parsing the component: " << components.at(i) << std::endl;
         exit(1);
      }

      int* curVal = valPtrs.at(i);

      if (compParts.size() != 2)
      {
         std::cerr << "Splitting on the equals failed for: " << components.at(i) << std::endl;
         exit(1);
      }

      // std::cout << "Cur Val Str: " << compParts.at(1) << std::endl;
      *curVal = atoi(compParts.at(1).c_str());
      // std::cout << "Found value: " << *curVal << std::endl;

   }

   theVx = 0;
   theVy = 0;
   theVz = 0;

   theId = theUniqueIdCounter++;
}

std::string Planet::toString()
{
   std::string retVal;
   retVal += "(";
   retVal += std::to_string(theId);
   retVal += "): x=";
   retVal += std::to_string(theX);
   retVal += ",y=";
   retVal += std::to_string(theY);
   retVal += ",z=";
   retVal += std::to_string(theZ);
   retVal += "\tVx=";
   retVal += std::to_string(theVx);
   retVal += ",Vy=";
   retVal += std::to_string(theVy);
   retVal += ",Vz=";
   retVal += std::to_string(theVz);
   return retVal;
}

void Planet::computeGravity(std::vector<Planet> allPlanets)
{
   // Zero out velocities to begin calculation
   //theVx = 0;
   //theVy = 0;
   //theVz = 0;

   for(auto pIt = allPlanets.begin(); pIt != allPlanets.end(); pIt++)
   {
      if (*pIt == *this)
      {
         // Don't compute gravity against yourself
         continue;
      }

      if (pIt->theX > theX)
      {
         theVx += 1;
      }
      else if (pIt->theX < theX)
      {
         theVx -= 1;
      }

      if (pIt->theY > theY)
      {
         theVy += 1;
      }
      else if (pIt->theY < theY)
      {
         theVy -= 1;
      }

      if (pIt->theZ > theZ)
      {
         theVz += 1;
      }
      else if (pIt->theZ < theZ)
      {
         theVz -= 1;
      }
   }

   // Velocity updated, now apply it to adjust position...
   theX += theVx;
   theY += theVy;
   theZ += theVz;
}

int Planet::computeEnergy()
{
   int potE = abs(theX) + abs(theY) + abs(theZ);
   int kinE = abs(theVx) + abs(theVy) + abs(theVz);

   std::cout << "(" << theId << ") PotE=" << potE << ", KinE=" << kinE << std::endl;

   return potE * kinE;
}

bool Planet::comparePlanetTelemetry(Planet const & other) const
{
   if ( (other.theX != theX) || (other.theY != theY) || (other.theZ != theZ) )
   {
      return false;
   }

   if ( (other.theVx != theVx) || (other.theVy != theVy) || (other.theVz != theVz) )
   {
      return false;
   }

   return true;
}

bool Planet::compareXTelemetry(Planet const & other) const
{
   return ( (other.theX == theX) && (other.theVx == theVx) );
}

bool Planet::compareYTelemetry(Planet const & other) const
{
   return ( (other.theY == theY) && (other.theVy == theVy) );
}

bool Planet::compareZTelemetry(Planet const & other) const
{
   return ( (other.theZ == theZ) && (other.theVz == theVz) );
}


bool operator==(Planet const & lhs, Planet const & rhs)
{
   return (lhs.theId == rhs.theId);
}

bool operator!=(Planet const & lhs, Planet const & rhs)
{
   return !(lhs == rhs);
}

bool operator==(std::vector<Planet> const & lhs, std::vector<Planet> const & rhs)
{
   if (lhs.size() != rhs.size())
   {
      return false;
   }

   // std::cout << "Sizes " << rhs.size() << " are same, woot" << std::endl;

   for(int i = 0; i < lhs.size(); i++)
   {
      if (! lhs.at(i).comparePlanetTelemetry(rhs.at(i)))
      {
         return false;
      }
   }

   return true;
}

bool compareSystemX(std::vector<Planet> const & lhs, std::vector<Planet> const & rhs)
{
   if (lhs.size() != rhs.size())
   {
      return false;
   }

   // std::cout << "Sizes " << rhs.size() << " are same, woot" << std::endl;

   for(int i = 0; i < lhs.size(); i++)
   {
      if (! lhs.at(i).compareXTelemetry(rhs.at(i)))
      {
         return false;
      }
   }

   return true;
}

bool compareSystemY(std::vector<Planet> const & lhs, std::vector<Planet> const & rhs)
{
   if (lhs.size() != rhs.size())
   {
      return false;
   }

   // std::cout << "Sizes " << rhs.size() << " are same, woot" << std::endl;

   for(int i = 0; i < lhs.size(); i++)
   {
      if (! lhs.at(i).compareYTelemetry(rhs.at(i)))
      {
         return false;
      }
   }

   return true;
}

bool compareSystemZ(std::vector<Planet> const & lhs, std::vector<Planet> const & rhs)
{
   if (lhs.size() != rhs.size())
   {
      return false;
   }

   // std::cout << "Sizes " << rhs.size() << " are same, woot" << std::endl;

   for(int i = 0; i < lhs.size(); i++)
   {
      if (! lhs.at(i).compareZTelemetry(rhs.at(i)))
      {
         return false;
      }
   }

   return true;
}

bool operator!=(std::vector<Planet> const & lhs, std::vector<Planet> const & rhs)
{
   return !(lhs == rhs);
}

void printAllPlanets(std::vector<Planet> & planets)
{
   for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
   {
      std::cout << pIt->toString() << std::endl;
   }
   std::cout << std::endl;
}

/**
 * @brief Least Common Multiple
 *
 * Not very efficient, there is a website that does this instantly, mine takes
 * minutes for same problem.
 */
int64_t lcm(std::vector<int64_t> vals)
{
   // Find the smallest number
   int64_t smallest = vals.at(0);

   for(auto it = vals.begin(); it != vals.end(); it++)
   {
      if (*it < smallest)
      {
         smallest = *it;
      }
   }

   int64_t retVal = 0;
   while(true)
   {
      retVal += smallest;

      // Is the current value divisible by everything?
      bool invalid = false;
      for(auto it = vals.begin(); it != vals.end(); it++)
      {
         if ( (retVal % *it) != 0)
         {
            invalid = true;
            break;
         }

      }

      if (!invalid)
      {
         return retVal;
      }
   }
}

int main(int, char**)
{
   std::string currentInput;
   std::vector<Planet> planets;

   // Not used for this part of the problem
   int numIterations;
   std::cin >> numIterations;

   while(true)
   {
      std::cin >> currentInput;

      std::cout << "Output: " << currentInput << std::endl;

      if (std::cin.eof())
      {
         break;
      }

      Planet p(currentInput);
      std::cout << p.toString() << std::endl;

      planets.push_back(p);
   }

   std::cout << "Done loading planets!" << std::endl;
   std::vector<Planet> originalPlanets = planets;

   int64_t i = 0;
   int64_t xRepeat = -1;
   int64_t yRepeat = -1;
   int64_t zRepeat = -1;
   while(true)
   {
      if ((i % 100000) == 0)
      {
         std::cout << "After " << i << " iterations:" << std::endl;
         printAllPlanets(planets);
      }

      // Compute gravity
      std::vector<Planet> oldPlanets = planets;
      for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
      {
         pIt->computeGravity(oldPlanets);
      }

      i++;

      if ( (xRepeat == -1) && compareSystemX(originalPlanets, planets))
      {
         std::cout << "X axis repeats after " << i << " iterations" << std::endl;
         xRepeat = i;
      }

      if ( (yRepeat == -1) && compareSystemY(originalPlanets, planets))
      {
         std::cout << "Y axis repeats after " << i << " iterations" << std::endl;
         yRepeat = i;
      }

      if ( (zRepeat == -1) && compareSystemZ(originalPlanets, planets))
      {
         std::cout << "Z axis repeats after " << i << " iterations" << std::endl;
         zRepeat = i;
      }

      if ( (xRepeat != -1) && (yRepeat != -1) && (zRepeat != -1) )
      {
         std::cout << "All have repeated!" << std::endl;
         break;
      }
   }

   std::vector<int64_t> loopVals;
   loopVals.push_back(xRepeat);
   loopVals.push_back(yRepeat);
   loopVals.push_back(zRepeat);

   std::cout << "LCM = " << lcm(loopVals) << std::endl;


   return 0;
}

