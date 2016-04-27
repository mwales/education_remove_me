#include "a.h"
#include "mod_c/c.h"
#include "mod_d/d.h"
#include <iostream>
#include "b.h"

int main(int argc, char** argv)
{
	Airplane aDemo(2);
	aDemo.fly();

	Bomber heather(1337, "Leetness");
	heather.DropBomb();
	std::cout << heather.GetIdentifier() << " just dropped some major bombs" << std::endl;

	Carrier nem("Nemesis");
	std::cout << "Created a carrier called " << nem.GetIdentification() << std::endl;

	Destroyer d("Blaster Master");
	std::cout << "Created a destroyer called " << d.GetIdentification() << std::endl;

}

Airplane::Airplane(int x):
	tailNumber(x)
{
	std::cout << "Created an aircraft with the tail number " << tailNumber << std::endl;
}		

Airplane::~Airplane()
{
	std::cout << "Airplane " << tailNumber << " has landed for the last time" << std::endl;
}

void Airplane::fly()
{
	std::cout << "Flying around in " << tailNumber << std::endl;
}
