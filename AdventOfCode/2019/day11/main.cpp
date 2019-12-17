#include <iostream>
#include <map>

#include "ElfComputer.h"

void updateDirection(int & dir, int outputData)
{
	if (outputData == 0)
	{
		// Turn left
		dir += 90;
	}
	else
	{
		dir -= 90;
	}

	if (dir < 0)
	{
		dir += 360;
	}

	if (dir >= 360)
	{
		dir -= 360;
	}

}

typedef std::pair<int, int> Coordinate;

void updateCoordinate(Coordinate& c, int const & dir)
{
	DEBUGOUT << "updateCoordinate( (" << c.first << "," << c.second << "), " << dir << ")" << std::endl;
	switch(dir)
	{
		case 0:
			c.first += 1;
			return;
		case 90:
			c.second += 1;
			return;

		case 180:
			c.first -= 1;
			return;

		case 270:
			c.second -= 1;
			return;

		default:
			std::cerr << "Invalid direction: " << dir << std::endl;
			exit(1);
	}
}

void printMap(std::map<Coordinate, int> const & journey, Coordinate brush, int direction)
{
	int minX = brush.first;
	int minY = brush.second;
	int maxX = brush.first;
	int maxY = brush.second;

	for(auto it = journey.begin(); it != journey.end(); it++)
	{
		// std::cout << it->first.first << "," << it->first.second << " = " << it->second << std::endl;
		
		if (it->first.first < minX)
		{
			minX = it->first.first;
		}

		if (it->first.first > maxX)
		{
			maxX = it->first.first;
		}

		if (it->first.second < minY)
		{
			minY = it->first.second;
		}

		if (it->first.second > maxY)
		{
			maxY = it->first.second;
		}
	}

	std::cout << "Start printing" << std::endl;

	for(int y = maxY; y >= minY; y--)
	{
		for(int x = minX; x <= maxX; x++)
		{
			Coordinate cur;
			cur.first = x;
			cur.second = y;

			if ( (x == brush.first) && (y == brush.second) )
			{
				// Draw the player
				switch(direction)
				{
					case 0:
						std::cout << ">";
						continue;

					case 90:
						std::cout << "^";
						continue;

					case 180:
						std::cout << "<";
						continue;

					case 270:
						std::cout << "V";
						continue;
				}
			}
			
			if (journey.count(cur) > 0)
			{
				if (journey.at(cur) == 0)
				{
					std::cout << " ";
				}
				else
				{
					std::cout << "#";
				}
			}
			else
			{
				// Not in the map make a blank
				std::cout << " ";
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void runProgram(ElfCode pd, int part)
{
	ElfCode inputQ;
	ElfCode outputQ;

	ElfComputer emu(pd, "painter");
	emu.setInputDataQ(&inputQ);
	emu.setOutputDataQ(&outputQ);

	int direction = 90;

	// Color scheme.  0 = black, 1 = white
	std::map<Coordinate, int> journey;
	
	// Is this part 2?
	if (part == 2)
	{
		journey[std::make_pair<int,int>(0,0)] = 1;
	}
	
	int outputStep = 0; // 0 = waiting for paint / no paint, 1 = waiting for left / right
	Coordinate coord = std::make_pair<int, int>(0,0);
	while(!emu.isHalted())
	{
		// Update the input value incase it is read
		inputQ.clear();
		if (journey.count(coord) == 0)
		{
			inputQ.push_back(0);
		}
		else
		{
			inputQ.push_back(journey.at(coord));
		}

		emu.runIteration();

		// Did the emulator output anything?
		if (outputQ.size() > 0)
		{
			int outVal = outputQ.at(0);
			outputQ.clear();

			if (outputStep == 0)
			{
				// Waiting for paint output
				DEBUGOUT << "Painting (" << coord.first << "," << coord.second << ") = "
					<< outVal << std::endl;;
				journey[coord] = outVal;
				outputStep = 1;
			}
			else
			{
				// Move output
				DEBUGOUT  << "From coord " << coord.first << "," << coord.second << "), moving "
					<< ( outVal ? "right" : "left") << " with dir " << direction << std::endl;
				updateDirection(direction, outVal);
				updateCoordinate(coord, direction);
				DEBUGOUT << "To coord " << coord.first << "," << coord.second << "), moving "
					<< ( outVal ? "right" : "left") << " with dir " << direction << std::endl;
				
				
				outputStep = 0;

				// printMap(journey, coord, direction);
			}
		}
	}

	
	printMap(journey, coord, direction);

	int numPainted = 0;
	for(auto singlePoint = journey.begin(); singlePoint != journey.end(); singlePoint++)
	{
		if (singlePoint->second == 1)
		{
			numPainted++;
		}
	}

	std::cout << "Num painted = " << numPainted << std::endl;
	std::cout << "Num painted atleast once " << journey.size() << std::endl;		

	return;
}




int main(int, char**)
{
	std::string programDataString;
	std::cin >> programDataString;

	ElfCode pd = strListToInt(split(programDataString, ","));

	runProgram(pd, 1);

	runProgram(pd, 2);

	return 0;
}


