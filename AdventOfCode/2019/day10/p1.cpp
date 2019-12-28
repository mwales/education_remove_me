#include <iostream>
#include <vector>
#include <map>
#include <set>

void reduceRatio(int & x, int & y)
{

	int minimumVal = abs(x);
	if (abs(y) < abs(x))
	{
		minimumVal = abs(y);
	}

	if (minimumVal == 0)
	{
		// Special case, horizontal or vertical line
		if (x == 0)
		{
			y /= abs(y);
		}
		else
		{
			x /= abs(x);
		}

		return;

	}

	if (minimumVal <= 1)
	{
		// This is the minimum ratio if one of the numbers is 1
		return;
	}

	for(int i = 2; i <= minimumVal; i++)
	{
		if ( (x % i == 0) && (y % i == 0) )
		{
			x /= i;
			y /= i;
			return reduceRatio(x, y);
		}
	}

}


class AsteroidMap
{
	public:
		AsteroidMap(std::vector<std::string> data);

		void printMap();

		bool isAsteroid(int x,int y);

		int numAsteroids();

		void scanSystem();

		void printBestLocation(void);

		

	protected:

		void identifyHiddenPlanets(std::pair<int, int> curCoord,
				           std::pair<int, int> otherCoord,
					   std::set<std::pair<int, int> > & hiddenPlanetList);

		std::map<std::pair<int, int>, int> theMap;

		int theWidth;

		int theHeight;


};

AsteroidMap::AsteroidMap(std::vector<std::string> inputData):
	theWidth(-1),
	theHeight(-1)
{
	theHeight = inputData.size();

	if (theHeight == 0)
	{
		std::cerr << "No rows in the map, fail" << std::endl;
		exit(1);
	}

	theWidth = inputData.at(0).size();
	
	int y = 0;
	for(auto it = inputData.begin(); it != inputData.end(); it++)
	{
		if (theWidth != it->size())
		{
			std::cerr << "The rows are not all the same width!" << std::endl;
			exit(1);
		}

		for(int x = 0; x < theWidth; x++)
		{
			if ( it->at(x) == '#' )
			{
				// Add this asteroid to our map
				std::pair<int, int> coord = std::make_pair(x, y);
				theMap[coord] = -1;
			}
		}

		y++;
	}





}

void AsteroidMap::printMap()
{
	for(int y = 0; y < theHeight; y++)
	{
		for(int x = 0; x < theWidth; x++)
		{
			if (isAsteroid(x,y))
			{
				std::cout << "#";
			}
			else
			{
				std::cout << ".";
			}
		}

		std::cout << std::endl;
	}
}

bool AsteroidMap::isAsteroid(int x, int y)
{
	std::pair<int, int> coord = std::make_pair(x,y);
	if (theMap.find(coord) == theMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

int AsteroidMap::numAsteroids()
{
	return theMap.size();
}

void AsteroidMap::scanSystem()
{
	for(auto aIt = theMap.begin(); aIt != theMap.end(); aIt++)
	{
		std::pair<int, int> curCoord = aIt->first;
		std::set<std::pair<int, int> > hiddenPlanets;

		std::cout << "Scanning " << curCoord.first << "," << curCoord.second << std::endl;

		for(auto otherIt = theMap.begin(); otherIt != theMap.end(); otherIt++)
		{
			std::pair<int, int> otherCoord = otherIt->first;

			if (otherCoord == curCoord)
			{
				// Dont investigate ourselves
				continue;
			}

			identifyHiddenPlanets(curCoord, otherCoord, hiddenPlanets);

		}

		int numAsteroidsDetected = numAsteroids() - hiddenPlanets.size() - 1;
		std::cout << "The planet @ " << curCoord.first << "," << curCoord.second << " can see " << numAsteroidsDetected << std::endl;

		theMap[curCoord] = numAsteroidsDetected;
	}
}

void AsteroidMap::printBestLocation(void)
{
	std::pair<int, int> bestCoord;

	int numDetections = -1;
	for(auto it = theMap.begin(); it != theMap.end(); it++)
	{
		if (numDetections < it->second)
		{
			bestCoord = it->first;
			numDetections = it->second;
		}
	}

	std::cout << "Best location @ " << bestCoord.first << "," << bestCoord.second << " with "
		<< numDetections << " detections" << std::endl;

}

void AsteroidMap::identifyHiddenPlanets(std::pair<int, int> curCoord,
				           std::pair<int, int> otherCoord,
					   std::set<std::pair<int, int> > & hiddenPlanetList)
{
	int deltaX = otherCoord.first - curCoord.first;
	int deltaY = otherCoord.second - curCoord.second;
	reduceRatio(deltaX, deltaY);

	int xCheck = otherCoord.first;
	int yCheck = otherCoord.second;
	while(true)
	{
		xCheck += deltaX;
		yCheck += deltaY;

		// Is this still inbounds?
		if ( (xCheck < 0) || (xCheck > theWidth) )
		{
			// We are out of bounds
			return;
		}

		if ( (yCheck < 0) || (yCheck > theHeight ) )
		{
			return;
		}

		std::pair<int, int> checkCoord = std::make_pair(xCheck, yCheck);

		if (theMap.find(checkCoord) != theMap.end())
		{
			// We found an asteroid at the location!  It will be hidden by otherCoord
			hiddenPlanetList.emplace(checkCoord);

			std::cout << "Asteroid @ " << xCheck << "," << yCheck 
				<< " will be obscured by " << otherCoord.first << "," 
				<< otherCoord.second << std::endl;
		}
				
	}

}



int main(int, char**)
{
	std::vector<std::string> mapData;

	while(true)
	{
		std::string singleLine;
		std::cin >> singleLine;

		if (std::cin.eof())
		{
			break;
		}

		mapData.push_back(singleLine);
	}

	std::cout << "Number of lines = " << mapData.size() << std::endl;

	AsteroidMap am(mapData);
	am.printMap();

	std::cout << "Number of asteroids = " << am.numAsteroids() << std::endl;

	am.scanSystem();

	am.printBestLocation();

	return 0;
}
