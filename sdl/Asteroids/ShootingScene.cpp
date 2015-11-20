#include "ShootingScene.h"
#include "Graphics.h"
#include "Mixer.h"
#include "ImageInfo.h"
#include "Logger.h"

ShootingScene::ShootingScene(Graphics* g, Mixer* m):
   Scene(g, m)
{
   _name = "Shooting";
   _background.SetImageInfo("assets/nebula_brown.png", _renderer);

   _background.SetFullscreen(true);

   _entities.push_back(&_background);
}

bool ShootingScene::ProcessEvent(SDL_Event const & ev)
{
   return false;
}

Scene* ShootingScene::GetNextState(bool* deleteMe)
{
   *deleteMe = false;
   return NULL;
}
