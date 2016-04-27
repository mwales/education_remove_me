#pragma once

#include <iostream>

class Destroyer
{
public:
	Destroyer(std::string shortName);

	std::string GetIdentification();

protected:
	std::string name;
};
