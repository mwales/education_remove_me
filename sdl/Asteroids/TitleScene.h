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

   GraphicEntity theBackground;

   GraphicEntity theSplashImage;

   GraphicEntity theAuthorName;

   Scene* theNextScene;
};

