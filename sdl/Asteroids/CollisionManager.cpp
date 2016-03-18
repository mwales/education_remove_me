#include "CollisionManager.h"
#include "Logger.h"
#include "GameMath.h"

#include <vector>
#include <math.h>
#include <algorithm>  // std::find

#define MORE_FLOATY_MATH_FOR_COMPARTMENTIZATION

CollisionManager::CollisionManager(int areaWidth, int areaHeight, int containerSize):
 theWidth(areaWidth),
 theHeight(areaHeight),
 theCompartmentSize(containerSize),
 theCurrentCollisionMode(CollisionMode::GRID)
{
    if ( (theWidth <= 0) || (theHeight <= 0))
    {
        LOG_FATAL() << "Collision manager area of" << areaWidth << "x" << areaHeight << " is invalid";
        return;
    }

    theCompartmentCols = ceil( (float) theWidth / (float) theCompartmentSize);
    theCompartmentRows = ceil( (float) theHeight / (float) theCompartmentSize);
}

CollisionManager::CollisionManager(int areaWidth, int areaHeight, int containerSize,
                 std::vector<ICollidable const *> const & bodiesA,
                 std::vector<ICollidable const *> const & bodiesB):
   theWidth(areaWidth),
   theHeight(areaHeight),
   theCompartmentSize(containerSize),
   theBodiesA(bodiesA),
   theBodiesB(bodiesB)
{
   if ( (theWidth <= 0) || (theHeight <= 0))
   {
       LOG_FATAL() << "Collision manager area of" << areaWidth << "x" << areaHeight << " is invalid";
       return;
   }

   theCompartmentCols = ceil(theWidth / theCompartmentSize);
   theCompartmentRows = ceil(theHeight / theCompartmentSize);
}

bool CollisionManager::RemoveFromA(ICollidable const * obj)
{
   auto it = std::find(theBodiesA.begin(), theBodiesA.end(), obj);

   if (it != theBodiesA.end())
   {
      LOG_DEBUG() << "Erasing object from bodies A list (" << (unsigned long) *it << ")";
      theBodiesA.erase(it);

      return true;
   }

   return false;
}

bool CollisionManager::RemoveFromB(ICollidable const * obj)
{
   if (obj == nullptr)
   {
      return false;
   }

   auto it = std::find(theBodiesB.begin(), theBodiesB.end(), obj);

   if (it != theBodiesB.end())
   {
      LOG_DEBUG() << "Erasing object from bodies B list";
      theBodiesB.erase(it);

      return true;
   }

   return false;
}

void CollisionManager::AddToA(ICollidable const * obj)
{
   theBodiesA.push_back(obj);
}

void CollisionManager::AddToB(ICollidable const * obj)
{
   theBodiesB.push_back(obj);
}

void CollisionManager::ClearCollisions()
{
   theCurrentCollisions.clear();
}

void CollisionManager::CheckForCollisions()
{
   // for debugging
   //DumpCollisionManager();

   switch(theCurrentCollisionMode)
   {
      case CollisionMode::EXPONENTIAL:
         CheckForCollisionsExponentialModern(&theBodiesA, &theBodiesB);
         break;
      case CollisionMode::GRID:
         CheckForCollisionsWithGrid();
   }
}

void CollisionManager::SetCollisionManagerMode(CollisionMode mode)
{
   theCurrentCollisionMode = mode;
}

void CollisionManager::CheckForCollisionsExponential()
{
   // For now, just do a simple comparison of comparing everything in A to B

   for(auto curA : theBodiesA)
   {
      for(auto curB : theBodiesB)
      {
         XYPair aPosition = curA->GetPosition();
         XYPair bPosition = curB->GetPosition();

         if (GameMath::Distance(aPosition, bPosition) <= 50)
         {
            // LOG_DEBUG() << "We have a collision.  A @ ("
            //             << curA->GetPosition()[0] << "," << curA->GetPosition()[1] << ") and B @ ("
            //             << curB->GetPosition()[0] << "," << curB->GetPosition()[1] << ")";

            Collision col(curA, curB);
            theCurrentCollisions.push_back(col);
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
   int numCompartmentsTotal = theCompartmentCols * theCompartmentRows;
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



#ifdef MORE_FLOATY_MATH_FOR_COMPARTMENTIZATION
void CollisionManager::GridHelper_PutIntoCompartments(std::vector<std::vector<ICollidable const *> >* gridA,
                                                      std::vector<std::vector<ICollidable const *> >* gridB)
{
   // LOG_DEBUG() << "GRID SIZE: " << _compartmentCols << " x " << _compartmentRows;

   for (ICollidable const * curObj : theBodiesA)
   {
      XYPair pos = curObj->GetPosition();
      int compX = pos[0] / theCompartmentSize;
      int compY = pos[1] / theCompartmentSize;
      int gridPos = compY * theCompartmentCols + compX;
      // LOG_DEBUG() << "Object going into compartment A" << gridPos << " with coord (" << pos[0]
      //             << "," << pos[1] << ")";
      (*gridA)[gridPos].push_back(curObj);
   }

   for (ICollidable const * curObj : theBodiesB)
   {
      XYPair pos = curObj->GetPosition();
      int compX = pos[0] / theCompartmentSize;
      int compY = pos[1] / theCompartmentSize;
      int gridPos = compY * theCompartmentCols + compX;
      // LOG_DEBUG() << "Object going into compartment B " << gridPos << " with coord (" << pos[0]
      //             << "," << pos[1] << ")";
      (*gridB)[gridPos].push_back(curObj);
   }
}
#endif

#ifdef LESS_FLOATY_MATH_FOR_COMPARTMENTIZATION
void CollisionManager::GridHelper_PutIntoCompartments(std::vector<std::vector<ICollidable const *> >* gridA,
                                                      std::vector<std::vector<ICollidable const *> >* gridB)
{
   // LOG_DEBUG() << "GRID SIZE: " << _compartmentCols << " x " << _compartmentRows;

   std::vector<ICollidable const *> bodiesACopy = _bodiesA;
   std::vector<ICollidable const *> bodiesBCopy = _bodiesB;
   int xCoord = 0;
   for (int xComp = 0; xComp < _compartmentCols; xComp++)
   {
      xCoord += _compartmentSize;
      int yCoord = 0;
      for (int yComp = 0; yComp < _compartmentRows; yComp++)
      {
         yCoord += _compartmentSize;
         int curCompartmentNum = yComp * _compartmentCols + xComp;

         // Loop through all the objects in the bodiesA list and determine which ones belong in this compartment.
         auto aIter = bodiesACopy.begin();
         while (aIter != bodiesACopy.end())
         {
            XYPair objPos = (*aIter)->GetPosition();
            if ( (objPos[0] < xCoord) && (objPos[1] < yCoord) )
            {
               // Belongs in this compartment
               // LOG_DEBUG() << "Found an object A compartment filler.  x=" << xCoord << ", y=" << yCoord << ", comp=" << curCompartmentNum << ", Obj pos = " << objPos;
               (*gridA)[curCompartmentNum].push_back(*aIter);
               aIter = bodiesACopy.erase(aIter);
            }
            else
            {
               aIter++;
            }
         }

         // Loop through all the objects in the bodiesB list and determine which ones belong in this compartment.
         auto bIter = bodiesBCopy.begin();
         while (bIter != bodiesBCopy.end())
         {
            XYPair objPos = (*bIter)->GetPosition();
            if ( (objPos[0] < xCoord) && (objPos[1] < yCoord) )
            {
               // Belongs in this compartment
               (*gridB)[curCompartmentNum].push_back(*bIter);
               bIter = bodiesBCopy.erase(bIter);
            }
            else
            {
               bIter++;
            }
         }
      }
   }
}
#endif

void CollisionManager::GridHelper_CollideCompartments(std::vector<std::vector<ICollidable const *> >* gridA,
                                                      std::vector<std::vector<ICollidable const *> >* gridB)
{
   for (int curRow = 0; curRow < theCompartmentRows; curRow++)
   {
      for (int curCol = 0; curCol < theCompartmentCols; curCol++)
      {
         // Collide everything in my current grid
         int gridPos = curRow * theCompartmentCols + curCol;
         // LOG_DEBUG() << "Checking " << curCol << "," << curRow << " for collisions, sizes"
         //             << (*gridA)[gridPos].size() << " and " << (*gridB)[gridPos].size();
         CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[gridPos]);

         // Collide everything in my grid with one to the right
         if (curCol + 1 < theCompartmentCols)
         {
            int altPos = gridPos + 1;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid with on below
         if (curRow + 1 < theCompartmentRows)
         {
            int altPos = gridPos + theCompartmentCols;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid with one to the right-below
         if ( (curRow + 1 < theCompartmentRows) && (curCol + 1 < theCompartmentCols) )
         {
            int altPos = gridPos + theCompartmentCols + 1;
            CheckForCollisionsExponentialModern( &(*gridA)[altPos] , &(*gridB)[gridPos]);
            CheckForCollisionsExponentialModern( &(*gridA)[gridPos] , &(*gridB)[altPos]);
         }

         // Collide everything in my grid wiht one to the left-below
         if ( (curRow + 1 < theCompartmentRows) && (curCol - 1 >= 0) )
         {
            int altPos = gridPos + theCompartmentCols - 1;
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
            theCurrentCollisions.push_back(kaboom);
            return true;
         }
      }
   }

   return false;
}

std::vector<Collision> CollisionManager::GetCollisions()
{
   return theCurrentCollisions;
}

void CollisionManager::DumpCollisionManager()
{
   LOG_DEBUG() << "A size=" << theBodiesA.size() << ", B size" << theBodiesB.size();
   for(auto curBodA : theBodiesA)
   {
      LOG_DEBUG() << "\tA> " << (long) curBodA; // << " @ " << curBodA->GetPosition();
   }
   for(auto curBodB : theBodiesB)
   {
      LOG_DEBUG() << "\tB> " << (long) curBodB; // << " @ " << curBodB->GetPosition();
   }
}
