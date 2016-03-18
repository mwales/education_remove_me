#include "Mixer.h"
#include "Logger.h"

Mixer::Mixer():
  theBgMusic(nullptr)
{
   int i,max=Mix_GetNumMusicDecoders();
   for(i=0; i<max; ++i)
      LOG_DEBUG() << "Music decoder" << i << "is for" << Mix_GetMusicDecoder(i);

   if (MIX_INIT_OGG != Mix_Init(MIX_INIT_OGG))
   {
      LOG_WARNING() << "Mixer Init failed:" << Mix_GetError();
      return;
   }

   // CD Quality, 2 channels (stereo)
   if (0 != Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096*4))
   {
      LOG_WARNING() << "Error opening mixer audio:" << Mix_GetError();
   }
}


Mixer::~Mixer()
{
   std::map<std::string, Mix_Music*>::const_iterator musicIt;
   for (musicIt = theMusicCatalog.begin(); musicIt != theMusicCatalog.end(); musicIt++)
   {
      LOG_DEBUG() << "Deleting music file from memory:" << musicIt->first;
      Mix_FreeMusic(musicIt->second);
   }

   std::map<std::string, Mix_Chunk*>::const_iterator soundIt;
   for(soundIt = theSoundCatalog.begin(); soundIt != theSoundCatalog.end(); soundIt++)
   {
      LOG_DEBUG() << "Deleting sound file from memory:" << soundIt->first;
      Mix_FreeChunk(soundIt->second);
   }

   Mix_CloseAudio();
   Mix_Quit();

   LOG_DEBUG() << "Mixer closed";
}

void Mixer::LoadMusic(std::string const & filename)
{
   if (theBgMusic)
   {
      LOG_DEBUG() << "Changing BG music to:" << filename;

      if (1 == Mix_PlayingMusic())
      {
         // Stop music that is already playing
         PauseMusic();
      }
   }
   else
   {
      LOG_DEBUG() << "Loading BG music:" << filename;
   }

   std::map<std::string, Mix_Music*>::const_iterator cachedSong = theMusicCatalog.find(filename);
   if (cachedSong == theMusicCatalog.end())
   {
      // Song has not been loaded before
      theBgMusic = Mix_LoadMUS(filename.c_str());
      if (theBgMusic == nullptr)
      {
         LOG_WARNING() << "Error loading BG Music (" << filename << "): " << Mix_GetError();
      }
      else
      {
         theMusicCatalog[filename] = theBgMusic;
      }
   }
   else
   {
      // We already had this song in our cache songs!
      LOG_DEBUG() << "Loading song from cache:" << filename;
      theBgMusic = (cachedSong->second);
   }
}

void Mixer::PlayMusic(int numberOfPlays)
{
   if (!theBgMusic)
   {
      LOG_WARNING() << "Play music failed because no music was loaded";
      return;
   }

   if (0 != Mix_PlayMusic(theBgMusic, numberOfPlays))
   {
      LOG_WARNING() << "Error playing music:" << Mix_GetError();
   }

   if (numberOfPlays == -1)
   {
      LOG_DEBUG() << "Started BG Music (infinite) play(s))";
   }
   else
   {
      LOG_DEBUG() << "Started BG Music (" << numberOfPlays  << " play(s))";
   }
}

void Mixer::PauseMusic()
{
   if (!theBgMusic)
   {
      LOG_WARNING() << "Trying to pause music, but never loaded any";
      return;
   }

   Mix_PauseMusic();
}

void Mixer::ResumeMusic()
{
   if (!theBgMusic)
   {
      LOG_WARNING() << "Trying to resume music, but never loaded any";
      return;
   }

   Mix_ResumeMusic();
}

void Mixer::UncacheMusic(std::string const & filename)
{
   std::map<std::string, Mix_Music*>::iterator it = theMusicCatalog.find(filename);

   if (it == theMusicCatalog.end())
   {
      LOG_WARNING() << "Uncache music failed, filename not in cache:" << filename;
      return;
   }

   if (it->second == theBgMusic)
   {
      LOG_WARNING() << "Uncache music failed, music being played!" << filename;
      return;
   }

   Mix_Music* fileToDestroy = it->second;
   theMusicCatalog.erase(it);

   Mix_FreeMusic(fileToDestroy);
}

Mix_Chunk* Mixer::LoadSound(std::string const & filename)
{
   if (theSoundCatalog.find(filename) != theSoundCatalog.end())
   {
      LOG_DEBUG() << "Sound load found sound file in catalog:" << filename;
      return theSoundCatalog[filename];
   }

   Mix_Chunk* retVal = Mix_LoadWAV(filename.c_str());
   if (retVal == nullptr)
   {
      LOG_WARNING() << "Error loading sound file (" << filename << "):" << Mix_GetError();
      return retVal;
   }

   // Add sound to the cache if successfully loaded
   LOG_DEBUG() << "Sound" << filename << "successfully loaded and cataloged";
   theSoundCache.insert(retVal);
   theSoundCatalog[filename] = retVal;
   return retVal;
}

void Mixer::PlaySound(Mix_Chunk* soundEffect)
{
   if ( (soundEffect == nullptr) ||
        (theSoundCache.count(soundEffect) == 0))
   {
      LOG_WARNING() << "Sound effect pointer not found in cache";
      return;
   }

   if (Mix_PlayChannel(-1, soundEffect, 0) == -1)
   {
      LOG_WARNING() << "Error playing sound:" << Mix_GetError();
   }
}

void Mixer::UncacheSound(Mix_Chunk* soundEffect)
{
   if ( (soundEffect == nullptr) ||
        (theSoundCache.count(soundEffect) == 0))
   {
      LOG_WARNING() << "Sound effect pointer not found in cache";
      return;
   }

   theSoundCache.erase(soundEffect);

   for(auto&& curSound : theSoundCatalog)
   {
      if (curSound.second == soundEffect)
      {
         LOG_DEBUG() << "Removing sound (" << curSound.first << ") from sound catalog";
         theSoundCatalog.erase(curSound.first);

         Mix_FreeChunk(soundEffect);

         return;
      }
   }

   // We should never get here!
   LOG_FATAL() << "Found a cached sound pointer in the cache, but not in the catalog.";
}


