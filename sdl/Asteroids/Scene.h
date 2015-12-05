#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Graphics;
class Mixer;
class GameEntity;
class Scene
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
    * Returns an instance of the next game scene.
    * @param[out] true if the scene object should be deleted
    * @return 0 if stay on the current scene, or the new scene.
    */
   virtual Scene* GetNextState(bool* deleteMe);

   std::string GetSceneName();

protected:

   virtual bool ProcessEvent(SDL_Event const & ev);

   SDL_Renderer* _renderer;

   std::vector<GameEntity*> _entities;

   Graphics* _graphics;

   Mixer* _mixer;

   std::string _name;

   int _updateRateHz;

   // After an update, these are the objects that should be deleted/added
   std::vector<GameEntity*> _deletionList;
   std::vector<GameEntity*> _additionList;
};

