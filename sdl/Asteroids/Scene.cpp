#include "Scene.h"

#include <algorithm>    // std::find

#include "Graphics.h"
#include "Mixer.h"
#include "GameEntity.h"
#include "Logger.h"



Scene::Scene(Graphics* g, Mixer* m):
   _graphics(g),
   _mixer(m),
   _name("Base"),
   _updateRateHz(60)
{
   _renderer = g->GetRenderer();
}

Scene::~Scene()
{
   LOG_DEBUG() << "End of scene" << _name;
}

void Scene::Update()
{
   // LOG_DEBUG() << "Updating Scene (" << (unsigned long) this << ")";

   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      // LOG_DEBUG() << "Updating entity (" << (unsigned long) *it << ")";
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

void Scene::ManageEntityLifetimes()
{
   std::vector<GameEntity*>::iterator it;

   while(!_deletionList.empty())
   {
      //it = _entities.find(_deletionList.back());
      it = std::find(_entities.begin(), _entities.end(), _deletionList.back());
      if (it != _entities.end())
      {
         LOG_DEBUG() << "Deleting entity" << (unsigned long) *it;
         delete (*it);
         _entities.erase(it);
      }
      else
      {
         LOG_WARNING() << "Couldn't find entity to delete, already deleted?";
      }

      _deletionList.pop_back();
   }

   while(!_additionList.empty())
   {
      LOG_DEBUG() << "Adding a new entity to scene";
      _entities.push_back(_additionList.back());
      _additionList.pop_back();
   }
}

void Scene::SetUpdateRate(int updateHz)
{
   for(std::vector<GameEntity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
   {
      (*it)->SetUpdateRate(updateHz);
   }

   _updateRateHz = updateHz;
}

void Scene::LoadBgMusic(char* filename)
{

}

void Scene::StartBgMusic()
{

}

bool Scene::PollInputs(int ticksToWait)
{
   if (ticksToWait == 0)
   {
       // Rendering is taking longer than normal, empty event queue before returning
       return PollInputsUntilEmpty();
   }

   int curTicks = SDL_GetTicks();
   int endOfFrameTicks = curTicks + ticksToWait;

   do
   {
      SDL_Event ev;
      bzero(&ev, sizeof(SDL_Event));

      if (SDL_WaitEventTimeout(&ev, endOfFrameTicks - curTicks))
      {
         switch (ev.type)
         {
            case SDL_QUIT:
               return true;
            case SDL_JOYDEVICEADDED:
            case SDL_JOYDEVICEREMOVED:
               _graphics->GetJoystick()->UpdateJoysticks();
               break;
            default:
            {
               bool abortPoll = ProcessEvent(ev);
               if (abortPoll)
               {
                  // Probably leaving the state early to transition states
                  return false;
               }
            }
         }
      }

      // See how much more time is left / process more events
      curTicks = SDL_GetTicks();

   } while (curTicks < endOfFrameTicks);

   return false;
}

bool Scene::PollInputsUntilEmpty()
{
    SDL_Event ev;
    bzero(&ev, sizeof(SDL_Event));
    while(SDL_WaitEventTimeout(&ev, 0))
    {
        switch (ev.type)
        {
           case SDL_QUIT:
              return true;
           case SDL_JOYDEVICEADDED:
           case SDL_JOYDEVICEREMOVED:
              _graphics->GetJoystick()->UpdateJoysticks();
              break;
           default:
           {
              bool abortPoll = ProcessEvent(ev);
              if (abortPoll)
              {
                 // Probably leaving the state early to transition states
                 return false;
              }
           }
        }
    }

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
