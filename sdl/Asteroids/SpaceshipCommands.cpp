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

bool FireCommand::Execute()
{
   _ship->Fire();
   return false;
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

bool ThrustCommand::Execute()
{
   _ship->SetThrustState(_thrustState);
   return false;
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

bool TurnCommand::Execute()
{
   _ship->SetTurningDirection(_rotateDirection);
   return false;
}

//*****************************************************************************
// TurnJSCommand
//*****************************************************************************


TurnJSCommand::TurnJSCommand(Spaceship* ship):
   _ship(ship)
{
   // Empty
}

bool TurnJSCommand::Execute()
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

   return false;
}

