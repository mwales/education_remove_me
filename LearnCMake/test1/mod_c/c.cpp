#include "c.h"

Carrier::Carrier(std::string shortName)
{
	name = "USS ";
	name += shortName;
}

std::string Carrier::GetIdentification()
{
	return name;
}


