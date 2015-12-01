#pragma once

#include "Scene.h"
#include "GraphicEntity.h"

class PauseScene : public Scene
{
public:
   PauseScene(Graphics* g, Mixer* m, Scene* underneath);

   ~PauseScene();

   Scene* GetNextState(bool* deleteMe);

   virtual void Draw();

protected:

   bool ProcessEvent(SDL_Event const & ev);

   Scene* _underneath;

   GraphicEntity _pauseText;

   Scene* _nextScene;
};
