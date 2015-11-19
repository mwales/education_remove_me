#pragma once

#include <SDL2/SDL.h>
#include <vector>

class Graphics;
class Mixer;
class GameEntity;

class Scene
{
public:
   Scene(Graphics* g, Mixer* m);

   virtual void Update();

   virtual void Draw();

   virtual void SetUpdateRate(int updateHz);

   virtual void LoadBgMusic(char* filename);

   virtual void StartBgMusic();


protected:

   virtual void PollInputs();

   SDL_Renderer* _renderer;

   std::vector<GameEntity*> _entities;

   Graphics* _graphics;

   Mixer* _mixer;
};

