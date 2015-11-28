#include "SpaceshipCommands.h"
#include "Logger.h"

//*****************************************************************************
// FireCommand
//*****************************************************************************

FireCommand::FireCommand(Spaceship* ship):
   _ship(ship)
{
   // Empty
}

void FireCommand::Execute()
{
   _ship->Fire();
}


//*****************************************************************************
// ThrustCommand
//*****************************************************************************


ThrustCommand::ThrustCommand(Spaceship* ship, bool thrustState):
   _ship(ship),
   _thrustState(thrustState)
{
   // Empty
}

void ThrustCommand::Execute()
{
   _ship->SetThrustState(_thrustState);
}


//*****************************************************************************
// TurnCommand
//*****************************************************************************


TurnCommand::TurnCommand(Spaceship* ship, int rotationDir):
   _ship(ship),
   _rotateDirection(rotationDir)
{
   // Empty
}

void TurnCommand::Execute()
{
   _ship->SetTurningDirection(_rotateDirection);
}

//*****************************************************************************
// TurnJSCommand
//*****************************************************************************


TurnJSCommand::TurnJSCommand(Spaceship* ship):
   _ship(ship)
{
   // Empty
}

void TurnJSCommand::Execute()
{
   if (Command::GetBundleSize() > 0)
   {
      // Bundle parameter was a value of -100 to 100 that was converted to 0-200
      _ship->SetRotationalAcceleration(GetBundleParameter(0)-100);
      ClearBundle();
   }
   else
   {
      LOG_FATAL() << "Bundle empty for turn js command";
   }
}

