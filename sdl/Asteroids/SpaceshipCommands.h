#pragma once

#include "Command.h"
#include "Spaceship.h"

class FireCommand: public Command
{
public:

   FireCommand(Spaceship* ship);

   void Execute();

protected:

   Spaceship* _ship;

};

class ThrustCommand: public Command
{
public:

   ThrustCommand(Spaceship* s, bool thrustState);
   void Execute();

protected:

   Spaceship* _ship;
   bool _thrustState;

};

class TurnCommand: public Command
{
public:

   TurnCommand(Spaceship* s, int rotationDir);
   void Execute();

protected:

   Spaceship* _ship;

   // Like polar coordinates
   // Positive = CCW
   // 0 = not rotation
   // 1 = rotation CW
   int _rotateDirection;

};


