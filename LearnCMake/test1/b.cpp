#include "b.h"
#include <sstream>

Bomber::Bomber(int num, std::string planeName):
	Airplane(num),
	name(planeName)
{
	std::cout << "Created a new bomber named " << name << std::endl;
}

void Bomber::DropBomb()
{
	std::cout << "Bomber " << name << " dropping bombs over baghdad" << std::endl;
}

std::string Bomber::GetIdentifier()
{
	std::ostringstream oss;
	oss << "Bomber " << name << " #" << tailNumber;
	return oss.str();
}
