#include <iostream>
#include <cstdio>
#include <strings.h>

int getData(int x, int y, int* matrix, int size)
{
    return matrix[y * size + x];
}

// Increment by 1 for horizontal rows, increment by size for vertical cols
bool isDataLatin(int* data, int size, int increment)
{
    //std::cout << "isDataLatin(" << size << ") ";

    int* mirrorData = (int*) malloc(size * sizeof(int));
    
    // Initialize all mirror data to -1
    bzero(mirrorData, sizeof(size * sizeof(int)));
    
    for(int i = 0; i < size; i++)
    {
        int nextVal = data[i * increment];

        //std::cout << nextVal << " ";

        if (mirrorData[nextVal - 1] != 0)
        {
            //std::cout << " REPEAT!!!\n";
            return false;
        }

        if ( (nextVal <= 0) || (nextVal > size) )
        {
            //std::cout << "  INVALID!!!\n";
            return false;
        }
        
        mirrorData[nextVal - 1] = 1;
    }
    
    // std::cout << " Good\n";
    return true;
}


void solveProblem(int pNumber)
{
    int matrixSize;
    std::cin >> matrixSize;
    
    int* data = (int*) malloc(matrixSize * matrixSize * sizeof(int));
    
    //std::cout << "New Data: ";
    for(int i = 0; i < matrixSize * matrixSize; i++)
    {
        std::cin >> data[i];
        //std::cout << data[i] << " ";
    }

    //std::cout << std::endl;

    int traceVal = 0;
    for(int i = 0; i < matrixSize; i++)
    {
        traceVal += getData(i, i, data, matrixSize);
    }
    
    // Check rows
    int badRows = 0;
    for(int i = 0; i < matrixSize; i++)
    {
        if (!isDataLatin(data + i * matrixSize, matrixSize, 1))
        {
            badRows++;
        }
    }
    
    
    // Check cols
    int badCols = 0;
    for(int i = 0; i < matrixSize; i++)
    {
        if (!isDataLatin(data + i, matrixSize, matrixSize))
        {
            badCols++;
        }
    }
    
    printf("Case #%d: %d %d %d\n", pNumber, traceVal, badRows, badCols);
}

int main(int argc, char** argv)
{
    int numProblems;
    std::cin >> numProblems;
    
    for(int i = 0; i < numProblems; i++)
    {
        solveProblem(i+1);
    }
    
    return 0;
}
