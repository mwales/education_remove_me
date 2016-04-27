#pragma once

#include <iostream>

class Carrier
{
public:
	Carrier(std::string shortName);

	std::string GetIdentification();
protected:
	std::string name;
};
