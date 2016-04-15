#include "EventRecorder.h"

#include <SDL2/SDL.h>

#include "Logger.h"

EventRecorder* EventRecorder::theInstance = nullptr;

EventRecorder* EventRecorder::GetInstance()
{
   if (theInstance == nullptr)
   {
      return new EventRecorder();
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

EventRecorder::EventRecorder()
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
