#include "Scene.h"

#include <algorithm>    // std::find

#include "Graphics.h"
#include "Mixer.h"
#include "GameEntity.h"
#include "Logger.h"


Scene::Scene(Graphics* g, Mixer* m):
   theGraphics(g),
   theMixer(m),
   theName("Base"),
   theUpdateRateHz(60)
{
   theRenderer = g->GetRenderer();

   theWaitEventTimeoutFp = EventRecorder::GetInstance()->GetWaitEventFunction();
}

Scene::~Scene()
{
   LOG_DEBUG() << "End of scene" << theName;
}

void Scene::Update()
{
   LOG_DEBUG() << "Updating Scene, entity count = " << theEntities.size();
   for(auto e : theEntities)
   {
      // LOG_DEBUG() << "Updating entity (" << (unsigned long) *it << ")";
      e->Update();
   }
}

void Scene::Draw()
{
   for(auto e : theEntities)
   {
      e->Draw();
   }
}

void Scene::ManageEntityLifetimes()
{
   // LOG_DEBUG() << "Enter ManageEntityLifetimes";
   ProcessDelEntityQueue();

   ProcessAddEntityQueue();
   // LOG_DEBUG() << "Exit ManageEntityLifetimes";
}

void Scene::ProcessAddEntityQueue()
{
   while(!theAddQueue.empty())
   {
      std::pair<GameEntity*, int> & curEnt = theAddQueue.back();
      LOG_DEBUG() << __FUNCTION__ << ": Adding a new entity (" << (long) curEnt.first << ") to scene, code = " << curEnt.second;
      theEntities.push_back(curEnt.first);
      theAddQueue.pop_back();
   }
}

void Scene::ProcessDelEntityQueue()
{
   while(!theDelQueue.empty())
   {
      std::pair<GameEntity*, int> & curEnt = theAddQueue.back();
      LOG_DEBUG() << __FUNCTION__ << ": Removing an entity from scene, e=" << curEnt.first << " ,code = " << curEnt.second;

      auto it = std::find(theEntities.begin(), theEntities.end(), curEnt.first);
      if (it != theEntities.end())
      {
         LOG_DEBUG() << "Deleting entity" << (unsigned long) *it;
         delete (curEnt.first);
         theEntities.erase(it);
      }
      else
      {
         LOG_WARNING() << "Couldn't find entity to delete, already deleted?";
      }

      theDelQueue.pop_back();
   }
}


void Scene::SetUpdateRate(int updateHz)
{
   for(auto e : theEntities)
   {
      e->SetUpdateRate(updateHz);
   }

   theUpdateRateHz = updateHz;
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

      if (theWaitEventTimeoutFp(&ev, endOfFrameTicks - curTicks))
      {
         switch (ev.type)
         {
            case SDL_QUIT:
               return true;
            case SDL_JOYDEVICEADDED:
            case SDL_JOYDEVICEREMOVED:
               theGraphics->GetJoystick()->UpdateJoysticks();
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
    while(theWaitEventTimeoutFp(&ev, 0))
    {
        switch (ev.type)
        {
           case SDL_QUIT:
              return true;
           case SDL_JOYDEVICEADDED:
           case SDL_JOYDEVICEREMOVED:
              theGraphics->GetJoystick()->UpdateJoysticks();
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
   return nullptr;
}

std::string Scene::GetSceneName()
{
   return theName;
}

void Scene::AddEntity(GameEntity * e, int addCode)
{
   // LOG_DEBUG() << __FUNCTION__ << ": AddEntity: e=" << (long) e << ", code=" << addCode;
   theAddQueue.push_back(std::pair<GameEntity*, int>(e,addCode));
}

void Scene::DeleteEntity(GameEntity * e, int delCode)
{
   // LOG_DEBUG() << __FUNCTION__ << ": DeleteEntity: e=" << (long) e << ", code=" << delCode;
   theDelQueue.push_back(std::pair<GameEntity*, int>(e,delCode));
}
