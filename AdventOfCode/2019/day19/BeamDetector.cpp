#include "BeamDetector.h"
#include "ElfComputer.h"
#include <stdlib.h>

// #define BEAM_DETECTOR_DEBUG
#ifdef BEAM_DETECTOR_DEBUG
    #define BDD std::cout
#else
    #define BDD if(0) std::cout
#endif

BeamDetector::BeamDetector(ElfCode pd):
    theProgramData(pd)
{
    BDD << "Beam Detector started" << std::endl;
}


int BeamDetector::readCoord(Coord c)
{
    // BDD << "readCoord( " << coordToString(c) << " ) called" << std::endl;

    std::vector<int64_t> inputQ;
    std::vector<int64_t> outputQ;

    ElfComputer emu(theProgramData, "emu");
    emu.setInputDataQ(&inputQ);
    emu.setOutputDataQ(&outputQ);

    // Feed coordinate into the emulator
    inputQ.push_back(c.first);
    inputQ.push_back(c.second);

    int numIterations = 0;
    while(!emu.isHalted() && outputQ.empty())
    {
        emu.runIteration();
        numIterations++;
        // BDD << "****** Ran a single iteration ******" << std::endl;
    }

    if (outputQ.empty())
    {
        std::cout << "Error, emulator halted, but no data in output Q after "
                  << numIterations << " iterations" << std::endl;
        exit(1);
    }

    int retVal = outputQ.at(0);

    BDD << "readCoord(" << coordToString(c) << ") = " << retVal << " in " << numIterations << " iterations" << std::endl;

    theData[c] = retVal;
    return retVal;
}

int BeamDetector::quantifyBeam(Coord topLeft, Coord botRight)
{
    int retVal = 0;

    for(int y = topLeft.second; y <= botRight.second; y++)
    {
        for(int x = topLeft.first; x <= botRight.first; x++)
        {
            if (readCoord(std::make_pair(x,y)))
            {
                retVal++;
            }
        }
    }

    return retVal;
}

void BeamDetector::printData(Coord topLeft, Coord botRight)
{
    for(int y = topLeft.second; y <= botRight.second; y++)
    {
        for(int x = topLeft.first; x <= botRight.first; x++)
        {
            Coord cur = std::make_pair(x,y);

            if (theData.find(cur) != theData.end())
            {
                int val = theData[cur];
                if (val)
                {
                    std::cout << "#";
                }
                else
                {
                    std::cout << ".";
                }
            }
            else
            {
                std::cout << "?";
            }

        }

        std::cout << std::endl;
    }
}

std::string BeamDetector::coordToString(Coord x)
{
    std::string retVal = "(";
    retVal += std::to_string(x.first);
    retVal += ",";
    retVal += std::to_string(x.second);
    retVal += ")";
    return retVal;
}

bool BeamDetector::doesSquareFit(Coord bottomLeft, int size)
{
    // Check all 4 corners of square and verify that they have beam set
    Coord topLeft = bottomLeft;
    topLeft.second -= size;

    Coord topRight = topLeft;
    topRight.first += size;

    Coord bottomRight = bottomLeft;
    bottomRight.first += size;

    BDD << "doesSquareFit(size=" << size << ", with coords " << coordToString(bottomLeft) << " "
        << coordToString(topLeft) << " " << coordToString(topRight) << " "
        << coordToString(bottomRight) << ")" << std::endl;

    if ( isInBeam(bottomLeft) && isInBeam(bottomRight) &&
         isInBeam(topLeft) && isInBeam(topRight) )
    {
        BDD << " FOUND!" << std::endl;
        return true;
    }

    BDD << "NOT YET..." << std::endl;
    return false;
}

bool BeamDetector::isInBeam(Coord c)
{
    // Read new values!
    if (theData.find(c) == theData.end())
    {
        readCoord(c);
    }

    BDD << "isInBeam(" << coordToString(c) << ") = " << (theData[c] == 1 ? "yes" : "no") << std::endl;
    return theData[c] == 1;
}

Coord BeamDetector::firstSquareOfSize(int size)
{
    // Given what we know of beam, start at row 10
    size -= 1;    // A difference of 1 is a square of size 2!

    // Find the left edge of the beam
    Coord leftEdge;
    leftEdge.first = 0;
    leftEdge.second = 10;

    while(true)
    {
        leftEdge.first += 1;
        BDD << "Searching for initial left edge @ " << coordToString(leftEdge) << std::endl;
        if (isInBeam(leftEdge))
        {
            break;
        }
    }

    // Now keep working down the left edge of the beam till we find the area we are looking for
    while(true)
    {
        leftEdge.second += 1;

        BDD << "Beam search starting at " << coordToString(leftEdge) << std::endl;

        while(true)
        {
            if (isInBeam(leftEdge))
            {
                // We found the the left edge for this row!
                if (doesSquareFit(leftEdge, size))
                {
                    return leftEdge;
                }
                else
                {
                    // No square at this point
                    break;
                }
            }

            // Move right and try to find the first point in the beam
            leftEdge.first++;
        }
    }
}
