#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	std::vector<int> data;

	while(true)
	{
		int singleNum;
		std::cin >> singleNum;


		if (std::cin.eof())
		{
			std::cout << "Done" << std::endl;
			break;
		}

		data.push_back(singleNum);
	}

	std::cout << "Num entries = " << data.size() << std::endl;

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
