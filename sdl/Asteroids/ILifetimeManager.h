#pragma once

class GameEntity;

class ILifetimeManager
{
   // Add an entity after update call
   virtual void AddEntity(GameEntity *) = 0;

   // Delete an entity after update call
   virtual void DeleteEntity(GameEntity *) = 0;
};
