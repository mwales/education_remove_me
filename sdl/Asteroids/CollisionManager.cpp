#include "CollisionManager.h"
#include "Logger.h"
#include "GameMath.h"

#include <math.h>
#include <algorithm>  // std::find

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

    _compartmentsPerRow = ceil(_width / _compartmentSize);
}

CollisionManager::CollisionManager(int areaWidth, int areaHeight, int containerSize,
                 std::vector<ICollidable const *> const & bodiesA,
                 std::vector<ICollidable const *> const & bodiesB):
   _width(areaWidth),
   _height(areaHeight),
   _compartmentSize(containerSize),
   _bodiesA(bodiesA),
   _bodiesB(bodiesB)
{

}

bool CollisionManager::RemoveFromA(ICollidable const * obj)
{
   std::vector<ICollidable const *>::iterator it;
   it = std::find(_bodiesA.begin(), _bodiesA.end(), obj);

   if (it != _bodiesA.end())
   {
      LOG_DEBUG() << "Erasing object from bodies A list";
      _bodiesA.erase(it);

      return true;
   }

   return false;
}

bool CollisionManager::RemoveFromB(ICollidable const * obj)
{
   std::vector<ICollidable const *>::iterator it;
   it = std::find(_bodiesB.begin(), _bodiesB.end(), obj);

   if (it != _bodiesB.end())
   {
      LOG_DEBUG() << "Erasing object from bodies B list";
      _bodiesB.erase(it);

      return true;
   }

   return false;
}

void CollisionManager::AddToA(ICollidable const * obj)
{
   _bodiesA.push_back(obj);
}

void CollisionManager::AddToB(ICollidable const * obj)
{
   _bodiesB.push_back(obj);
}

void CollisionManager::ClearCollisions()
{
   _currentCollisions.clear();
}

void CollisionManager::CheckForCollisions()
{
   // For now, just do a simple comparison of comparing everything in A to B
   std::vector<ICollidable const *>::const_iterator iterA;
   std::vector<ICollidable const *>::const_iterator iterB;

   for(iterA = _bodiesA.begin(); iterA != _bodiesA.end(); iterA++)
   {
      ICollidable const * curA = *iterA;

      for(iterB = _bodiesB.begin(); iterB != _bodiesB.end(); iterB++)
      {
         ICollidable const * curB = *iterB;

         XYPair aPosition = curA->GetPosition();
         XYPair bPosition = curB->GetPosition();

         if (GameMath::Distance(aPosition, bPosition) <= 50)
         {
            LOG_DEBUG() << "We have a collision";
         }
      }
   }
}

std::vector<Collision> CollisionManager::GetCollisions()
{
   return _currentCollisions;
}
