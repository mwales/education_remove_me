#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

#define Layer std::vector<std::string> 

typedef struct CoordStruct
{
	int x;
	int y;
	int z;
} Coord;

std::ostream& operator<< (std::ostream& o, Coord c)
{
	return o << "(" << c.x << "," << c.y << "," << c.z << ")";
}

bool operator<(Coord const & lhs, Coord const & rhs)
{
	if (lhs.x < rhs.x)
                return true;
        if (lhs.x > rhs.x)
                return false;

        if (lhs.y < rhs.y)
                return true;
        if (lhs.y > rhs.y)
                return false;

        if (lhs.z < rhs.z)
                return true;
        else
                return false;
}

class ConwayCube
{
public:
	ConwayCube();

	void addStartLayer(std::vector<std::string> text);
	void dump();

	bool isActive(Coord c);
	void setState(Coord c, bool active);
	void updateMinMax(Coord c);

	std::vector<Coord> getNeighborList(Coord c);
	int getNumberOfActiveNeighbors(Coord c);
	int getNumberOfActiveWorld();

	void runLifeCycle();


	int theCycle;

	std::set<Coord> theActiveCells;

	Coord theMinC;
	Coord theMaxC;
};

void ConwayCube::updateMinMax(Coord c)
{
	theMinC.x = c.x < theMinC.x ? c.x : theMinC.x;
	theMinC.y = c.y < theMinC.y ? c.y : theMinC.y;
	theMinC.z = c.z < theMinC.z ? c.z : theMinC.z;

	theMaxC.x = c.x > theMaxC.x ? c.x : theMaxC.x;
	theMaxC.y = c.y > theMaxC.y ? c.y : theMaxC.y;
	theMaxC.z = c.z > theMaxC.z ? c.z : theMaxC.z;
}

void ConwayCube::setState(Coord c, bool active)
{
	if (active)
	{
		theActiveCells.emplace(c);
		updateMinMax(c);
	}
	else
	{
		auto it = theActiveCells.find(c);
		if (it != theActiveCells.end())
		{
			theActiveCells.erase(it);
		}
	}
}

void ConwayCube::runLifeCycle()
{
	std::vector<Coord> makeActive;
	std::vector<Coord> makeInactive;

	std::cout << "Cycle " << theCycle << std::endl;

	for(int x = theMinC.x - 1; x <= theMaxC.x + 1; x++)
	{
		for(int y = theMinC.y - 1; y <= theMaxC.y + 1; y++)
		{
			for(int z = theMinC.z - 1; z <= theMaxC.z + 1; z++)
			{
				Coord c = {x,y,z};
				int numNeighborsActive = getNumberOfActiveNeighbors(c);
				DEBUG << "   " << c << ": "
				      << (isActive(c) ? "ACTIVE" : "DEAD") << "   "
				      << numNeighborsActive << std::endl;

				if (isActive({x,y,z}))
				{
					if ( (numNeighborsActive < 2) || (numNeighborsActive > 3) )
					{
						makeInactive.push_back({x,y,z});
					}
				}
				else
				{
					if (numNeighborsActive == 3)
					{
						makeActive.push_back({x,y,z});
					}
				}
			}
		}
	}

	for(auto curCoord: makeActive)
	{
		DEBUG << "Turn Active: " << curCoord << std::endl;
		setState(curCoord, true);
		DEBUG << "Poopy" << std::endl;
	}
	for(auto curCoord: makeInactive)
	{
		DEBUG << "Turn Inactive: " << curCoord << std::endl;
		setState(curCoord, false);
	}

	theCycle++;
}

int ConwayCube::getNumberOfActiveWorld()
{
	return theActiveCells.size();
}

std::vector<Coord> ConwayCube::getNeighborList(Coord c)
{
	std::vector<Coord> retVal;
	for(int x = c.x - 1; x <= c.x + 1; x++)
	{
		for(int y = c.y - 1; y <= c.y + 1; y++)
		{
			for(int z = c.z - 1; z <= c.z + 1; z++)
			{
				if ( (x == c.x) && (y == c.y) && (z == c.z) )
				{
					// Don't add self
					continue;
				}
				
				retVal.push_back({x,y,z});
			}
		}
	}
	return retVal;
}


int ConwayCube::getNumberOfActiveNeighbors(Coord c)
{
	std::vector<Coord> nlist = getNeighborList(c);

	int retVal = 0;
	for(auto pt: nlist)
	{
		if (isActive(pt))
		{
			retVal += 1;
		}
	}
	return retVal;
}

bool ConwayCube::isActive(Coord c)
{

	//DEBUG << "isActive(" << c << std::endl;
	return theActiveCells.find(c) != theActiveCells.end();
}

ConwayCube::ConwayCube()
{
	theMinC = {0,0,0};
	theMaxC = {0,0,0};
}

void ConwayCube::addStartLayer(std::vector<std::string> text)
{
	int x = 0;
	int y = 0;
	for(auto curRow: text)
	{
		x = 0;
		for(auto curChar: curRow)
		{
			if (curChar == '#')
			{
				Coord c = {x,y,0};
				setState(c, true);
			}
			x++;
		}
		y++;
	}
}

void ConwayCube::dump()
{
	DEBUG << "Cyc=" << theCycle << "  MinC=" << theMinC
	      << ", theMaxC=" << theMaxC << " totalActive=" << getNumberOfActiveWorld() << std::endl;

	for(int z = theMinC.z; z <= theMaxC.z; z++)
	{
		for(int y = theMinC.y; y <= theMaxC.y; y++)
		{
			for(int x = theMinC.x; x <= theMaxC.x; x++)
			{
				Coord c = {x,y,z};
				DEBUG << (isActive(c) ? '#' : '.');
			}
			DEBUG << std::endl;
		}

		DEBUG << std::endl;
	}

	debugPrintSet(theActiveCells);
	DEBUG << std::endl;
								

}

int main(int argc, char** argv)
{

	if (argc < 3)
	{
		std::cerr << "Provide filename and cycle count" << std::endl;
		return 0;
	}

	std::vector<std::string> startLayer;

	std::ifstream infile(argv[1]);

	while(infile.is_open())
	{
		std::string text;
		std::getline(infile,text);


		// out of output
check_for_eof:
      if (infile.eof())
		{
			break;
		}

		DEBUG << text << std::endl;
		startLayer.push_back(text);
	}

	infile.close();

	ConwayCube cc;
	cc.addStartLayer(startLayer);

	DEBUG << "Starting State: " << std::endl;
	cc.dump();

	int numIters = atoi(argv[2]);

	for(int it = 0; it < numIters; it++)
	{


		cc.dump();

		DEBUG << std::endl << std::endl;
	
		DEBUG << "Run life cycle" << std::endl;
		cc.runLifeCycle();

		DEBUG << "After a single life cycle" << std::endl;
		cc.dump();
	}

	std::cout << "Pt1 Num Total Active after " << numIters << " iterations: "
	          << cc.getNumberOfActiveWorld() << std::endl;

	return 0;
}
