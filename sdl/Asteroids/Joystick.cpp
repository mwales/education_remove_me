#include "Joystick.h"

#include <ostream>

#include "Command.h"
#include "Logger.h"

Joystick::Joystick():
   _joystick(nullptr),
   _deadzone(25)
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
   if (_joystick)
   {
      LOG_DEBUG() << "Closing joystick";
      SDL_JoystickClose(_joystick);
      _joystick = nullptr;
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

      _joystick = SDL_JoystickOpen(0);
      if (_joystick != nullptr)
      {
         LOG_DEBUG() << "Opened joystick" << JoystickInfoString(_joystick);
      }
      else
      {
         LOG_WARNING() << "Error opening joystick" << SDL_JoystickNameForIndex(0) << ":" << SDL_GetError();
      }
   }
}

void Joystick::UpdateJoysticks()
{
   if ( (_joystick != nullptr) && SDL_JoystickGetAttached(_joystick))
   {
      // They joystick we are using hasn't disappeared, so do nothing
      LOG_DEBUG() << "UpdateJoysticks, nothing needs updating";
      return;
   }

   if (_joystick != nullptr)
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
   _buttonUpHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());

   cmdHandlers = subscriber->GetButtonDownHandlers();
   _buttonDownHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());

   cmdHandlers = subscriber->GetAxesHandlers();
   _movementHandlers.insert(cmdHandlers.begin(), cmdHandlers.end());
}

void Joystick::ClearRegisteredCommands()
{
   std::map<int, Command*>::iterator it;
   for(it = _buttonDownHandlers.begin(); it != _buttonDownHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick down handler";
      delete (*it).second;
   }

   for(it = _buttonUpHandlers.begin(); it != _buttonUpHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick up handler";
      delete (*it).second;
   }

   for(it = _movementHandlers.begin(); it != _movementHandlers.end(); it++)
   {
      LOG_DEBUG() << "Deleting a Joystick movement handler";
      delete (*it).second;
   }

   _buttonDownHandlers.clear();
   _buttonUpHandlers.clear();
   _movementHandlers.clear();
}

bool Joystick::ProcessEvent(SDL_Event const & ev)
{
   switch (ev.type)
   {
      case SDL_JOYBUTTONDOWN:
      {
         std::map<int, Command*>::iterator it = _buttonDownHandlers.find(ev.jbutton.button);
         if (it != _buttonDownHandlers.end())
         {
            // Found a handler for this button, execute command
            return it->second->Execute();
            break;
         }
         break;
      }

      case SDL_JOYBUTTONUP:
      {
         std::map<int, Command*>::iterator it = _buttonUpHandlers.find(ev.jbutton.button);
         if (it != _buttonUpHandlers.end())
         {
            // Found a handler for this button, execute command
            return it->second->Execute();
            break;
         }
         break;
      }

      case SDL_JOYAXISMOTION:
      {
         std::map<int, Command*>::iterator it = _movementHandlers.find(ev.jaxis.axis);
         if (it != _movementHandlers.end())
         {
            // Found a handler for this button, execute command

            // Scale the input down to -100 - 100.  We will then add 100 to put in the bundle
            // parameter (which is unsigned)
            int scaledValue = ev.jaxis.value / 327;

            if ( (scaledValue < -1.0 * _deadzone) || (scaledValue > _deadzone) )
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

   std::map<int, Command*>::iterator it = _buttonDownHandlers.find(button);
   if (it != _buttonDownHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for (down) button" << button << "and one already existed";
      delete it->second;
   }

   _buttonDownHandlers[button] = cmd;
}

void Joystick::AddButtonUpHandler(int button, Command* cmd)
{
   std::map<int, Command*>::iterator it = _buttonUpHandlers.find(button);
   if (it != _buttonUpHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for (up) button" << button << "and one already existed";
      delete it->second;
   }

   _buttonUpHandlers[button] = cmd;
}

void Joystick::AddAxesHandler(int axis, Command* cmd)
{
   std::map<int, Command*>::iterator it = _movementHandlers.find(axis);
   if (it != _movementHandlers.end())
   {
      LOG_WARNING() << "Adding a joystick handler for axis" << axis << "and one already existed";
      delete it->second;
   }

   _movementHandlers[axis] = cmd;
}
