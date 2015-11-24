#include <math.h>

#include "MovingEntity.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "GameMath.h"

const float FRICTION_SCALAR = 0.3;
const float ROT_FRICTION_SCALAR = 0.75;

MovingEntity::MovingEntity(XYPair mapBounds):
   _mapBounds(mapBounds)
{
   //_mapBounds = XYPair(0.0, 0.0);;

   //_graphicBounds = XYPair(0.0, 0.0);;

   _updateRateScalar = 0.0;

   _velocity = XYPair(0.0, 0.0);

   _acceleration = XYPair(0.0, 0.0);

   _rotVelocity = 0.0;

   _rotAcceleration = 0.0;
}

void MovingEntity::Update()
{
   _velocity += _acceleration * _updateRateScalar;
   //LOG_DEBUG() << "1) _v=" << _velocity << ", _acc=" << _acceleration << ", _updateRate=" << _updateRateScalar;

   _velocity *= (1.0 - FRICTION_SCALAR * _updateRateScalar);
   //LOG_DEBUG() << "2) _v=" << _velocity << ", _acc=" << _acceleration << ", _updateRate=" << _updateRateScalar;

   _position += _velocity;

   LOG_DEBUG() << "3) Acceleration=" << _acceleration << ", velocity=" << _velocity << ", New position=" << _position;

   _rotVelocity += _rotAcceleration * _updateRateScalar;

   _rotVelocity *= (1.0 - ROT_FRICTION_SCALAR * _updateRateScalar);

   AddAngle(_rotVelocity);

   //_angle = fmod(_angle + 360.0, 360.0);

   LOG_DEBUG() << "4) RotAcceleration=" << _rotAcceleration << ", Rotvelocity=" << _rotVelocity
               << ", New angle=" << _angle;


   // Wrap position around
   _position = GameMath::PositionModulus(_position, _mapBounds);
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
