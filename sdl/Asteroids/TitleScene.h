#pragma once

#include "Scene.h"
#include "GraphicEntity.h"

class TitleScene : public Scene
{
public:
   TitleScene(Graphics* g, Mixer* m);

   ~TitleScene();

   Scene* GetNextState(bool* deleteMe);

protected:

   bool ProcessEvent(SDL_Event const & ev);

   GraphicEntity _background;

   GraphicEntity _splashImage;

   GraphicEntity _authorName;

   Scene* _nextScene;
};

