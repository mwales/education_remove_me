#pragma once

#include <vector>
#include <string>

#include "XYPair.h"
#include <SDL2/SDL.h>

struct CollisionRect
{
    XYPair _topLeft;

    XYPair _size;
};

class ICollidable
{
public:

    virtual std::vector<SDL_Rect> GetCollisionBoxes() const = 0;

    virtual XYPair GetPosition() const = 0;

    virtual ~ICollidable() {};

    //virtual bool CollisionDetected(std::string collidingObject) const = 0;

    //virtual std::string GetName() const = 0;
};
