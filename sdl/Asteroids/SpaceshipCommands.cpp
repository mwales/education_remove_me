#include "SpaceshipCommands.h"
#include "Logger.h"

//*****************************************************************************
// FireCommand
//*****************************************************************************

FireCommand::FireCommand(Spaceship* ship, bool fireState):
   theShip(ship),
   theFireState(fireState)
{
   // Empty
}

bool FireCommand::Execute()
{
   theShip->Fire(theFireState);
   return false;
}


//*****************************************************************************
// ThrustCommand
//*****************************************************************************


ThrustCommand::ThrustCommand(Spaceship* ship, bool thrustState):
   theShip(ship),
   theThrustState(thrustState)
{
   // Empty
}

bool ThrustCommand::Execute()
{
   theShip->SetThrustState(theThrustState);
   return false;
}


//*****************************************************************************
// TurnCommand
//*****************************************************************************


TurnCommand::TurnCommand(Spaceship* ship, int rotationDir):
   theShip(ship),
   theRotateDirection(rotationDir)
{
   // Empty
}

bool TurnCommand::Execute()
{
   theShip->SetTurningDirection(theRotateDirection);
   return false;
}

//*****************************************************************************
// TurnJSCommand
//*****************************************************************************


TurnJSCommand::TurnJSCommand(Spaceship* ship):
   theShip(ship)
{
   // Empty
}

bool TurnJSCommand::Execute()
{
   if (Command::GetBundleSize() > 0)
   {
      // Bundle parameter was a value of -100 to 100 that was converted to 0-200
      theShip->SetRotationalAcceleration(GetBundleParameter(0)-100);
      ClearBundle();
   }
   else
   {
      LOG_FATAL() << "Bundle empty for turn js command";
   }

   return false;
}

