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

class ConwayCube
{
public:
	ConwayCube();

	void addStartLayer(std::vector<std::string> text);
	void dump();

	bool isActive(int x, int y, int z);
	bool isActive(Coord c);
	void setState(Coord c, bool active);

	int getNumActiveLayerZ(int z);
	int getNumActiveColX(int x);
	int getNumActiveRowY(int y);

	std::vector<Coord> getNeighborList(Coord c);
	int getNumberOfActiveNeighbors(Coord c);
	int getNumberOfActiveWorld();

	void runLifeCycle();

	void expandCube();

	int theCurX;
	int theSizeX;

	int theCurY;
	int theSizeY;

	int theCurZ;
	int theSizeZ;

	int theCycle;

	std::vector<Layer> theLayers;
};

void ConwayCube::setState(Coord c, bool active)
{
	theLayers[c.z - theCurZ][c.y - theCurY][c.x - theCurX] = (active ? '#' : '.');
}

void ConwayCube::runLifeCycle()
{
	std::vector<Coord> makeActive;
	std::vector<Coord> makeInactive;

	for(int x = theCurX; x < theCurX + theSizeX; x++)
	{
		for(int y = theCurY; y < theCurY + theSizeY; y++)
		{
			for(int z = theCurZ; z < theCurZ + theSizeZ; z++)
			{
				int numNeighborsActive = getNumberOfActiveNeighbors({x,y,z});
				DEBUG << "   " << x << "," << y << "," << z << ": "
				      << (isActive({x,y,z}) ? "ACTIVE" : "DEAD") << "   "
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
		DEBUG << "Turn Active: " << curCoord.x << "," << curCoord.y << "," << curCoord.z << std::endl;
		setState(curCoord, true);
		DEBUG << "Poopy" << std::endl;
	}
	for(auto curCoord: makeInactive)
	{
		DEBUG << "Turn Inactive: " << curCoord.x << "," << curCoord.y << "," << curCoord.z << std::endl;
		setState(curCoord, false);
	}
}

int ConwayCube::getNumberOfActiveWorld()
{
	int retVal = 0;
	for(int z = theCurZ; z < theCurZ + theSizeZ; z++)
	{
		retVal += getNumActiveLayerZ(z);
	}
	return retVal;
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
	return isActive(c.x, c.y, c.z);
}

bool ConwayCube::isActive(int x, int y, int z)
{
	//DEBUG << "isActive(" << x << "," << y << "," << z << ") = ";
	int indexX = x - theCurX;
	int indexY = y - theCurY;
	int indexZ = z - theCurZ;

	if ( (indexX < 0) || (indexX >= theSizeX) )
	{
		//DEBUG << "X out of range" << std::endl;
		return false;
	}

	if ( (indexY < 0) || (indexY >= theSizeY) )
	{
		//DEBUG << "Y out of range" << std::endl;
		return false;
	}

	if ( (indexZ < 0) || (indexZ >= theSizeZ) )
	{
		//DEBUG << "Z out of range" << std::endl;
		return false;
	}

	bool val = theLayers[indexZ][indexY][indexX] == '#';
	//DEBUG << (val ? "ACTIVE" : "INACTIVE") << std::endl;
	return val;
}


int ConwayCube::getNumActiveLayerZ(int z)
{
	int sum = 0;
	for(int x = theCurX; x < (theCurX + theSizeX); x++)
	{
		for(int y = theCurY; y < (theCurY + theSizeY); y++)
		{
			if (isActive(x, y, z))
			{
				sum++;
			}
		}
	}
	return sum;
}

int ConwayCube::getNumActiveColX(int x)
{
	int sum = 0;
	for(int z = theCurZ; z < (theCurZ + theSizeZ); z++)
	{
		for(int y = theCurY; y < (theCurY + theSizeY); y++)
		{
			if (isActive(x, y, z))
			{
				sum++;
			}
		}
	}
	return sum;
}

int ConwayCube::getNumActiveRowY(int y)
{
	int sum = 0;
	for(int x = theCurX; x < (theCurX + theSizeX); x++)
	{
		for(int z = theCurZ; z < (theCurZ + theSizeZ); z++)
		{
			if (isActive(x, y, z))
			{
				sum++;
			}
		}
	}
	return sum;
}

void ConwayCube::expandCube()
{
	// Helping objects to add if neccessary
	std::string rowText;
	for(int x = 0; x < theSizeX; x++)
	{
		rowText += ".";
	}

	Layer nl;
	for(int y = 0; y < theSizeY; y++)
	{
		nl.push_back(rowText);
	}
	
	// Only expand if the layer has 1 active cell in it, cause that cell may expand...

	if (getNumActiveLayerZ(theCurZ))
	{
		DEBUG << "Adding new layer before other layers" << std::endl;
		theLayers.insert(theLayers.begin(), nl);
		theCurZ--;
		theSizeZ++;
	}

	if (getNumActiveLayerZ(theCurZ + theSizeZ - 1))
	{
		DEBUG << "Adding new layer after other layers" << std::endl;
		theLayers.push_back(nl);
		theSizeZ++;
	}

	if (getNumActiveRowY(theCurY))
	{
		DEBUG << "Adding new row before other rows" << std::endl;
		for(auto & curL: theLayers)
		{
			curL.insert(curL.begin(), rowText);
		}
		theCurY--;
		theSizeY++;
	}
	DEBUG << "Here" << std::endl;

	if (getNumActiveRowY(theCurY + theSizeY - 1))
	{
		DEBUG << "Adding new row after the other rows" << std::endl;
		for(auto & curL: theLayers)
		{
			curL.push_back(rowText);
		}
		theSizeY++;
	}

	if (getNumActiveColX(theCurX))
	{
		DEBUG << "Adding new col before the other cols" << std::endl;
		for(auto & curL: theLayers)
		{
			for(auto & curRow: curL)
			{
				curRow.insert(curRow.begin(), '.');
			}
		}
		theCurX--;
		theSizeX++;
	}

	if (getNumActiveColX(theCurX + theSizeX - 1))
	{
		DEBUG << "Adding new col after the other cols" << std::endl;
		for(auto & curL: theLayers)
		{
			for(auto & curRow: curL)
			{
				curRow += '.';
			}
		}
		theSizeX++;
	}


	
}


ConwayCube::ConwayCube()
{
	theCurX = 0;
	theCurY = 0;
	theCurZ = 0;
	theCycle = 0;
	theSizeX = 0;
	theSizeY = 0;
	theSizeZ = 0;
}

void ConwayCube::addStartLayer(std::vector<std::string> text)
{
	theLayers.push_back(text);

	theSizeZ = 1;
	theSizeX = text[0].size();
	theSizeY = text.size();
}

void ConwayCube::dump()
{
	DEBUG << "Cyc=" << theCycle << "  X=" << theCurX << ",Y=" 
	      << theCurY << ",Z=" << theCurZ 
	      << ", totalActive=" << getNumberOfActiveWorld() << std::endl;

	int zCounter = theCurZ;
	for(auto curLayer: theLayers)
	{
		DEBUG << "  Z = " << zCounter++ << std::endl;
		for(auto curRow: curLayer)
		{
			DEBUG << curRow << std::endl;
		}

		DEBUG << std::endl;
	}
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

		DEBUG << "Expanding cube" << std::endl;
		cc.expandCube();

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
