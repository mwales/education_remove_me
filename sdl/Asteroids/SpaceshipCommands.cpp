#include "SpaceshipCommands.h"

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

}

void TurnCommand::Execute()
{
   _ship->SetTurningDirection(_rotateDirection);
}
