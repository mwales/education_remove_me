#include "MovingEntity.h"
#include "ImageInfo.h"

const float FRICTION_SCALAR = 0.9;

MovingEntity::MovingEntity(XYPair mapBounds):
   _mapBounds(mapBounds)
{

}

void MovingEntity::Update()
{
   _velocity += _acceleration * _updateRateScalar;
   _velocity *= (FRICTION_SCALAR * _updateRateScalar);
   _position += _velocity;
}

void MovingEntity::SetUpdateRate(int updateHz)
{
   _updateRate = updateHz;
   _updateRateScalar = 1.0 / (float) updateHz;
}

void MovingEntity::SetVelocity(XYPair vel)
{
   _velocity = vel;
}

void MovingEntity::SetAcceleration(XYPair acc)
{
   _acceleration = acc;
}

void MovingEntity::SetAngularVelocity(float degPerSec)
{
   _rotVelocity = degPerSec;
}

void MovingEntity::SetAngularAcceleration(float degPerSecSquared)
{
   _rotAcceleration = degPerSecSquared;
}
