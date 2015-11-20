#pragma once

#include "Scene.h"
#include "GraphicEntity.h"

class ShootingScene : public Scene
{
public:
   ShootingScene(Graphics* g, Mixer* m);

   Scene* GetNextState(bool* deleteMe);

protected:

   bool ProcessEvent(SDL_Event const & ev);

protected:

   GraphicEntity _background;

};

