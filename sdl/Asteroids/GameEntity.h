#pragma once

#include <SDL2/SDL.h>

class GameEntity
{
public:
   GameEntity();

   virtual void SetUpdateRate(int updateHz);

   virtual void Update();

   virtual void Draw() { };

protected:

   int _updateRate;
};

