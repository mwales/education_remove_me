#pragma once

#include <SDL2/SDL.h>

/**
 * All game objects that are drawn / updated each frame should derive from GameEntity
 */
class GameEntity
{
public:
   GameEntity();

   virtual ~GameEntity() { };

   virtual void SetUpdateRate(int updateHz);

   virtual void Update();

   virtual void Draw() { };

protected:

   int theUpdateRateHz;
};

