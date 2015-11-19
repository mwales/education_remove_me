#include "TitleScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"

TitleScene::TitleScene(Graphics* g, Mixer* m):
   Scene(g, m)
{
   _background.SetImageInfo("assets/nebula_blue.png", _renderer);
   _splashImage.SetImageInfo("assets/splash.png", _renderer);

   _background.SetFullscreen(true);

   XYPair splashCenter = g->GetWindowSize();
   splashCenter -= _splashImage.GetImageInfo()->GetSize();
   splashCenter *= 0.5;

   _splashImage.SetPosition(splashCenter);

   _entities.push_back(&_background);
   _entities.push_back(&_splashImage);


}
