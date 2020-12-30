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

	int isWinner();

	uint64_t scoreGame();

	void playUntilWinner();

	void addGameToHistory();

	bool isGameRepeat();

	std::vector<int> theDeck1;
	std::vector<int> theDeck2;
	int theRound;

	std::vector<int> theDeck1Orig;
	std::vector<int> theDeck2Orig;

	std::set<std::pair<std::vector<int>, std::vector<int> > > theGameHistory;


	int theWinner;
};

void CardGame::addGameToHistory()
{
	std::pair<std::vector<int>, std::vector<int> > pairOfDecks;
	pairOfDecks.first = theDeck1;
	pairOfDecks.second = theDeck2;
	theGameHistory.emplace(pairOfDecks);
}

void CardGame::playUntilWinner()
{
	dump();

	while(true)
	{
		playRound();
		dump();

		if (theWinner)
		{
			DEBUG << "Player " << theWinner << " just won" << std::endl;
			break;
		}
	}

	dump();
}


bool CardGame::isGameRepeat()
{
	if (theGameHistory.find({theDeck1, theDeck2}) != theGameHistory.end())
	{
		DEBUG << "This current game is a repeat of previous game" << std::endl;
		return true;
	}

	return false;
}


void CardGame::playRound()
{
	if (isGameRepeat())
	{
		theWinner = 1;
		return;
	}

	addGameToHistory();

	assert(theDeck1.size() > 0);
	assert(theDeck2.size() > 0);

	int card1 = theDeck1[0];
	theDeck1.erase(theDeck1.begin());
	DEBUG << "Player 1 plays: " << card1 << std::endl;

	int card2 = theDeck2[0];
	theDeck2.erase(theDeck2.begin());
	DEBUG << "Player 2 plays: " << card2 << std::endl;

	int winner = 0;
	if ( (card1 > theDeck1.size()) || (card2 > theDeck2.size()) )
	{
		// Simple case, largest number wins
		if (card1 > card2)
		{
			DEBUG << "Player 1 wins a simple game" << std::endl;
			winner = 1;
		}
		else
		{
			DEBUG << "Player 2 wins a simple game" << std::endl;
			winner = 2;
		}
	}
	else
	{
		// Complicated case, play a sub game
		std::vector<int> subdeck1, subdeck2;
		for(int i = 0; i < card1; i++)
		{
			subdeck1.push_back(theDeck1[i]);
		}
		for(int i = 0; i < card2; i++)
		{
			subdeck2.push_back(theDeck2[i]);
		}

		CardGame sg(subdeck1, subdeck2);
		DEBUG << "Going to have to play a subgame to decide the winner" << std::endl;

		sg.playUntilWinner();
		winner = sg.isWinner();

		DEBUG << "Subgame complete!" << std::endl;
	}

	assert(winner != 0);

	if (winner == 1)
	{
		DEBUG << "Player 1 wins the round!" << std::endl;
		theDeck1.push_back(card1);
		theDeck1.push_back(card2);
	}
	else
	{
		DEBUG << "Player 2 wins the round!" << std::endl;
		theDeck2.push_back(card2);
		theDeck2.push_back(card1);
	}

	DEBUG << std::endl;
	theRound++;

	if (theDeck1.size() == 0)
	{
		DEBUG << "Player 1 out of cards, player 2 wins" << std::endl;
		theWinner = 2;
	}

	if (theDeck2.size() == 0)
	{
		DEBUG << "Player 2 out of cards, player 1 wins" << std::endl;
		theWinner = 1;
	}
}

int CardGame::isWinner()
{
	return theWinner;
}

uint64_t CardGame::scoreGame()
{
	uint64_t retVal = 0;
	uint64_t multiplier = 1;

	std::vector<int>* winningDeck;
	
	assert(theWinner != 0);

	if (theWinner == 2)
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
	theWinner = 0;

	theDeck1Orig = deck1;
	theDeck2Orig = deck2;
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
