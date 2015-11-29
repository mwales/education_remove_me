#pragma once

#include "Command.h"
#include "Spaceship.h"

//*****************************************************************************
// FireCommand
//*****************************************************************************

class FireCommand: public Command
{
public:

   FireCommand(Spaceship* ship);

   bool Execute();

protected:

   Spaceship* _ship;

};

//*****************************************************************************
// ThrustCommand
//*****************************************************************************

class ThrustCommand: public Command
{
public:

   ThrustCommand(Spaceship* s, bool thrustState);
   bool Execute();

protected:

   Spaceship* _ship;
   bool _thrustState;

};

//*****************************************************************************
// TurnCommand
//*****************************************************************************

class TurnCommand: public Command
{
public:

   TurnCommand(Spaceship* s, int rotationDir);
   bool Execute();

protected:

   Spaceship* _ship;

   // Like polar coordinates
   // Positive = CCW
   // 0 = not rotation
   // 1 = rotation CW
   int _rotateDirection;

};

//*****************************************************************************
// TurnJSCommand
//*****************************************************************************

class TurnJSCommand: public Command
{
public:

   TurnJSCommand(Spaceship* s);
   bool Execute();

protected:

   Spaceship* _ship;

};

