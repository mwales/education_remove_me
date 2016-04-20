#include "EventRecorder.h"

#include <SDL2/SDL.h>
#include <string>

#include "Logger.h"

EventRecorder* EventRecorder::theInstance = nullptr;

EventRecorder* EventRecorder::GetInstance()
{
   if (theInstance == nullptr)
   {
      LOG_FATAL() << "GetInstance called with no instance created yet";
      return nullptr;
   }
   else
   {
      return theInstance;
   }
}

void EventRecorder::DestroyInstance()
{
   if (theInstance != nullptr)
   {
      delete theInstance;
      theInstance = nullptr;
   }
}

EventRecorder* EventRecorder::CreateInstance()
{
   if (theInstance != nullptr)
   {
      LOG_FATAL() << "theInstance not null when we went to create it!";
      return nullptr;
   }

   theInstance = new EventRecorder();
   return theInstance;
}

void EventRecorder::ParseArguments(int argc, char** argv)
{
   bool grabNextParameter = false;
   for(int i = 1; i < argc; i++)
   {
      std::string param(argv[i]);
      if (grabNextParameter)
      {
         theFilename = param;
         grabNextParameter = false;
         continue;
      }
      if ( (param == "--playback") || (param == "--play") )
      {
         grabNextParameter = true;
         theState = RecorderState::PLAYBACK;
         continue;
      }
      if ( (param == "--record") || (param == "--rec") )
      {
         grabNextParameter = true;
         theState = RecorderState::RECORDING;
         continue;
      }
      if (param == "--speedup")
      {
         theSpeedupFlag = true;
      }

      LOG_WARNING() << "Unknown command line parameter: " << param;
   }

   if (theFilename == "")
   {
      // If no filename given, revert state back to IDLE
      theState = RecorderState::IDLE;
   }

   // Print confirmation to the user
   switch (theState)
   {
      case RecorderState::PLAYBACK:
         LOG_DEBUG() << "Event Playback from " << theFilename << " started.";
         theDataFile.open(theFilename.c_str(), std::fstream::in);
         break;
      case RecorderState::RECORDING:
         LOG_DEBUG() << "Events being recorded to file " << theFilename << ".";
         theDataFile.open(theFilename.c_str(), std::fstream::out | std::fstream::trunc);
      default:
         LOG_DEBUG() << "Event Recording is not enabled";
         break;
   }

   if ( (theState != RecorderState::IDLE) )
   {
      // verify file is open
      if (!theDataFile.is_open())
      {
          LOG_FATAL() << "Data file " << theFilename << " couldn't be opened";
          return;
      }

   }
}

void EventRecorder::SeedRandomNumberGenerator()
{
   srand(time(NULL));
}

EventRecorder::EventRecorder() :
   theState(RecorderState::IDLE),
   theSpeedupFlag(false)
{
   LOG_DEBUG() << "Event recorder started";
}

EventRecorder::~EventRecorder()
{
   LOG_DEBUG() << "Event Recording complete";
}

void EventRecorder::AdvanceFrame()
{
   LOG_DEBUG() << "Event recorder, advance frame";
}

void EventRecorder::StoreEvent(SDL_Event* ev)
{
   LOG_DEBUG() << "Store event";
}

WaitEventFuncType EventRecorder::GetWaitEventFunction()
{
   return EventRecorder::WaitEventTimeoutRecordEvent;
}

int EventRecorder::WaitEventTimeoutRecordEvent(SDL_Event* event, int timeout)
{
   LOG_DEBUG() << "WaitEventTimeoutRecordEvent event";
   return SDL_WaitEventTimeout(event, timeout);
}

int EventRecorder::WaitEventTimeoutReplayEvent(SDL_Event* event, int timeout)
{
   LOG_DEBUG() << "WaitEventTimeoutReplayEvent event";
   return SDL_WaitEventTimeout(event, timeout);
}
