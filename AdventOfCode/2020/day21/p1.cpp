#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <algorithm>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

typedef struct FoodItemStruct
{
	std::vector<std::string> theIngredients;

	std::vector<std::string> theAllergens;
} FoodItem;

std::ostream &operator<<(std::ostream& os, FoodItem const & fi)
{
	os << "{ Ingred:" << fi.theIngredients << " Allergens" << fi.theAllergens << " }";
	return os;
}

typedef std::map<std::string, std::set<std::string> > AllergenSource;

class AllergyDatabase
{
public:

	AllergyDatabase();
	
	void addFoodItem(FoodItem fi);

	bool analData();

	void dumpData();

	bool hasKnownAllergen(std::string ingredient);

	// Allergens, and suspected sources.  Key = allergy.  Values = possible ingredients
	std::map<std::string, std::set<std::string> > theSuspects;

	// Allergen and known source.  Key = ingredient, value = allergen
	std::map<std::string, std::string> theSources;



};

void AllergyDatabase::dumpData()
{
	DEBUG << "********** Allergy Database **********" << std::endl;
	DEBUG << " Known Allergens" << std::endl;
	for(auto it: theSources)
	{
		DEBUG << "    " << it.first << " causes allergy " << it.second << std::endl;
	}

	DEBUG << " Unknown Allergens" << std::endl;
	for(auto it: theSuspects)
	{
		DEBUG << "    " << it.first << " is in " << it.second << std::endl;
	}
}


AllergyDatabase::AllergyDatabase()
{
}

void AllergyDatabase::addFoodItem(FoodItem fi)
{
	for(auto allergen: fi.theAllergens)
	{
		if (theSuspects.find(allergen) == theSuspects.end())
		{
			// No data on this allergen
			DEBUG << "  allergen(" << allergen << ") has no data before" << std::endl;
			std::set<std::string> ingreds;
			for(auto curIngred: fi.theIngredients)
			{
				ingreds.emplace(curIngred);
			}
			theSuspects[allergen] = ingreds;
		}
		else
		{
			// We already have allergen in our system!
			DEBUG << "  allergen(" << allergen << ") has data from before: " << theSuspects[allergen] << std::endl;
			// Any ingredient not on both lists is eliminated as a source
			std::vector<std::string> ingredsToRemove;
			for(auto curSuspectIngred: theSuspects[allergen])
			{
				if (std::find(fi.theIngredients.begin(), fi.theIngredients.end(), curSuspectIngred) == fi.theIngredients.end())
				{
					// Suspect ingred was not found in this recipe, can't be a suspect
					DEBUG << "  Previously suspected ingredient " << curSuspectIngred << " not found in new food" << std::endl;
					ingredsToRemove.push_back(curSuspectIngred);
				}
				else
				{
					DEBUG << "  Previously suspected ingredient " << curSuspectIngred << " found in new food too!" << std::endl;
				}
			}

			// Now remove the ingreds ruled out!
			for(auto clearedIngred: ingredsToRemove)
			{
				DEBUG << "Clearing ingred(" << clearedIngred << ") as a cause of allergen " << allergen << std::endl;
				theSuspects[allergen].erase(clearedIngred);
			}
		}
	}

}

bool AllergyDatabase::hasKnownAllergen(std::string ingredient)
{
	return theSources.find(ingredient) != theSources.end();
}

bool AllergyDatabase::analData()
{
	DEBUG << __FUNCTION__ << std::endl;
	bool retVal = false;
	
	std::vector<std::string> noLongerSus;

	for(auto & curSus: theSuspects)
	{
		// Do any suspects just have 1 ingredient now?
		if (curSus.second.size() == 1)
		{
			// Allergen has only 1 known ingredient, no longer just sus
			DEBUG << "Allergen " << curSus.first << " only has 1 suspected ingredient now" << std::endl;

			theSources[*curSus.second.begin()] = curSus.first;
			retVal = true;
			noLongerSus.push_back(curSus.first);
		}

		// Any of the suspects have ingredients with already known allergies?
		for(auto singleIngred: curSus.second)
		{
			if (hasKnownAllergen(singleIngred))
			{
				DEBUG << singleIngred << " on list for " << curSus.first << ", but already know to cause "
				      << theSources[singleIngred] << std::endl;
				curSus.second.erase(singleIngred);
				retVal = true;

				// Exit this loop for now
				break;
			}
		}
	}


	// Now do the cleanup
	for(auto curSusDiscard: noLongerSus)
	{
		theSuspects.erase(curSusDiscard);
	}

	DEBUG << "After anal, my dump looks like:" << std::endl;
	dumpData();

	return retVal;
}









int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);

	std::vector<std::string> inputData;

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
		inputData.push_back(text);
	}

	infile.close();
	std::vector<char> delimList = { ' ', '(', ')', ',' };;
	
	std::vector<FoodItem> foodList;	
	for(auto singleStr: inputData)
	{

		std::vector<std::string> stringParts = stringSplit(singleStr, delimList);

		DEBUG << "Len = " << stringParts.size() << ": ";
		debugPrintVector(stringParts);
		DEBUG << std::endl;

		FoodItem fi;
		bool listingAllergens = false;
		for(auto comp: stringParts)
		{
			if (comp == "contains")
			{
				listingAllergens = true;
				continue;
			}

			if (listingAllergens)
			{
				fi.theAllergens.push_back(comp);
			}
			else
			{
				fi.theIngredients.push_back(comp);
			}
		}

		DEBUG << "Food Item: " << fi << std::endl;
		foodList.push_back(fi);
	}

	DEBUG << "List of Food: " << foodList << std::endl;

	AllergyDatabase db;

	std::vector<AllergenSource> allSource;	
	{
		for(auto fi: foodList)
		{
			DEBUG << "Adding " << fi << " to the database" << std::endl;

			db.addFoodItem(fi);
			db.dumpData();

			DEBUG << std::endl;
		}
	}

	while(db.analData());

	DEBUG << "Done analyzing the data" << std::endl;

	db.dumpData();

	int goodIngredCount = 0;
	for(auto const & fi: foodList)
	{
		for(auto const & ingred: fi.theIngredients)
		{
			if (!db.hasKnownAllergen(ingred))
			{
				DEBUG << ingred << "is always safe to eat" << std::endl;
				goodIngredCount++;
			}
		}
	}

	std::cout << "Pt 1: " << goodIngredCount << std::endl;

	std::map<std::string,std::string> allergenOrder = reverseMap(db.theSources);

	std::cout << "Pt 2: ";
	bool first = true;
	for(auto it: allergenOrder)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			std::cout << ",";
		}

		std::cout << it.second;
	}

	std::cout << std::endl;
	return 0;
}
