#include <iostream>
#include <vector>
#include "../customstuff.h"

int main(int argc, char** argv)
{

	if (argc < 2)
        {
		std::cerr << "Provide filename" << std::endl;
		return 0;
	}

        std::vector<std::string> fileData = readFile(argv[1]);

	std::vector<int> data;
	for(auto curLine: fileData)
	{
		data.push_back(atoi(curLine.c_str()));
	}

	DEBUG << "Num entries = " << data.size() << std::endl;

	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data.size(); j++)
		{
			if ( data[i] + data[j] == 2020 )
			{
				std::cout << "part1: " << data[i] << " x " << data[j] << " = "
				          << data[i] * data[j] << std::endl;
			}
		}
	}

	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data.size(); j++)
		{
			for(int k = 0; k < data.size(); k++)
			{
				if ( data[i] + data[j]  + data[k] == 2020 )
				{
					std::cout << "part2: " << data[i] << " x " << data[j] 
					          << " x " <<  data[k] << " = "
				        	  << data[i] * data[j] * data[k] << std::endl;
				}
			}
		}
	}

	return 0;
}
