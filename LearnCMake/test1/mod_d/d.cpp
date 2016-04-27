#include "d.h"

Destroyer::Destroyer(std::string shortName)
{
	name = "UDD (for a double dose of destruction) ";
	name += shortName;
}

std::string Destroyer::GetIdentification()
{
	return name;
}

