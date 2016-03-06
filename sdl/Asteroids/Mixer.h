#pragma once

#include <map>
#include <set>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Mixer
{
public:
   Mixer();

   ~Mixer();

   /// ************************************************************
   /// Music functions (only 1 BG music can be played at a time)
   /// ************************************************************

   void LoadMusic(std::string const & filename);

   void PlayMusic(int numberOfPlays=-1);

   void PauseMusic();

   void ResumeMusic();

   // Removes song from memory, it will have to be reloaded from file to play again if necessary
   void UncacheMusic(std::string const & filename);

   /// ************************************************************
   /// Sound functions
   /// ************************************************************

   Mix_Chunk* LoadSound(std::string const & filename);

   void PlaySound(Mix_Chunk* soundEffect);

   void UncacheSound(Mix_Chunk* soundEffect);

protected:

   Mix_Music* theBgMusic;

   // Don't unload music, just cache it in RAM for use later
   std::map<std::string, Mix_Music* > theMusicCatalog;

   std::set<Mix_Chunk*> theSoundCache;

   std::map<std::string, Mix_Chunk*> theSoundCatalog;
};

