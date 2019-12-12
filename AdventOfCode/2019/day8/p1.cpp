#include <iostream>
#include <vector>

bool getLayer(int w, int h, int layer, std::string const & data, std::string* retVal)
{
	int layerSize = w * h;

	if ((layerSize * layer) >= data.size())
	{
		return false;
	}

	*retVal = data.substr(layerSize * layer, layerSize);
	return true;
}

int letterCount(std::string haystack, char needle)
{
	int retVal = 0;
	for(auto cIt = haystack.begin(); cIt != haystack.end(); cIt++)
	{
		if (*cIt == needle)
		{
			retVal++;
		}
	}

	return retVal;
}

void printImage(int w, int h, std::string const & data)
{
	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			char curLetter = data[y*w+x];
			if (curLetter == '1')
			{
				std::cout << "#";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}


int main(int, char**)
{
	int width, height; 
	std::string imgdata;
	
	std::cin >> width;
	std::cin >> height;	
	std::cin >> imgdata;
	std::vector<std::string> layerData;

	int fewest0Digits = 99999999;
	int layer = 0;
	int solution = 0;
	while(true)
	{

		std::string curLayer;
	       	bool success= getLayer(width, height, layer, imgdata, &curLayer);
		if (!success)
		{
			break;
		}

		// For pt2
		layerData.push_back(curLayer);

		int numZeros = letterCount(curLayer, '0');
		int numOnes = letterCount(curLayer, '1');
		int numTwos = letterCount(curLayer, '2');

		std::cout << "Layer " << layer << ") " << curLayer << std::endl;
		std::cout << "0's=" << numZeros << "   1's=" << numOnes << "   2's=" << numTwos << std::endl;
	
		if (numZeros < fewest0Digits)
		{
			std::cout << "Best row so far, numZeros previously = " << fewest0Digits << std::endl;
			solution = numOnes * numTwos;
			fewest0Digits = numZeros;
		}
		else
		{
			std::cout << "Not the best row" << std::endl;
		}
		
		layer++;
	}

	std::cout << "pt 1 solution " << solution << std::endl;

	int numLayers = layerData.size();
	int numPixelsPerLayer = width * height;
	std::string pt2Layer;
	for(int i = 0; i < numPixelsPerLayer; i++)
	{
		for(auto it = layerData.begin(); it != layerData.end(); it++)
		{
			if (it->at(i) != '2')
			{
				pt2Layer += it->at(i);
				break;
			}
		}
	}

	printImage(width, height, pt2Layer);

	return 0;
}
