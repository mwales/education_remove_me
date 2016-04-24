#include "EventRecorder.h"

#include <SDL2/SDL.h>
#include <string>

#include "Logger.h"

EventRecorder* EventRecorder::theInstance = nullptr;

#define NOVERIFY_SEQ_NUMBERS

#ifdef VERIFY_SEQ_NUMBERS
   #define WRITE_SEQ_NUM(operation) theInstance->WriteSequenceNumber(operation)
   #define READ_SEQ_NUM(operation) theInstance->ReadVerifySequenceNumber(operation)
#else
   #define WRITE_SEQ_NUM(operation) do { } while (0)
   #define READ_SEQ_NUM(operation) do { } while (0)
#endif



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
         theDataFile.open(theFilename.c_str(), std::fstream::in | std::fstream::binary);
         break;
      case RecorderState::RECORDING:
         LOG_DEBUG() << "Events being recorded to file " << theFilename << ".";
         theDataFile.open(theFilename.c_str(), std::fstream::out | std::fstream::trunc | std::fstream::binary);
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
   unsigned int seed;
   switch(theState)
   {
   case RecorderState::IDLE:
      seed = time(NULL);
      break;
   case RecorderState::PLAYBACK:
      READ_SEQ_NUM("SeedRand");

      theDataFile.read( (char*) &seed, sizeof(seed));

       break;
   case RecorderState::RECORDING:
      seed = time(NULL);

      WRITE_SEQ_NUM("SeedRand");
      theDataFile.write( (char*) &seed, sizeof(seed));

   }

   srand(seed);
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

   if (theState != RecorderState::IDLE)
   {
      LOG_DEBUG() << "Closing the recording file: " << theFilename;
      theDataFile.close();
   }
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
   switch(theState)
   {
      case RecorderState::PLAYBACK:
         return WaitEventTimeoutReplayEvent;
      case RecorderState::RECORDING:
         return WaitEventTimeoutRecordEvent;
      default:
         return SDL_WaitEventTimeout;
   }
}

int EventRecorder::WaitEventTimeoutRecordEvent(SDL_Event* event, int timeout)
{
   LOG_DEBUG() << "WaitEventTimeoutRecordEvent event";

   int retVal = SDL_WaitEventTimeout(event, timeout);

   WRITE_SEQ_NUM("WaitEvent");
   theInstance->theDataFile.write( (char*) &retVal, sizeof(retVal));
   theInstance->theDataFile.write( (char*) event, sizeof(*event));

   LOG_DEBUG() << "WaitEventTimeoutRecordEvent returned " << EventTypeToString(event) << " and "
               << retVal;
   return retVal;
}

int EventRecorder::WaitEventTimeoutReplayEvent(SDL_Event* event, int timeout)
{
   LOG_DEBUG() << "WaitEventTimeoutReplayEvent event";
   READ_SEQ_NUM("WaitEvent");

   int retVal;
   theInstance->theDataFile.read( (char*) &retVal, sizeof(retVal));
   theInstance->theDataFile.read( (char*) event, sizeof(*event));

   LOG_DEBUG() << "WaitEventTimeoutReplayEvent returned " << EventTypeToString(event) << " and "
               << retVal;
   return retVal;
}


GetTicksFuncType EventRecorder::GetTicksFunction()
{

   switch(theState)
   {
      case RecorderState::PLAYBACK:
         return GetTicksAndReplay;
      case RecorderState::RECORDING:
         return GetTicksAndRecord;
      default:
         return SDL_GetTicks;
   }
}

uint32_t EventRecorder::GetTicksAndRecord()
{
   WRITE_SEQ_NUM("GetTicks");

   uint32_t retVal = SDL_GetTicks();
   theInstance->theDataFile.write( (char*) &retVal, sizeof(retVal));

   LOG_DEBUG() << "GetTicksAndRecord() returning " << retVal;
   return retVal;
}

uint32_t EventRecorder::GetTicksAndReplay()
{
   READ_SEQ_NUM("GetTicks");

   uint32_t retVal;

   theInstance->theDataFile.read( (char*) &retVal, sizeof(retVal));

   LOG_DEBUG() << "GetTicksAndReplay() returning " << retVal;

   if (!theInstance->theSpeedupFlag)
   {
      // Delay till the real ticks equals the ticks found in the file
      unsigned int currentRealTicks = SDL_GetTicks();

      while(retVal >= currentRealTicks)
      {
         // Short sleep, or regular sleep required
         const int SLEEP_INTERVAL_MS = 10;
         if ((retVal - currentRealTicks) > 10000)
         {
            SDL_Delay(SLEEP_INTERVAL_MS);
         }
         else
         {
            SDL_Delay((retVal - currentRealTicks) / 1000);
         }

         currentRealTicks = SDL_GetTicks();
      }
   }
   return retVal;
}

std::string EventRecorder::EventTypeToString(SDL_Event* ev)
{
   switch(ev->type)
   {
   case SDL_FIRSTEVENT:
      return "SDL_FIRSTEVENT";
   case SDL_QUIT:
      return "SDL_QUIT";
   case SDL_APP_TERMINATING:
      return "SDL_APP_TERMINATING";
   case SDL_APP_LOWMEMORY:
      return "SDL_APP_LOWMEMORY";
   case SDL_APP_WILLENTERBACKGROUND:
      return "SDL_APP_WILLENTERBACKGROUND";
   case SDL_APP_DIDENTERBACKGROUND:
      return "SDL_APP_DIDENTERBACKGROUND";
   case SDL_APP_WILLENTERFOREGROUND:
      return "SDL_APP_WILLENTERFOREGROUND";
   case SDL_APP_DIDENTERFOREGROUND:
      return "SDL_APP_DIDENTERFOREGROUND";
   case SDL_WINDOWEVENT:
      return "SDL_WINDOWEVENT";
   case SDL_SYSWMEVENT:
      return "SDL_SYSWMEVENT";
   case SDL_KEYDOWN:
      return "SDL_KEYDOWN";
   case SDL_KEYUP:
      return "SDL_KEYUP";
   case SDL_TEXTEDITING:
      return "SDL_TEXTEDITING";
   case SDL_TEXTINPUT:
      return "SDL_TEXTINPUT";
   case SDL_MOUSEMOTION:
      return "SDL_MOUSEMOTION";
   case SDL_MOUSEBUTTONDOWN:
      return "SDL_MOUSEBUTTONDOWN";
   case SDL_MOUSEBUTTONUP:
      return "SDL_MOUSEBUTTONUP";
   case SDL_MOUSEWHEEL:
      return "SDL_MOUSEWHEEL";
   case SDL_JOYAXISMOTION:
      return "SDL_JOYAXISMOTION";
   case SDL_JOYBALLMOTION:
      return "SDL_JOYBALLMOTION";
   case SDL_JOYHATMOTION:
      return "SDL_JOYHATMOTION";
   case SDL_JOYBUTTONDOWN:
      return "SDL_JOYBUTTONDOWN";
   case SDL_JOYBUTTONUP:
      return "SDL_JOYBUTTONUP";
   case SDL_JOYDEVICEADDED:
      return "SDL_JOYDEVICEADDED";
   case SDL_JOYDEVICEREMOVED:
      return "SDL_JOYDEVICEREMOVED";
   case SDL_CONTROLLERAXISMOTION:
      return "SDL_CONTROLLERAXISMOTION";
   case SDL_CONTROLLERBUTTONDOWN:
      return "SDL_CONTROLLERBUTTONDOWN";
   case SDL_CONTROLLERBUTTONUP:
      return "SDL_CONTROLLERBUTTONUP";
   case SDL_CONTROLLERDEVICEADDED:
      return "SDL_CONTROLLERDEVICEADDED";
   case SDL_CONTROLLERDEVICEREMOVED:
      return "SDL_CONTROLLERDEVICEREMOVED";
   case SDL_CONTROLLERDEVICEREMAPPED:
      return "SDL_CONTROLLERDEVICEREMAPPED";
   case SDL_FINGERDOWN:
      return "SDL_FINGERDOWN";
   case SDL_FINGERUP:
      return "SDL_FINGERUP";
   case SDL_FINGERMOTION:
      return "SDL_FINGERMOTION";
   case SDL_DOLLARGESTURE:
      return "SDL_DOLLARGESTURE";
   case SDL_DOLLARRECORD:
      return "SDL_DOLLARRECORD";
   case SDL_MULTIGESTURE:
      return "SDL_MULTIGESTURE";
   case SDL_CLIPBOARDUPDATE:
      return "SDL_CLIPBOARDUPDATE";
   case SDL_DROPFILE:
      return "SDL_DROPFILE";
   case SDL_RENDER_TARGETS_RESET:
      return "SDL_RENDER_TARGETS_RESET";
   case SDL_USEREVENT:
      return "SDL_USEREVENT";
   case SDL_LASTEVENT:
      return "SDL_LASTEVENT";
   default:
      return "*** UNKNOWN EVENT ***";
   }
}

void EventRecorder::ReadVerifySequenceNumber(std::string const & op)
{
   int verify;
   theDataFile.read( (char*) &verify, sizeof(verify));

   if (verify != theSeqNumber)
   {
       LOG_FATAL() << "Sequence number " << theSeqNumber << " invalid for " << op;
   }
   else
   {
      LOG_DEBUG() << "Sequence number " << theSeqNumber << " valid for " << op;
   }

   theSeqNumber++;
}

void EventRecorder::WriteSequenceNumber(std::string const & op)
{
   theDataFile.write( (char*) &theSeqNumber, sizeof(theSeqNumber));
   LOG_DEBUG() << "Sequence number " << theSeqNumber++ << " written for " << op;
}
