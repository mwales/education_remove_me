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

   void ClearRegisteredCommand();

   void ProcessEvent(SDL_Event const & ev);

protected:

   SDL_Joystick* _joystick;

   std::map<int, Command*> _buttonDownHandlers;

   std::map<int, Command*> _buttonUpHandlers;

   std::map<int, Command*> _movementHandlers;

   int _deadzone;
};


