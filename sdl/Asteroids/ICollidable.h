#pragma once

#include <vector>
#include <string>

#include "XYPair.h"

struct CollisionRect
{
    XYPair _topRight;

    XYPair _size;
};

class ICollidable
{
public:

    virtual std::vector<CollisionRect> GetCollisionBoxes() = 0;

    virtual XYPair GetLocation() = 0;

    virtual CollisionDetected(std::string collidingObject) = 0;

    virtual std::string GetName() = 0;
};
