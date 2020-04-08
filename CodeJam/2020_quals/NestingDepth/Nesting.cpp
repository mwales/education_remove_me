#include <iostream>

void solveProblem(int probNumber)
{
    std::string input;
    std::cin >> input;

    int nestingLevel = 0;

    std::string outputData;

    for(int i = 0; i < input.length(); i++)
    {
        char smallBuf[2];
        smallBuf[0] = input[i];
        smallBuf[1] = 0;
        int curVal = atoi(smallBuf);

        while(curVal > nestingLevel)
        {
            outputData += "(";
            nestingLevel++;
        }

        while(curVal < nestingLevel)
        {
            outputData += ")";
            nestingLevel--;
        }

        outputData += smallBuf;
    }

    while(nestingLevel > 0)
    {
        outputData += ")";
        nestingLevel--;
    }

    std::cout << "Case #" << probNumber << ": ";

    std::cout << outputData;

    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    int numProbs;
    std::cin >> numProbs;

    for(int i = 0; i < numProbs; i++)
    {
        solveProblem(i+1);
    }


}
