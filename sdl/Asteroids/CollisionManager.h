#pragma once
#include<vector>
#include"GraphicEntity.h"

class CollisionManager
{
public:
    CollisionManager(int areaWidth, int areaHeight, int containerSize);

protected:

    int _width;

    int _height;

    // Size of compartment to divide area into
    int _compartmentSize;

    /**
     * Precalculate the number of compartments in a row (helpful in case the _width isn't
     * divisible by _compartmentSize
     */
    int _compartmentsPerRow;


};
