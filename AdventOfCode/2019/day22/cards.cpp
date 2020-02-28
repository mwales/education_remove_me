#include <iostream>
#include <vector>
#include <stdint.h>

typedef std::vector<uint64_t> CardDeck;

void printDeck(CardDeck const & cards)
{
    bool first = true;
    for(auto it = cards.begin(); it != cards.end(); it++)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            std::cout << " ";
        }

        std::cout << *it;
    }

    std::cout << std::endl;
}

CardDeck dealDeck(CardDeck const & cards)
{
    CardDeck retVal;
    retVal.reserve(cards.size());

    for(auto it = cards.rbegin(); it != cards.rend(); it++)
    {
        retVal.push_back(*it);
    }

    return retVal;
}

CardDeck cutDeck(CardDeck const & cards, int cutPos)
{
    CardDeck retVal;
    retVal.reserve(cards.size());

    uint64_t numCardsProcessed = 0;

    while(numCardsProcessed < cards.size())
    {
        if (cutPos < 0)
        {
            cutPos += cards.size();
        }

        if (cutPos >= cards.size())
        {
            cutPos -= cards.size();
        }

        retVal.push_back(cards[cutPos]);

        cutPos++;
        numCardsProcessed++;
    }

    return retVal;
}

CardDeck dealWithIncrement(CardDeck const & cards, int incrementVal)
{
    CardDeck retVal(cards.size());

    uint64_t numCardsProcessed = 0;
    uint64_t pos = 0;

    while(numCardsProcessed < cards.size())
    {
        if (pos >= cards.size())
        {
            pos -= cards.size();
        }

        retVal[pos] = cards[numCardsProcessed];

        pos += incrementVal;
        numCardsProcessed++;
    }

    return retVal;
}

std::vector<std::string> splitString(std::string const & data)
{
    std::vector<std::string> retVal;
    std::string curString;

    for(auto it = data.begin(); it != data.end(); it++)
    {
        if (*it == ' ')
        {
            if (curString.size() > 0)
            {
                retVal.push_back(curString);
                curString = "";
            }

        }
        else
        {
            curString += *it;
        }
    }

    if (curString.size() > 0)
    {
        retVal.push_back(curString);
    }

    return retVal;
}

void printStrings(std::vector<std::string> const & data)
{
    for(auto it = data.begin(); it != data.end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << std::endl;
}

void processCommand(CardDeck & deck, std::string command)
{
    std::vector<std::string> tokens = splitString(command);

    if ( (tokens.size() == 4) && (tokens[0] == "deal") && (tokens[1] == "into") )
    {
        // deal into new deck command
        deck = dealDeck(deck);
    }

    if ( (tokens.size() == 2) && (tokens[0] == "cut") )
    {
        // cut
        int cutVal = atoi(tokens[1].c_str());
        deck = cutDeck(deck, cutVal);
    }

    if ( (tokens.size() == 4) && (tokens[0] == "deal") && (tokens[1] == "with") )
    {
        // deal with incrmeent
        int incVal = atoi(tokens[3].c_str());
        deck = dealWithIncrement(deck, incVal);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: cat instructions.txt | " << argv[0] << " numCard" << std::endl;
        return 1;
    }

    CardDeck deck;
    for(uint64_t i = 0; i < atoll(argv[1]); i++)
    {
        deck.push_back(i);
    }

    while(true)
    {
        std::string command;
        getline(std::cin, command);

        if (std::cin.eof())
        {
            break;
        }

        std::cout << "Rx:" << command << std::endl;

        // std::vector<std::string> tokens = splitString(command);

        // printStrings(tokens);

        processCommand(deck, command);

        // printDeck(deck);
    }

    if (deck.size() > 2019)
    {
	// printDeck(deck);
	for(int i = 0; i < deck.size(); i++)
	{
		if (deck[i] == 2019)
		{
			std::cout << "Card 2019 is at position " << i << std::endl;
		}
	}
    }
    else
    {
        printDeck(deck);
    }

    return 0;
}


