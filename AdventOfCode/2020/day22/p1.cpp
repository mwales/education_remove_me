#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>
#include <assert.h>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

class CardGame
{
public:
	CardGame(std::vector<int> deck1, std::vector<int> deck2);

	void dump();

	void playRound();

	bool isWinner();

	uint64_t scoreGame();

	void addCurrentGameToHistory();

	bool isCurrentGameRepeat();

	std::vector<int> theDeck1;
	std::vector<int> theDeck2;
	int theRound;

};

void CardGame::playRound()
{
	assert(theDeck1.size() > 0);
	assert(theDeck2.size() > 0);

	int p1Card = theDeck1[0];
	int p2Card = theDeck2[0];

	theDeck1.erase(theDeck1.begin());
	theDeck2.erase(theDeck2.begin());

	DEBUG << "Player 1 plays: " << p1Card << std::endl;
	DEBUG << "Player 2 plays: " << p2Card << std::endl;

	if (p1Card > p2Card)
	{
		DEBUG << "Player 1 wins the round!" << std::endl;
		theDeck1.push_back(p1Card);
		theDeck1.push_back(p2Card);
	}
	else
	{
		DEBUG << "Player 2 wins the round!" << std::endl;
		theDeck2.push_back(p2Card);
		theDeck2.push_back(p1Card);
	}

	DEBUG << std::endl;
	theRound++;
}

bool CardGame::isWinner()
{
	if (theDeck1.size() == 0)
	{
		DEBUG << "Player 2 wins the game" << std::endl;
		return true;
	}

	if (theDeck2.size() == 0)
	{
		DEBUG << "Player 1 wins the game" << std::endl;
		return true;
	}

	return false;
}

uint64_t CardGame::scoreGame()
{
	uint64_t retVal = 0;
	uint64_t multiplier = 1;

	std::vector<int>* winningDeck;
	
	assert(isWinner());

	if (theDeck1.size() == 0)
	{
		winningDeck = &theDeck2;
	}
	else
	{
		winningDeck = &theDeck1;
	}

	for(int i = winningDeck->size() - 1; i >= 0; i--)
	{
		retVal += (multiplier * winningDeck->at(i));
		multiplier++;
	}

	return retVal;
}


CardGame::CardGame(std::vector<int> deck1, std::vector<int> deck2)
{
	theRound = 0;
	theDeck1 = deck1;
	theDeck2 = deck2;
}

void CardGame::dump()
{
	DEBUG << "-- Round " << theRound << " --" << std::endl;
	DEBUG << "Player 1's deck: " << theDeck1 << std::endl;
	DEBUG << "Player 2's deck: " << theDeck2 << std::endl;
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::vector<std::string> fileData = readFile(argv[1]);

	std::vector<int> player1;
	std::vector<int> player2;

	assert(fileData[0] == "Player 1:");

	int curPos; 
	for(curPos = 1; curPos < fileData.size(); curPos++)
	{
		if (fileData[curPos] == "")
		{
			break;
		}

		player1.push_back(atoi(fileData[curPos].c_str()));
	}

	// Skip the player2 banner
	curPos++;
	assert(fileData[curPos] == "Player 2:");
	curPos++;

	for(; curPos < fileData.size(); curPos++)
	{
		player2.push_back(atoi(fileData[curPos].c_str()));
	}

	CardGame cg(player1, player2);
	cg.dump();
	while(true)
	{
		cg.playRound();
		cg.dump();

		if (cg.isWinner())
		{
			break;
		}
	}

	std::cout << "Score:" << cg.scoreGame() << std::endl;
	
	return 0;
}
