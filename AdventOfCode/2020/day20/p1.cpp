#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <assert.h>
#include <math.h>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

#define TILE_SIZE 10

class Tile
{
public:
	Tile(std::vector<std::string> const & imageData);

	uint64_t getTileNumber() const;

	int getRotation() const;

	void printTile() const;

	// rotate clockwise 90deg
	void rotateTile();

	bool matchRightSide(Tile const & rhs);

	bool getState(int x, int y) const;

	std::vector<std::string> theData;

	uint64_t theTileNum;

	int theNumRotations;

	std::string theMatchLog;
};

bool Tile::getState(int x, int y) const
{
	return theData[y][x] == '#';
}

int Tile::getRotation() const
{
	return theNumRotations;
}


bool Tile::matchRightSide(Tile const & rhs)
{
	for(int i = 0; i < TILE_SIZE; i++)
	{
		if (getState(TILE_SIZE-1, i) != rhs.getState(0,i) )
		{
			return false;
		}
	}

	// Every point on our right side matched left side of other tile
	theMatchLog += "Match.  My rot=";
	theMatchLog += std::to_string(theNumRotations);
	theMatchLog += ",otherTile=";
	theMatchLog += std::to_string(rhs.getTileNumber());
	theMatchLog += ",otherTileRot=";
	theMatchLog += std::to_string(rhs.getRotation());
	theMatchLog += ". ";

	return true;
}

Tile::Tile(std::vector<std::string> const & imageData)
{
	assert(imageData.size() == TILE_SIZE + 1);

	// Tiles are 10x10, with 1 row tile id header
	std::vector<char> delimiters = { ' ', ':' };
	std::vector<std::string> idParts = stringSplit(imageData[0], delimiters);
	assert(idParts.size() == 3);

	DEBUG << "HeaderParts=";
	debugPrintVector(idParts);
	DEBUG << std::endl;

	theTileNum = atoi(idParts[1].c_str());

	for(int i = 1; i <= TILE_SIZE; i++)
	{
		theData.push_back(imageData[i]);
	}

	theMatchLog = "Log Start: ";
}

uint64_t Tile::getTileNumber() const
{
	return theTileNum;
}

void Tile::printTile() const
{
	DEBUG << "Tile: " << theTileNum << "   Rot=" << theNumRotations << std::endl;
	for(auto eachLine: theData)
	{
		DEBUG << "  " << eachLine << std::endl;
	}
}

void Tile::rotateTile()
{
	std::vector<std::string> theOldData = theData;
	theData.clear();

	for(int y = 0; y < TILE_SIZE; y++)
	{
		std::string singleRow;
		for(int x = 0; x < TILE_SIZE; x++)
		{
			if ( (theNumRotations == 3) || (theNumRotations == 7) )
			{
				// Special rotation case, the mirror flip
				singleRow += theOldData[y][TILE_SIZE-x-1];
			}
			else
			{
				// Rotations 90 deg clockwise
				singleRow += theOldData[TILE_SIZE-x-1][y];
			}
		}
		theData.push_back(singleRow);
	}


	theNumRotations = (theNumRotations + 1) % 8;
}


int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);
	std::vector<std::string> imageData;
	std::vector<Tile*> tileSet;

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

		if (text == "")
		{
			Tile* t = new Tile(imageData);
			imageData.clear();

			t->printTile();
			tileSet.push_back(t);
		}
		else
		{
			imageData.push_back(text);
		}
	}

	Tile* t = new Tile(imageData);
	t->printTile();
	tileSet.push_back(t);

	infile.close();

	std::vector<int> numberOfOtherMatches;
	for(int i = 0; i < tileSet.size(); i++)
	{
		/*std::vector<Tile> allRotations;
		for(int nr = 0; nr < 8; nr++)
		{
			allRotations.push_back(*tileSet[i]);
			tileSet[i]->rotateTile();
		}*/

		int curMatches = 0;
		for(int j = 0; j < tileSet.size(); j++)
		{
			DEBUG << "Comparing " << i << "(" << tileSet[i]->getTileNumber() << ") to "
			      << j << "(" << tileSet[j]->getTileNumber() << std::endl;
			
			if (i == j)
			{
				continue;
			}
			
			//DEBUG << "Intial state of J tile: " << std::endl;
			//tileSet[j]->printTile();

			for(int nrself = 0; nrself < 8; nrself++)
			{
				tileSet[i]->rotateTile();

				for(int nrother = 0; nrother < 8; nrother++)
				{
					
					if (tileSet[i]->matchRightSide(*tileSet[j]))
					{
						DEBUG << "MATCH:  tileSet" << i << "(" << tileSet[i]->getTileNumber() 
						      << ") rotated " << nrself
						      << " times, matches against tileSet" << j
						      << "(" << tileSet[j]->getTileNumber() << ") rotated "
						      << nrother << " times" << std::endl;
						curMatches++;
					}

					tileSet[j]->rotateTile();
					//DEBUG << "Other tile now rotated to :" << std::endl;
					//tileSet[j]->printTile();
				}
			}
		}

		numberOfOtherMatches.push_back(curMatches);
	}

	uint64_t part1Sol = 1;
	for(int i = 0; i < tileSet.size(); i++)
	{
		DEBUG << "Tile: " << tileSet[i]->getTileNumber() << " = " << numberOfOtherMatches[i] << " matches" << std::endl;
		DEBUG << "      " << tileSet[i]->theMatchLog << std::endl;

		if (numberOfOtherMatches[i] == 4)
		{
			DEBUG << tileSet[i]->getTileNumber() << " is a corner!" << std::endl;
			part1Sol *= tileSet[i]->getTileNumber();
		}
	}

	std::cout << "Part 1 = " << part1Sol << std::endl;	

	return 0;
}
