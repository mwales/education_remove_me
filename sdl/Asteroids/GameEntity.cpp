#include "GameEntity.h"

GameEntity::GameEntity():
   _updateRate { 0 }
{
}

void GameEntity::SetUpdateRate(int updateHz)
{
   _updateRate = updateHz;
}

void GameEntity::Update()
{

}
