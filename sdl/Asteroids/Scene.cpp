#include "Scene.h"

#include "Graphics.h"
#include "Mixer.h"
#include "GameEntity.h"
#include "Logger.h"



Scene::Scene(Graphics* g, Mixer* m):
   _graphics(g),
   _mixer(m),
   _name("Base")
{
   _renderer = g->GetRenderer();
}

Scene::~Scene()
{

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

bool Scene::PollInputs(int ticksToWait)
{
   int curTicks = SDL_GetTicks();
   int goalTicks = curTicks + ticksToWait;

   do
   {
      SDL_Event ev;
      SDL_WaitEventTimeout(&ev, goalTicks - curTicks);

      if (ev.type == SDL_QUIT)
      {
         return true;
      }
      else
      {
         bool abortPoll = ProcessEvent(ev);
         if (abortPoll)
         {
            // Probably leaving the state early to transition states
            return false;
         }
      }

      // See how much more time is left / process more events
      curTicks = SDL_GetTicks();

   } while (curTicks < goalTicks);

   return false;
}

bool Scene::ProcessEvent(SDL_Event const & ev)
{
   LOG_WARNING() << "Why the fuck did someone call the default ProcessEvent!";
   return false;
}

Scene* Scene::GetNextState(bool* deleteMe)
{
   LOG_WARNING() << "Why the fuck did someone call the default GetNextState!";
   *deleteMe = false;
   return NULL;
}

std::string Scene::GetSceneName()
{
   return _name;
}
