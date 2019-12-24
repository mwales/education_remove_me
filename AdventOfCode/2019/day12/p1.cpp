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

bool operator==(Planet const & lhs, Planet const & rhs)
{
  return (lhs.theId == rhs.theId);
}

bool operator!=(Planet const & lhs, Planet const & rhs)
{
   return !(lhs == rhs);
}

int main(int, char**)
{
	std::string currentInput;
	std::vector<Planet> planets;
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

   for(int i = 0; i < numIterations; i++)
   {
      std::cout << "After " << i << " iterations:" << std::endl;
      for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
      {
         std::cout << pIt->toString() << std::endl;
      }
      std::cout << std::endl;

      // Compute gravity
      std::vector<Planet> oldPlanets = planets;
      for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
      {
         pIt->computeGravity(oldPlanets);
      }
   }

   std::cout << "After all iterations:" << std::endl;
   for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
   {
      std::cout << pIt->toString() << std::endl;
   }
   std::cout << std::endl;
	
   int totalEnergy = 0;
   for(auto pIt = planets.begin(); pIt != planets.end(); pIt++)
   {
      totalEnergy += pIt->computeEnergy();
   }

   std::cout << "Total Energy = " << totalEnergy << std::endl;


	return 0;
}

