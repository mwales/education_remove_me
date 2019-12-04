#include <iostream>

int simpleFuel(int mass)
{
	return mass / 3 - 2;
}

int totalFuel(int mass)
{
	int sumFuel = 0;
	int fuelIter = mass;
	do
	{
		fuelIter = simpleFuel(fuelIter);
		if (fuelIter > 0)
		{
			sumFuel += fuelIter;
		}
	} while (fuelIter > 0);

	return sumFuel;
}



int main(int argc, char** argv)
{
	int simpleSum = 0;
	int totalSum = 0;

	while(true)
	{
		int mass;
		int sfuel, tfuel;
		std::cin >> mass;


		if (std::cin.eof())
		{
			break;
		}

		sfuel = simpleFuel(mass);
		tfuel = totalFuel(mass);

		std::cout << "Simple Fuel of " << mass << " = " << sfuel << std::endl;	
		std::cout << "Total Fuel of " << mass << " = " << tfuel << std::endl;

		simpleSum += sfuel;
		totalSum += tfuel;
	}

	std::cout << std::endl;
	std::cout << "Full Sum = " << simpleSum << std::endl;
	std::cout << "Total Fuel = " << totalSum << std::endl;

	return 0;
}
