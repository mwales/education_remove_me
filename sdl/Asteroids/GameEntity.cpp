#include "GameEntity.h"

GameEntity::GameEntity():
   theUpdateRateHz { 0 }
{
}

void GameEntity::SetUpdateRate(int updateHz)
{
   theUpdateRateHz = updateHz;
}

void GameEntity::Update()
{

}
