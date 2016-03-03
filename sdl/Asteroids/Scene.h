#pragma once

#include "ILifetimeManager.h"
#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Graphics;
class Mixer;
class GameEntity;

/**
 * The scene is the main state machine for the game
 */
class Scene : public ILifetimeManager
{
public:
   Scene(Graphics* g, Mixer* m);

   virtual ~Scene();

   virtual void Update();

   virtual void Draw();

   virtual void ManageEntityLifetimes();

   virtual void SetUpdateRate(int updateHz);

   virtual void LoadBgMusic(char* filename);

   virtual void StartBgMusic();

   /**
    * Waits on the SDL event loop for events
    *
    * @param ticksToWait How long to wait for new events
    * @return True if the application should exit
    */
   virtual bool PollInputs(int ticksToWait);

   /**
    * This will empty the event queue completely.  This is called when the rendering taking a long time
    * and if we didn't, we would only process a single event per render, which causes a bunch of events
    * to queue up.
    * @return True if the application should exit
    */
   virtual bool PollInputsUntilEmpty();

   /**
    * Returns an instance of the next game scene.
    * @param[out] true if the scene object should be deleted
    * @return 0 if stay on the current scene, or the new scene.
    */
   virtual Scene* GetNextState(bool* deleteMe);

   std::string GetSceneName();

   void AddEntity(GameEntity * e) override;

   void DeleteEntity(GameEntity * e) override;

protected:

   virtual bool ProcessEvent(SDL_Event const & ev);

   SDL_Renderer* theRenderer;

   std::vector<GameEntity*> theEntities;

   Graphics* theGraphics;

   Mixer* theMixer;

   std::string theName;

   int theUpdateRateHz;

   // After an update, these are the objects that should be deleted/added
   std::vector<GameEntity *> theDeletionList;
   std::vector<GameEntity *> theAdditionList;
};

