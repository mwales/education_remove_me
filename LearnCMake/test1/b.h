#pragma once

#include<iostream>
#include "a.h"

class Bomber : public Airplane
{
public:

	Bomber(int num, std::string planeName);

	void DropBomb();

	std::string GetIdentifier();
protected:

	std::string name;
};

