#include <iostream>

bool isValidPassword(std::string pass)
{
	char curPassLetter = '0';
	bool isFirstLetter = true;
	bool adjacentPairLetters = false;

	for(auto letterIt = pass.begin(); letterIt != pass.end(); letterIt++)
	{
		if (*letterIt < curPassLetter)
		{
			return false;
		}

		if (isFirstLetter)
		{
			isFirstLetter = false;
		}
		else
		{
			// Compare this letter and last letter for sameness
			if (curPassLetter == *letterIt)
			{
				adjacentPairLetters = true;
			}
		}

		curPassLetter = *letterIt;
	}

	return adjacentPairLetters;
}

int letterCount(std::string word, char letterToCount)
{
	int count;
	for(auto letterIt = word.begin(); letterIt != word.end(); letterIt++)
	{
		if (*letterIt == letterToCount)
		{
			count++;
		}
	}

	return count;
}

bool isBetterValidPassword(std::string pass)
{
	char curPassLetter = '0';
	bool isFirstLetter = true;
	bool adjacentPairLetters = false;

	for(auto letterIt = pass.begin(); letterIt != pass.end(); letterIt++)
	{
		if (*letterIt < curPassLetter)
		{
			return false;
		}

		if (isFirstLetter)
		{
			isFirstLetter = false;
		}
		else
		{
			// Compare this letter and last letter for sameness
			if (curPassLetter == *letterIt)
			{
				if (letterCount(pass, curPassLetter) == 2)
				{
					adjacentPairLetters = true;
				}
			}
		}

		curPassLetter = *letterIt;
	}

	return adjacentPairLetters;
}


int main(int, char**)
{
	int numValidPasswords = 0;
	int numValidBetterPasswords = 0;

	std::string beginPassword;
	std::string endPassword;

	std::cout << "Enter the beginning number" << std::endl;
	std::cin >> beginPassword;

	std::cout << "Enter the end password" << std::endl;
	std::cin >> endPassword;

	int beginNumber = atoi(beginPassword.c_str());
	int endNumber = atoi(endPassword.c_str());

	for(int i = beginNumber; i <= endNumber; i++)
	{
		std::string passString = std::to_string(i);

		if (isValidPassword(passString))
		{
			std::cout << "Valid: " << passString << std::endl;
			numValidPasswords++;
		}

		if (isBetterValidPassword(passString))
		{
			std::cout << "Better too: " << passString << std::endl;
			numValidBetterPasswords++;
		}
	}

	std::cout << "Num valid passwords = " << numValidPasswords << std::endl;
	std::cout << "Num valid better passwords = " << numValidBetterPasswords << std::endl;

	return 0;
}
