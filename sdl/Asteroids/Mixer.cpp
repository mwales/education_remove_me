#include "Mixer.h"
#include "Logger.h"

Mixer::Mixer():
  _bgMusic(nullptr)
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
   for (musicIt = _musicCatalog.begin(); musicIt != _musicCatalog.end(); musicIt++)
   {
      LOG_DEBUG() << "Deleting music file from memory:" << musicIt->first;
      Mix_FreeMusic(musicIt->second);
   }

   std::map<std::string, Mix_Chunk*>::const_iterator soundIt;
   for(soundIt = _soundCatalog.begin(); soundIt != _soundCatalog.end(); soundIt++)
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
   if (_bgMusic)
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

   std::map<std::string, Mix_Music*>::const_iterator cachedSong = _musicCatalog.find(filename);
   if (cachedSong == _musicCatalog.end())
   {
      // Song has not been loaded before
      _bgMusic = Mix_LoadMUS(filename.c_str());
      if (_bgMusic == nullptr)
      {
         LOG_WARNING() << "Error loading BG Music (" << filename << "): " << Mix_GetError();
      }
      else
      {
         _musicCatalog[filename] = _bgMusic;
      }
   }
   else
   {
      // We already had this song in our cache songs!
      LOG_DEBUG() << "Loading song from cache:" << filename;
      _bgMusic = (cachedSong->second);
   }
}

void Mixer::PlayMusic(int numberOfPlays)
{
   if (!_bgMusic)
   {
      LOG_WARNING() << "Play music failed because no music was loaded";
      return;
   }

   if (0 != Mix_PlayMusic(_bgMusic, numberOfPlays))
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
   if (!_bgMusic)
   {
      LOG_WARNING() << "Trying to pause music, but never loaded any";
      return;
   }

   Mix_PauseMusic();
}

void Mixer::ResumeMusic()
{
   if (!_bgMusic)
   {
      LOG_WARNING() << "Trying to resume music, but never loaded any";
      return;
   }

   Mix_ResumeMusic();
}

void Mixer::UncacheMusic(std::string const & filename)
{
   std::map<std::string, Mix_Music*>::iterator it = _musicCatalog.find(filename);

   if (it == _musicCatalog.end())
   {
      LOG_WARNING() << "Uncache music failed, filename not in cache:" << filename;
      return;
   }

   if (it->second == _bgMusic)
   {
      LOG_WARNING() << "Uncache music failed, music being played!" << filename;
      return;
   }

   Mix_Music* fileToDestroy = it->second;
   _musicCatalog.erase(it);

   Mix_FreeMusic(fileToDestroy);
}

Mix_Chunk* Mixer::LoadSound(std::string const & filename)
{
   if (_soundCatalog.find(filename) != _soundCatalog.end())
   {
      LOG_DEBUG() << "Sound load found sound file in catalog:" << filename;
      return _soundCatalog[filename];
   }

   Mix_Chunk* retVal = Mix_LoadWAV(filename.c_str());
   if (retVal == nullptr)
   {
      LOG_WARNING() << "Error loading sound file (" << filename << "):" << Mix_GetError();
      return retVal;
   }

   // Add sound to the cache if successfully loaded
   LOG_DEBUG() << "Sound" << filename << "successfully loaded and cataloged";
   _soundCache.insert(retVal);
   _soundCatalog[filename] = retVal;
   return retVal;
}

void Mixer::PlaySound(Mix_Chunk* soundEffect)
{
   if ( (soundEffect == nullptr) ||
        (_soundCache.count(soundEffect) == 0))
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
        (_soundCache.count(soundEffect) == 0))
   {
      LOG_WARNING() << "Sound effect pointer not found in cache";
      return;
   }

   _soundCache.erase(soundEffect);

   std::map<std::string, Mix_Chunk*>::iterator it;
   for(it = _soundCatalog.begin(); it != _soundCatalog.end(); it++)
   {
      if (it->second == soundEffect)
      {
         LOG_DEBUG() << "Removing sound (" << it->first << ") from sound catalog";
         _soundCatalog.erase(it);

         Mix_FreeChunk(soundEffect);

         return;
      }
   }

   // We should never get here!
   LOG_FATAL() << "Found a cached sound pointer in the cache, but not in the catalog.";
}


