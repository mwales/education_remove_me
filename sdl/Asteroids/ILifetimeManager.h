#pragma once

class GameEntity;

class ILifetimeManager
{

public:
   /**
    * Tells the scene to add a new entity to the scene (call update, manage it's lifetime)
    * @param gameEnt The new game entity
    * @param addCode Special code for use by particular scene, or not used at all
    */
   virtual void AddEntity(GameEntity * gameEnt, int addCode) = 0;

   /**
    * Tells the scene to delete an entity from the scene
    * @param gameEnt The game entity
    * @param addCode Special code for use by particular scene, or not used at all
    */
   virtual void DeleteEntity(GameEntity * gameEnt, int delCode) = 0;
};
