#ifndef BEAM_DETECTOR_H
#define BEAM_DETECTOR_H

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include "ElfComputer.h"

typedef std::pair<int, int> Coord;

class BeamDetector
{
public:

    BeamDetector(ElfCode programData);

    int readCoord(Coord c);

    int quantifyBeam(Coord topLeft, Coord botRight);

    void printData(Coord topLeft, Coord botRight);

    static std::string coordToString(Coord x);

    Coord firstSquareOfSize(int size);

    bool doesSquareFit(Coord bottomLeft, int size);

    bool isInBeam(Coord c);

private:

    std::map<Coord, int> theData;

    ElfCode theProgramData;
};



#endif // BEAM_DETECTOR_H
