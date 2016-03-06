#pragma once

#include "Command.h"
#include "Spaceship.h"

//*****************************************************************************
// FireCommand
//*****************************************************************************

class FireCommand: public Command
{
public:

   FireCommand(Spaceship* ship, bool fireState);

   bool Execute();

protected:

   Spaceship* theShip;
   bool theFireState;

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

   Spaceship* theShip;
   bool theThrustState;

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

   Spaceship* theShip;

   // Like polar coordinates
   // Positive = CCW
   // 0 = not rotation
   // 1 = rotation CW
   int theRotateDirection;

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

   Spaceship* theShip;

};

