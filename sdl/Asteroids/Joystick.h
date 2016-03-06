#pragma once

#include <string>
#include <map>

#include <SDL2/SDL.h>

class Command;

class JoystickRegistrationCallbacks
{
public:
   virtual std::map<int, Command*> GetButtonDownHandlers() = 0;

   virtual std::map<int, Command*> GetButtonUpHandlers() = 0;

   virtual std::map<int, Command*> GetAxesHandlers() = 0;
};


class Joystick
{
public:
   Joystick();

   ~Joystick();

   void UpdateJoysticks();

   std::string JoystickInfoString(int index);

   std::string JoystickInfoString(SDL_Joystick* js);

   void AddJoystick();

   void CloseAllJoysticks();

   void RegisterCommand(JoystickRegistrationCallbacks* subscriber, bool clearExisting = true);

   void ClearRegisteredCommands();

   bool ProcessEvent(SDL_Event const & ev);

   void AddButtonDownHandler(int button, Command* cmd);

   void AddButtonUpHandler(int button, Command* cmd);

   void AddAxesHandler(int axis, Command* cmd);

protected:

   SDL_Joystick* theJoystick;

   std::map<int, Command*> theButtonDownHandlers;

   std::map<int, Command*> theButtonUpHandlers;

   std::map<int, Command*> theMovementHandlers;

   int theDeadzone;
};


