#pragma once

#include "Scene.h"
#include "GraphicEntity.h"

class TitleScene : public Scene
{
public:
   TitleScene(Graphics* g, Mixer* m);

protected:

   GraphicEntity _background;

   GraphicEntity _splashImage;
};

