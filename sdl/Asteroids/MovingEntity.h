#pragma once

#include "GraphicEntity.h"
#include "GameMath.h"

class MovingEntity : public GraphicEntity
{
public:
   MovingEntity(XYPair mapBounds);

   virtual void SetUpdateRate(int updateHz);

   virtual void Update();

   void SetVelocity(XYPair vel);

   void SetAcceleration(XYPair acc);

   void SetAngularVelocity(float degPerSec);

   void SetAngularAcceleration(float degPerSecSquared);

   void SetTranslationalFriction(float scalar);

   void SetRotationalFriction(float scalar);

protected:

   XYPair _mapBounds;

   XYPair _graphicBounds;

   float _updateRateScalar;

   XYPair _velocity;

   XYPair _acceleration;

   float _rotVelocity;

   float _rotAcceleration;

   float _translationalFrictionScalar;

   float _rotationalFrictionScalar;
};


