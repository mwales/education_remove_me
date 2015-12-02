#include <math.h>

#include "MovingEntity.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "GameMath.h"

const float FRICTION_SCALAR = 0.1;
const float ROT_FRICTION_SCALAR = 0.3;

MovingEntity::MovingEntity(XYPair mapBounds):
   _mapBounds(mapBounds),
   _translationalFrictionScalar(FRICTION_SCALAR),
   _rotationalFrictionScalar(ROT_FRICTION_SCALAR)
{
   //_graphicBounds = XYPair(0.0, 0.0);;

   _updateRateScalar = 0.0;

   _velocity = XYPair(0.0, 0.0);

   _acceleration = XYPair(0.0, 0.0);

   _rotVelocity = 0.0;

   _rotAcceleration = 0.0;
}

void MovingEntity::Update()
{
//   LOG_DEBUG() << "Before:    "
//               << "\tPosition=" << _position
//               << "\tVelocity=" << _velocity
//               << "\tAcceleration" << _acceleration;

//   LOG_DEBUG() << "Before:    "
//               << "\tAngle=" << _angle
//               << "\tRot Velocity=" << _rotVelocity
//               << "\tAcceleration" << _rotAcceleration;

   _velocity += _acceleration * _updateRateScalar;

   _velocity *= (1.0 - _translationalFrictionScalar * _updateRateScalar);

   _position += _velocity * _updateRateScalar;

   _rotVelocity += _rotAcceleration * _updateRateScalar;

   _rotVelocity *= (1.0 - _rotationalFrictionScalar * _updateRateScalar);

   AddAngle(_rotVelocity * _updateRateScalar);

//   LOG_DEBUG() << "After:    "
//               << "\tPosition=" << _position
//               << "\tVelocity=" << _velocity
//               << "\tAcceleration" << _acceleration;

//   LOG_DEBUG() << "After:    "
//               << "\tAngle=" << _angle
//               << "\tRot Velocity=" << _rotVelocity
//               << "\tAcceleration" << _rotAcceleration;

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

void MovingEntity::SetTranslationalFriction(float scalar)
{
   _translationalFrictionScalar = scalar;
}

void MovingEntity::SetRotationalFriction(float scalar)
{
   _rotationalFrictionScalar = scalar;
}
