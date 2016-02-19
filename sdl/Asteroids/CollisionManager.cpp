#include "CollisionManager.h"
#include "Logger.h"
#include "GameMath.h"

#include <vector>
#include <math.h>
#include <algorithm>  // std::find

CollisionManager::CollisionManager(int areaWidth, int areaHeight, int containerSize):
 _width(areaWidth),
 _height(areaHeight),
 _compartmentSize(containerSize),
 _currentCollisionMode(CollisionMode::GRID)
{
    if ( (_width <= 0) || (_height <= 0))
    {
        LOG_FATAL() << "Collision manager area of" << areaWidth << "x" << areaHeight << " is invalid";
        return;
    }

    _compartmentCols = ceil(_width / _compartmentSize);
    _compartmentRows = ceil(_height / _compartmentSize);
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
   if ( (_width <= 0) || (_height <= 0))
   {
       LOG_FATAL() << "Collision manager area of" << areaWidth << "x" << areaHeight << " is invalid";
       return;
   }

   _compartmentCols = ceil(_width / _compartmentSize);
   _compartmentRows = ceil(_height / _compartmentSize);
}

bool CollisionManager::RemoveFromA(ICollidable const * obj)
{
   std::vector<ICollidable const *>::iterator it;
   it = std::find(_bodiesA.begin(), _bodiesA.end(), obj);

   if (it != _bodiesA.end())
   {
      LOG_DEBUG() << "Erasing object from bodies A list (" << (unsigned long) *it << ")";
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
   switch(_currentCollisionMode)
   {
      case CollisionMode::EXPONENTIAL:
         CheckForCollisionsExponentialModern(&_bodiesA, &_bodiesB);
         break;
      case CollisionMode::GRID:
         CheckForCollisionsWithGrid();
   }
}

void CollisionManager::SetCollisionManagerMode(CollisionMode mode)
{
   _currentCollisionMode = mode;
}

void CollisionManager::CheckForCollisionsExponential()
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
//            LOG_DEBUG() << "We have a collision.  A @ ("
//                        << curA->GetPosition()[0] << "," << curA->GetPosition()[1] << ") and B @ ("
//                        << curB->GetPosition()[0] << "," << curB->GetPosition()[1] << ")";

            Collision col(curA, curB);
            _currentCollisions.push_back(col);
         }
      }
   }
}

void CollisionManager::CheckForCollisionsExponentialModern(std::vector<ICollidable const *>* listA,
                                                           std::vector<ICollidable const *>* listB)
{

   for(auto curA : *listA)
   {
      for(auto curB : *listB)
      {
         DoObjectsOverlap(curA, curB);
      }
   }
}

void CollisionManager::CheckForCollisionsWithGrid()
{
   // empty until i write stuff

   // Put the objects into compartments
   int numCompartmentsTotal = _compartmentCols * _compartmentRows;
   std::vector< std::vector<ICollidable const *> > aGrid(numCompartmentsTotal);
   std::vector< std::vector<ICollidable const *> > bGrid(numCompartmentsTotal);

   for(int i = 0; i < numCompartmentsTotal; i++)
   {
      aGrid.push_back(std::vector<ICollidable const *>());
      bGrid.push_back(std::vector<ICollidable const *>());
   }

   GridHelper_PutIntoCompartments(&aGrid, &bGrid);
   GridHelper_CollideCompartments(&aGrid, &bGrid);
}

void CollisionManager::GridHelper_PutIntoCompartments(std::vector<std::vector<ICollidable const *> >* gridA,
                                                      std::vector<std::vector<ICollidable const *> >* gridB)
{
   //LOG_DEBUG() << "GRID SIZE: " << _compartmentCols << " x " << _compartmentRows;

   for (ICollidable const * curObj : _bodiesA)
   {
      XYPair pos = curObj->GetPosition();
      int compX = pos[0] / _compartmentSize;
      int compY = pos[1] / _compartmentSize;
      int gridPos = compY * _compartmentCols + compX;
//      LOG_DEBUG() << "Object going into compartment A" << gridPos << " with coord (" << pos[0]
//                  << "," << pos[1] << ")";
      (*gridA)[gridPos].push_back(curObj);
   }

   for (ICollidable const * curObj : _bodiesB)
   {
      XYPair pos = curObj->GetPosition();
      int compX = pos[0] / _compartmentSize;
      int compY = pos[1] / _compartmentSize;
      int gridPos = compY * _compartmentCols + compX;
//      LOG_DEBUG() << "Object going into compartment B " << gridPos << " with coord (" << pos[0]
//                  << "," << pos[1] << ")";
      (*gridB)[gridPos].push_back(curObj);
   }
}

void CollisionManager::GridHelper_CollideCompartments(std::vector<std::vector<ICollidable const *> >* gridA,
                                                      std::vector<std::vector<ICollidable const *> >* gridB)
{
   for (int curRow = 0; curRow < _compartmentRows; curRow++)
   {
      for (int curCol = 0; curCol < _compartmentCols; curCol++)
      {
         // Collide everything in my current grid
         int gridPos = curRow * _compartmentCols + curCol;
//         LOG_DEBUG() << "Checking " << curCol << "," << curRow << " for collisions, sizes"
//                     << (*gridA)[gridPos].size() << " and " << (*gridB)[gridPos].size();
         CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[gridPos]);

         // Collide everything in my grid with one to the right
         if (curCol + 1 < _compartmentCols)
         {
            int altPos = gridPos + 1;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid with on below
         if (curRow + 1 < _compartmentRows)
         {
            int altPos = gridPos + _compartmentCols;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid wiht one to the right-below
         if ( (curRow + 1 < _compartmentRows) && (curCol + 1 < _compartmentCols) )
         {
            int altPos = gridPos + _compartmentCols + 1;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid wiht one to the left-below
         if ( (curRow + 1 < _compartmentRows) && (curCol - 1 >= 0) )
         {
            int altPos = gridPos + _compartmentCols - 1;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }
      }
   }
}

bool CollisionManager::DoObjectsOverlap(ICollidable const * objA, ICollidable const * objB)
{
   for (auto boxA : objA->GetCollisionBoxes())
   {
      for (auto boxB : objB->GetCollisionBoxes())
      {
         if (SDL_HasIntersection(&boxA, &boxB) == SDL_TRUE)
         {
            Collision kaboom { objA, objB };
            _currentCollisions.push_back(kaboom);
            return true;
         }
      }
   }

   return false;
}

std::vector<Collision> CollisionManager::GetCollisions()
{
   return _currentCollisions;
}
