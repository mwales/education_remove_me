#include "CollisionManager.h"
#include "Logger.h"

CollisionManager::CollisionManager(int areaWidth, int areaHeight, int containerSize):
 _width(areaWidth),
 _height(areaHeight),
 _compartmentSize(containerSize)
{
    if ( (_width <= 0) || (_height <= 0))
    {
        LOG_FATAL() << "Collision manager area of" << areaWidth << "x" << areaHeight << " is invalid";
        return;
    }

    _compartmentsPerRow = _width / _compartmentSize;
}
