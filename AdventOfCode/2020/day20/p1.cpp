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

	bool matchBottomSide(Tile const & rhs);

	bool getState(int x, int y) const;

	int searchForSeaMonster();

	int getNumActiveCells();

	std::vector<std::string> theData;

	uint64_t theTileNum;

	int theNumRotations;

	std::string theMatchLog;

	int theTileSize;

	std::set<uint64_t> theMatchingTiles;
};

class TileMap
{
public:
	TileMap(std::vector<Tile*> tileSet);

	void findCorners();

	void placeCornerTopLeft();

	void placeRemainingTiles();

	void printEntireMap();

	std::vector<std::string> createMegaTile();

	Tile* getTileByNumber(uint64_t num);

	std::vector<int> getCornerList();

	std::vector<int> theCornerList;

	std::vector<Tile*> theTiles;

	std::map<std::pair<int, int>, Tile*> theMap;

	int theWidth;

	int theHeight;
};


Tile* TileMap::getTileByNumber(uint64_t num)
{
	for(auto t: theTiles)
	{
		if (t->getTileNumber() == num)
		{
			return t;
		}
	}
	
	DEBUG << "There is not tile number " << num << std::endl;
	assert(false);
	return nullptr;
}

std::vector<std::string> TileMap::createMegaTile()
{
	assert(theMap.size() == theTiles.size());
	std::vector<std::string> retVal;

	for(int curY = 0; curY < theHeight; curY++)
	{
		for(int tileRow = 1; tileRow < theTiles[0]->theTileSize - 1; tileRow++)
		{

			std::string singleRow;
			for(int curX = 0; curX < theWidth; curX++)
			{

				for(int subX = 1; subX < theTiles[0]->theTileSize - 1; subX++)
				{
					singleRow += theMap[{curX, curY}]->theData[tileRow][subX];
				}

				//DEBUG << theMap[{curX, curY}]->theData[tileRow] << " ";

			}
			retVal.push_back(singleRow);
			//DEBUG << std::endl;

		}

		//DEBUG << std::endl;
	}

	//DEBUG << std::endl;
	return retVal;
}

void TileMap::printEntireMap()
{
	assert(theMap.size() == theTiles.size());

	for(int curY = 0; curY < theHeight; curY++)
	{
		for(int tileRow = 0; tileRow < theTiles[0]->theTileSize; tileRow++)
		{
			for(int curX = 0; curX < theWidth; curX++)
			{
				DEBUG << theMap[{curX, curY}]->theData[tileRow] << " ";
			}

			DEBUG << std::endl;

		}

		DEBUG << std::endl;
	}

	DEBUG << std::endl;
}

void TileMap::placeRemainingTiles()
{
	int curX = 2;
	int curY = 0;

	std::set<Tile*> remainingTiles;
	for(auto t: theTiles)
	{
		remainingTiles.emplace(t);
	}

	// Remove the 3 tiles we have already placed
	remainingTiles.erase(theMap[{0,0}]);
	remainingTiles.erase(theMap[{0,1}]);
	remainingTiles.erase(theMap[{1,0}]);

	DEBUG << "Remaining tile to place = " << remainingTiles.size() << std::endl;

	// Do top row
	while(curY == 0)
	{
		Tile* lt = theMap[{curX-1,curY}];

		bool foundTile = false;
		for(auto possibleTile: remainingTiles)
		{

			for(int rot = 0; rot < 8; rot++)
			{
				possibleTile->rotateTile();

				if (lt->matchRightSide(*possibleTile))
				{
					// We found the next tile
					DEBUG << "Tile at " << curX << "," << curY << " is " << possibleTile->getTileNumber() << std::endl;
					theMap[{curX,curY}] = possibleTile;

					// Don't rotate this tile anymore!
					foundTile = true;
					rot = 8;
					remainingTiles.erase(possibleTile);
				}
			}

			if (foundTile)
				break;
		}

		if (foundTile)
		{
			curX++;
			DEBUG << "Moving to next X " << curX << std::endl;
		}
		else
		{
			DEBUG << "We didn't find any more tiles on this row.  Width = " << curX << std::endl;
			theWidth = curX;
			theHeight = theTiles.size() / theWidth;
			DEBUG << "The height will be " << theHeight << std::endl;
			curY++;
			curX = 0;
		}

	}

	for(curY = 1; curY < theHeight; curY++)
	{
		for(curX = 0; curX < theWidth; curX++)
		{
			// Super special case, 0,1 already solved
			if ( (curX == 0) && (curY == 1))
				continue;

			bool foundTile = false;
			for(auto possibleTile: remainingTiles)
			{

				for(int rot = 0; rot < 8; rot++)
				{
					possibleTile->rotateTile();

					bool matchSuccess;
					if (curX == 0)
					{
						// Special case, we are matching against the tile above us!
						Tile* above = theMap[{0,curY - 1}];
						matchSuccess = above->matchBottomSide(*possibleTile);
					}
					else
					{
						Tile* before = theMap[{curX - 1, curY}];
						matchSuccess = before->matchRightSide(*possibleTile);
					}

					if (matchSuccess)
					{
						// We found the next tile
						DEBUG << "Tile at " << curX << "," << curY << " is " << possibleTile->getTileNumber() << std::endl;
						theMap[{curX,curY}] = possibleTile;

						// Don't rotate this tile anymore!
						foundTile = true;
						rot = 8;
						remainingTiles.erase(possibleTile);
					}
				}

				if (foundTile)
					break;
			}
		}
	}


}

void TileMap::placeCornerTopLeft()
{
	DEBUG << "placeCornerTopLeft entered" << std::endl;
	assert(theCornerList.size() == 4);

	// Any corner can be top left due to all the rotations
	std::pair<int, int> tlc = {0,0};

	Tile* tlt = getTileByNumber(theCornerList[0]);
	theMap[tlc] = tlt;
	DEBUG << "Top Left Tile = " << tlt->getTileNumber() << std::endl;

	std::vector<Tile*> nearbyTiles;
	for(auto matchingTile: tlt->theMatchingTiles)
	{
		Tile* t = getTileByNumber(matchingTile);
		nearbyTiles.push_back(t);
		DEBUG << "Nearby tile: " << t->getTileNumber() << std::endl;
	}

	assert(tlt->theMatchingTiles.size() == 2);

	// It only has 2 matches, 1 should match for the bottom, one for the right
	for(int selfRotations = 0; selfRotations < 8; selfRotations++)
	{
		tlt->rotateTile();

		for(int rightTileIndex = 0; rightTileIndex < 2; rightTileIndex++)
		{
			Tile* rt = nearbyTiles[rightTileIndex];
			Tile* bt = nearbyTiles[(rightTileIndex + 1) % 2];

			DEBUG << "Testing Right Tile = " << rt->getTileNumber() << ", and bottom tile = " 
			      << bt->getTileNumber() << std::endl;

			for(int rightTileRots = 0; rightTileRots < 8; rightTileRots++)
			{
				rt->rotateTile();

				for(int bottomTileRots = 0; bottomTileRots < 8; bottomTileRots++)
				{
					bt->rotateTile();

					if (tlt->matchRightSide(*rt) && tlt->matchBottomSide(*bt))
					{
						// Everything is rotated just right!

						DEBUG << "Top Left Tile:" << std::endl;
						tlt->printTile();
						DEBUG << std::endl << "Bottom Tile: " << std::endl;
						bt->printTile();
						DEBUG << std::endl << "Right Tile:" << std::endl;
						rt->printTile();

						std::pair<int, int> rtc = {1,0};
						std::pair<int, int> btc = {0,1};
						theMap[rtc] = rt;
						theMap[btc] = bt;
						return;
					}
				}
			}
		}


	}

	DEBUG << "We tried all the rotations, can't make top-left work.  We are dumb" << std::endl;
}

TileMap::TileMap(std::vector<Tile*> tileSet)
{
	theTiles = tileSet;
}

void TileMap::findCorners()
{
	std::vector<int> numberOfOtherMatches;
	for(int i = 0; i < theTiles.size(); i++)
	{

		int curMatches = 0;
		for(int j = 0; j < theTiles.size(); j++)
		{
			DEBUG << "Comparing " << i << "(" << theTiles[i]->getTileNumber() << ") to "
			      << j << "(" << theTiles[j]->getTileNumber() << std::endl;
			
			if (i == j)
			{
				continue;
			}
			
			//DEBUG << "Intial state of J tile: " << std::endl;
			//tileSet[j]->printTile();

			for(int nrself = 0; nrself < 8; nrself++)
			{
				theTiles[i]->rotateTile();

				for(int nrother = 0; nrother < 8; nrother++)
				{
					
					if (theTiles[i]->matchRightSide(*theTiles[j]))
					{
						DEBUG << "MATCH:  tileSet" << i << "(" << theTiles[i]->getTileNumber() 
						      << ") rotated " << nrself
						      << " times, matches against tileSet" << j
						      << "(" << theTiles[j]->getTileNumber() << ") rotated "
						      << nrother << " times" << std::endl;
						curMatches++;
					}

					theTiles[j]->rotateTile();
					//DEBUG << "Other tile now rotated to :" << std::endl;
					//tileSet[j]->printTile();
				}
			}
		}

		numberOfOtherMatches.push_back(curMatches);
	}

	// The corners will have 4 matches.  One match for each side, another match for the mirror of those same sides
	for(int i = 0; i < theTiles.size(); i++)
	{
		if (numberOfOtherMatches[i] == 4)
		{
			// This is a corner
			theCornerList.push_back(theTiles[i]->getTileNumber());
		}
	}

}

std::vector<int> TileMap::getCornerList()
{
	return theCornerList;
}

int Tile::getNumActiveCells()
{
	int retVal = 0;
	for(int y = 0; y < theTileSize; y++)
	{
		for(int x = 0; x < theTileSize; x++)
		{
			if (getState(x,y))
			{
				retVal++;
			}
		}
	}

	return retVal;
}

int Tile::searchForSeaMonster()
{
	//                  #
	//#    ##    ##    ###
	// #  #  #  #  #  #
	int retVal = 0;

	static std::vector<std::pair<int, int> > seaMonsterOffsets;
	const int SEA_MONSTER_WIDTH = 20;
	const int SEA_MONSTER_HEIGHT = 3;

	static bool oneTimeInit = true;

	if(oneTimeInit)
	{
		std::vector<std::string> smdata;
		smdata.push_back("                  # ");
		smdata.push_back("#    ##    ##    ###");
		smdata.push_back(" #  #  #  #  #  #   ");

		for(int y = 0; y < smdata.size(); y++)
		{
			std::string curRow = smdata[y];
			for(int x = 0; x < curRow.size(); x++)
			{
				if (curRow[x] == '#')
				{
					seaMonsterOffsets.push_back({x,y});
				}
			}
		}

	}

	std::set<std::pair<int, int> > totalSeaMonsterHits;

	for(int y = 0; y < theTileSize - SEA_MONSTER_HEIGHT; y++)
	{
		for(int x = 0; x < theTileSize - SEA_MONSTER_WIDTH; x++)
		{
			// X,Y is where we are testing for a sea monster

			int smHits = 0;
			for(auto smOffset: seaMonsterOffsets)
			{
				int smx = x + smOffset.first;
				int smy = y + smOffset.second;
				if (getState(smx,smy))
				{
					smHits++;
				}
			}

			if (smHits == seaMonsterOffsets.size())
			{
				retVal++;

				for(auto smOffset: seaMonsterOffsets)
				{
					int smx = x + smOffset.first;
					int smy = y + smOffset.second;
					if (getState(smx,smy))
					{
						totalSeaMonsterHits.emplace(smx, smy);
					}
				}
			}
		}
	}

	if (totalSeaMonsterHits.size())
	{
		// Show pretty picture of the sea monsters!
		DEBUG << "WE HAVE SEA MONSTERS!!!!" << std::endl << std::endl;
		for(int y = 0; y < theTileSize; y++)
		{
			for(int x = 0; x < theTileSize; x++)
			{
				if (totalSeaMonsterHits.find({x,y}) != totalSeaMonsterHits.end())
				{
					DEBUG << "O";
				}
				else
				{
					if (getState(x,y))
					{
						DEBUG << "#";
					}
					else
					{
						DEBUG << ".";
					}
				}
			}

			DEBUG << std::endl;
		}
	}

	return totalSeaMonsterHits.size();

}


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
	for(int i = 0; i < theTileSize; i++)
	{
		if (getState(theTileSize-1, i) != rhs.getState(0,i) )
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

	theMatchingTiles.emplace(rhs.getTileNumber());
	return true;
}

bool Tile::matchBottomSide(Tile const & rhs)
{
	for(int i = 0; i < theTileSize; i++)
	{
		if (getState(i, theTileSize-1) != rhs.getState(i,0) )
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
	theTileSize = imageData[1].size();
	DEBUG << "TILE_SIZE = " << theTileSize << std::endl;
	
	assert(imageData.size() == theTileSize + 1);
	
	// Tiles are 10x10, with 1 row tile id header
	std::vector<char> delimiters = { ' ', ':' };
	std::vector<std::string> idParts = stringSplit(imageData[0], delimiters);
	assert(idParts.size() == 3);

	DEBUG << "HeaderParts=";
	debugPrintVector(idParts);
	DEBUG << std::endl;

	theTileNum = atoi(idParts[1].c_str());

	for(int i = 1; i <= theTileSize; i++)
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

	for(int y = 0; y < theTileSize; y++)
	{
		std::string singleRow;
		for(int x = 0; x < theTileSize; x++)
		{
			if ( (theNumRotations == 3) || (theNumRotations == 7) )
			{
				// Special rotation case, the mirror flip
				singleRow += theOldData[y][theTileSize-x-1];
			}
			else
			{
				// Rotations 90 deg clockwise
				singleRow += theOldData[theTileSize-x-1][y];
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

	TileMap tm(tileSet);
	tm.findCorners();

	std::vector<int> cornerList = tm.getCornerList();
	uint64_t cornerProduct = 1;
	for(auto ctn: cornerList)
	{
		DEBUG << "Corner = " << ctn << std::endl;
		cornerProduct *= (uint64_t) ctn;
	}
	

	std::cout << "Part 1 = " << cornerProduct << std::endl;	

	tm.placeCornerTopLeft();
	tm.placeRemainingTiles();

	tm.printEntireMap();

	std::vector<std::string> mtdata = tm.createMegaTile();

	DEBUG << "MEGA PREVIEW:" << std::endl;
	for(auto mtrow: mtdata)
	{
		DEBUG << mtrow << std::endl;
	}


	std::string megaLabel = "MEGATILE: 1:";
	mtdata.insert(mtdata.begin(), megaLabel);
	Tile mt(mtdata);
	mt.printTile();

	int numActiveCells = mt.getNumActiveCells();

	for(int rot = 0; rot < 8; rot++)
	{
		mt.rotateTile();
		int sm = mt.searchForSeaMonster();


		int stillActive = numActiveCells - sm;

		DEBUG << "sm on rot" << rot << " = " << sm << ", still active = " << stillActive << std::endl;

		if (sm)
		{
			std::cout << "Part 2: " << stillActive << std::endl;
		}

	}

	return 0;
}
