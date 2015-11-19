#include "Scene.h"

#include "Graphics.h"
#include "Mixer.h"
#include "GameEntity.h"



Scene::Scene(Graphics* g, Mixer* m):
   _graphics(g),
   _mixer(m)
{
   _renderer = g->GetRenderer();
}

void Scene::Update()
{
   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      (*it)->Update();
   }
}

void Scene::Draw()
{
   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      (*it)->Draw();
   }
}

void Scene::SetUpdateRate(int updateHz)
{
   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      (*it)->SetUpdateRate(updateHz);
   }
}

void Scene::LoadBgMusic(char* filename)
{

}

void Scene::StartBgMusic()
{

}

void Scene::PollInputs()
{

}
