#include "Joystick.h"

#include <ostream>

#include "Command.h"
#include "Logger.h"

Joystick::Joystick():
   theJoystick(nullptr),
   theDeadzone(25)
{
   // SDL not initialized yet
}

std::string Joystick::JoystickInfoString(int index)
{
   std::ostringstream oss;
   oss << "Joystick[" << SDL_JoystickNameForIndex(index) << "]";
   return oss.str();
}

std::string Joystick::JoystickInfoString(SDL_Joystick* js)
{
   if (js == nullptr)
   {
      return "Joystick[NULL]";
   }

   std::ostringstream oss;
   oss << "Joystick[" << SDL_JoystickName(js) << ", Axes=" << SDL_JoystickNumAxes(js)
       << ", Balls=" << SDL_JoystickNumBalls(js) << ", Buttons=" << SDL_JoystickNumButtons(js)
       << ", Hats=" << SDL_JoystickNumHats(js) << "]";
   return oss.str();
}

Joystick::~Joystick()
{
   CloseAllJoysticks();
}

void Joystick::CloseAllJoysticks()
{
   if (theJoystick)
   {
      LOG_DEBUG() << "Closing joystick";
      SDL_JoystickClose(theJoystick);
      theJoystick = nullptr;
   }
}

void Joystick::AddJoystick()
{
   int numJoysticks = SDL_NumJoysticks();
   if (numJoysticks < 0)
   {
      LOG_WARNING() << "Error determing the number of joysticks:" << SDL_GetError();
   }
   else if (numJoysticks == 0)
   {
      LOG_DEBUG() << "No joysticks were found";
   }
   else
   {
      LOG_DEBUG() << "Found" << numJoysticks << "joysticks!";
      for (int i = 0; i < numJoysticks; i++)
      {
         LOG_DEBUG() << "Joystick" << i << JoystickInfoString(i);
      }

      theJoystick = SDL_JoystickOpen(0);
      if (theJoystick != nullptr)
      {
         LOG_DEBUG() << "Opened joystick" << JoystickInfoString(theJoystick);
      }
      else
      {
         LOG_WARNING() << "Error opening joystick" << SDL_JoystickNameForIndex(0) << ":" << SDL_GetError();
      }
   }
}

void Joystick::UpdateJoysticks()
{
   if ( (theJoystick != nullptr) && SDL_JoystickGetAttached(theJoystick))
   {
      // They joystick we are using hasn't disappeared, so do nothing
      LOG_DEBUG() << "UpdateJoysticks, nothing needs updating";
      return;
   }

   if (theJoystick != nullptr)
   {
      // We are no longer attached to the joystick
      LOG_WARNING() << "Joystick detached, attempt to add new one";
      //SDL_JoystickClose(_joystick);
   }

   AddJoystick();

   // Is joystick already open / still attached?
}

void Joystick::RegisterCommand(JoystickRegistrationCallbacks* subscriber,
                               bool clearExisting)
{
   if (subscriber == nullptr)
   {
      LOG_FATAL() << "Registering joystick commands with invalid callback";
      return;
   }

   if (clearExisting)
   {
      ClearRegisteredCommands();
   }

   std::map<int, Command*> cmdHandlers;

   cmdHandlers = subscriber->GetButtonUpHandlers();
   theButtonUpHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());

   cmdHandlers = subscriber->GetButtonDownHandlers();
   theButtonDownHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());

   cmdHandlers = subscriber->GetAxesHandlers();
   theMovementHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());
}

void Joystick::ClearRegisteredCommands()
{
   std::map<int, Command*>::iterator it;
   for(it = theButtonDownHandlers.begin(); it != theButtonDownHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick down handler";
      delete (*it).second;
   }

   for(it = theButtonUpHandlers.begin(); it != theButtonUpHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick up handler";
      delete (*it).second;
   }

   for(it = theMovementHandlers.begin(); it != theMovementHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick movement handler";
      delete (*it).second;
   }

   theButtonDownHandlers.clear();
   theButtonUpHandlers.clear();
   theMovementHandlers.clear();
}

bool Joystick::ProcessEvent(SDL_Event const & ev)
{
   switch (ev.type)
   {
      case SDL_JOYBUTTONDOWN:
      {
         std::map<int, Command*>::iterator it = theButtonDownHandlers.find(ev.jbutton.button);
         if (it != theButtonDownHandlers.end())
         {
            // Found a handler for this button, execute command
            return it->second->Execute();
            break;
         }
         break;
      }

      case SDL_JOYBUTTONUP:
      {
         std::map<int, Command*>::iterator it = theButtonUpHandlers.find(ev.jbutton.button);
         if (it != theButtonUpHandlers.end())
         {
            // Found a handler for this button, execute command
            return it->second->Execute();
            break;
         }
         break;
      }

      case SDL_JOYAXISMOTION:
      {
         std::map<int, Command*>::iterator it = theMovementHandlers.find(ev.jaxis.axis);
         if (it != theMovementHandlers.end())
         {
            // Found a handler for this button, execute command

            // Scale the input down to -100 - 100.  We will then add 100 to put in the bundle
            // parameter (which is unsigned)
            int scaledValue = ev.jaxis.value / 327;

            if ( (scaledValue < -1.0 * theDeadzone) || (scaledValue > theDeadzone) )
            {
               LOG_DEBUG() << "Axis NOT in the deadzone" << scaledValue;
               it->second->PushInBundle( scaledValue + 100 );
            }
            else
            {
               LOG_DEBUG() << "Axis in the deadzone" << scaledValue;
               it->second->PushInBundle(100);
            }
            return it->second->Execute();
            break;
         }
         break;
      }
   }

   return false;
}

void Joystick::AddButtonDownHandler(int button, Command* cmd)
{

   std::map<int, Command*>::iterator it = theButtonDownHandlers.find(button);
   if (it != theButtonDownHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for (down) button" << button << "and one already existed";
      delete it->second;
   }

   theButtonDownHandlers[button] = cmd;
}

void Joystick::AddButtonUpHandler(int button, Command* cmd)
{
   std::map<int, Command*>::iterator it = theButtonUpHandlers.find(button);
   if (it != theButtonUpHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for (up) button" << button << "and one already existed";
      delete it->second;
   }

   theButtonUpHandlers[button] = cmd;
}

void Joystick::AddAxesHandler(int axis, Command* cmd)
{
   std::map<int, Command*>::iterator it = theMovementHandlers.find(axis);
   if (it != theMovementHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for axis" << axis << "and one already existed";
      delete it->second;
   }

   theMovementHandlers[axis] = cmd;
}
