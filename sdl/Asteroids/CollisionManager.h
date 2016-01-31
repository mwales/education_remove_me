#pragma once
#include <vector>
#include <utility>      // std::pair
#include "GraphicEntity.h"
#include "ICollidable.h"


typedef std::pair<ICollidable const *, ICollidable const *> Collision;

/**
 * The collision manager will manage checking for collisions between objects A and B.  A objects can pass by each
 * other, and B objects can pass by each other, but an A and B object will collide
 */
class CollisionManager
{
public:
    CollisionManager(int areaWidth, int areaHeight, int containerSize);

    CollisionManager(int areaWidth, int areaHeight, int containerSize,
                     std::vector<ICollidable const *> const & bodiesA,
                     std::vector<ICollidable const *> const & bodiesB);

    // These methods are for adding and removing objects from lists
    bool RemoveFromA(ICollidable const * obj);
    bool RemoveFromB(ICollidable const * obj);
    void AddToA(ICollidable const * obj);
    void AddToB(ICollidable const * obj);

    // These methods are typically called each time collisions are processed

    /**
     * Erases any collisions that would have been returned by GetCollisions()
     */
    void ClearCollisions();

    /**
     * Checks all objects in list A for collisions with objects in list B
     */
    void CheckForCollisions();

    /**
     * Returns pairs of objects that have collided
     */
    std::vector<Collision> GetCollisions();

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

    std::vector<ICollidable const *> _bodiesA;

    std::vector<ICollidable const *> _bodiesB;

    std::vector<Collision> _currentCollisions;



};
