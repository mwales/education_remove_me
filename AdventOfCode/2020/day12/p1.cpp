#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <set>
#include <fstream>
#include <math.h>

#include "../customstuff.h"

#ifdef AOC_DEBUG
	#define DEBUG std::cout
#else
	#define DEBUG if(0) std::cout
#endif

// Rotation angle is always positve polar angle in degrees
void rotate2(int & wpx, int & wpy, int angle)
{
	// Somehow remembered that atan2 gives you the real angle of a coordinate
	double originalAng = atan2(wpy, wpx);

	// Add the angle modification to it...
	double modifiedAng = originalAng + (angle * 3.14159 / 180);

	// Find dist of point from 0,0
	double dist = pow(wpx * wpx + wpy * wpy, .5);

	// I was really worried about rounding errors after this calculation, hoping nearbyint
	// does the trick for me (seemed to work)
	wpx = nearbyint(cos(modifiedAng) * dist);
	wpy = nearbyint(sin(modifiedAng) * dist);

}

// Navigation function for part 2 featuring a waypoint that moves/rotates
int navigate2(std::vector<std::string> const & directions)
{
	int direction = 0;
	int x = 0;
	int y = 0;
	int wpx = 10;
	int wpy = 1;

	std::vector<std::pair<int, int> > theForwardOffset = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

	for(auto curIns: directions)
	{
		char command = curIns[0];
		int mag = atoi(curIns.c_str() + 1);

		DEBUG << "Command = " << command << ", mag = " << mag << std::endl;

		if (command == 'F')
		{
			DEBUG << "Forward" << std::endl;
			int offsetX = wpx * mag;
			int offsetY = wpy * mag;

			x += offsetX;
			y += offsetY;
		}

		if (command == 'L')
		{
			DEBUG << "Rotate Left" << std::endl;
			rotate2(wpx, wpy, mag);
		}

		if (command == 'R')
		{
			if (mag == 90)
			{
				rotate2(wpx, wpy, 270);
			}
			else if (mag == 270)
			{
				rotate2(wpx, wpy, 90);
			}
			else
			{
				// Angle should be 0 or 180, rotate direction doesn't matter
				rotate2(wpx, wpy, mag);
			}
		}

		if (command == 'N')
		{
			DEBUG << "North" << std::endl;
			wpy += mag;
		}

		if (command == 'S')
		{
			DEBUG << "South" << std::endl;
			wpy -= mag;
		}
		
		if (command == 'E')
		{
			DEBUG << "East" << std::endl;
			wpx += mag;
		}

		if (command == 'W')
		{
			DEBUG << "West" << std::endl;
			wpx -= mag;
		}

		DEBUG << "After moving, X=" << x << ", Y=" << y << ", WpX=" << wpx << ", WpY=" << wpy << std::endl;

	}

	return abs(x) + abs(y);

}

int navigate(std::vector<std::string> const & directions)
{
	int direction = 0;
	int x = 0;
	int y = 0;

	std::vector<std::pair<int, int> > theForwardOffset = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

	for(auto curIns: directions)
	{
		char command = curIns[0];
		int mag = atoi(curIns.c_str() + 1);

		DEBUG << "Command = " << command << ", mag = " << mag << std::endl;

		if (command == 'F')
		{
			DEBUG << "Forward" << std::endl;
			int offsetX = theForwardOffset[direction].first * mag;
			int offsetY = theForwardOffset[direction].second * mag;

			x += offsetX;
			y += offsetY;
		}

		if (command == 'L')
		{
			DEBUG << "Rotate Left" << std::endl;
			direction = (direction + mag / 90) % 4;
		}

		if (command == 'R')
		{
			DEBUG << "Rotate Right" << std::endl;
			direction = (direction - mag / 90 + 16) % 4;
		}

		if (command == 'N')
		{
			DEBUG << "North" << std::endl;
			y += mag;
		}

		if (command == 'S')
		{
			DEBUG << "South" << std::endl;
			y -= mag;
		}
		
		if (command == 'E')
		{
			DEBUG << "East" << std::endl;
			x += mag;
		}

		if (command == 'W')
		{
			DEBUG << "West" << std::endl;
			x -= mag;
		}

		DEBUG << "After moving, X=" << x << ", Y=" << y << ", Dir=" << direction << std::endl;

	}

	return abs(x) + abs(y);

}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

	std::ifstream infile(argv[1]);

	std::vector<std::string> directions;

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

		directions.push_back(text);
	}

	infile.close();

	int md = navigate(directions);
	std::cout << "manhattan distance = " << md << std::endl;

	std::cout << std::endl << std::endl;

	int md2 = navigate2(directions);
	std::cout << "manhattan distance 2 = " << md2 << std::endl;
	
	
	return 0;
}
