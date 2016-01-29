#include <math.h>

#include "GraphicEntity.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "TiledImage.h"
#include "TextImage.h"
#include "GameMath.h"

const float FRICTION_SCALAR = 0.1;
const float ROT_FRICTION_SCALAR = 0.3;

GraphicEntity::GraphicEntity(XYPair mapBounds):
   _image(NULL),
   _fullscreen(false),
   _mapBounds(mapBounds),
   _translationalFrictionScalar(FRICTION_SCALAR),
   _rotationalFrictionScalar(ROT_FRICTION_SCALAR)
{
   _position = XYPair(0,0);
   _angle = 0.0;

   _updateRateScalar = 0.0;

   _velocity = XYPair(0.0, 0.0);

   _acceleration = XYPair(0.0, 0.0);

   _rotVelocity = 0.0;

   _rotAcceleration = 0.0;
}

GraphicEntity::~GraphicEntity()
{
   if (_image)
   {
      delete _image;
      _image = NULL;
   }
}

void GraphicEntity::Update()
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

void GraphicEntity::SetUpdateRate(int updateHz)
{
   _updateRate = updateHz;
   _updateRateScalar = 1.0 / (float) updateHz;
}

void GraphicEntity::SetVelocity(XYPair vel)
{
   _velocity = vel;
}

void GraphicEntity::SetAcceleration(XYPair acc)
{
   _acceleration = acc;
}

void GraphicEntity::SetAngularVelocity(float degPerSec)
{
   _rotVelocity = degPerSec;
}

void GraphicEntity::SetAngularAcceleration(float degPerSecSquared)
{
   _rotAcceleration = degPerSecSquared;
}

void GraphicEntity::SetTranslationalFriction(float scalar)
{
   _translationalFrictionScalar = scalar;
}

void GraphicEntity::SetRotationalFriction(float scalar)
{
   _rotationalFrictionScalar = scalar;
}

void GraphicEntity::Draw()
{
   if (_image == NULL)
   {
      LOG_WARNING() << "Drawing graphical entity with no image information";
      return;
   }

   if (_fullscreen)
   {
      LOG_DEBUG() << "Drawing image fullscreen";
      _image->DrawFullScreen();
   }
   else
   {
      LOG_DEBUG() << "Drawing image at" << _position;
      _image->Draw(_position);
   }
}

void GraphicEntity::SetImageInfo(char const * filename, SDL_Renderer* r)
{
   _image = new ImageInfo(filename, r);
}

void GraphicEntity::SetTextImageInfo(std::string text, SDL_Color color, SDL_Renderer* r)
{
   _image = new TextImage(text, color, r);
}

void GraphicEntity::SetImageInfo(ImageInfo* ii)
{
   _image = ii;
}

void GraphicEntity::SetTiledImageInfo(char const * filename,
                                      SDL_Renderer* renderer,
                                      int width,
                                      int height,
                                      int spacing,
                                      TiledImage::TilingMode mode)
{
   _image = new TiledImage(filename, renderer, width, height, spacing, mode);
}


void GraphicEntity::SetAngle(float degrees)
{
   _angle = degrees;

   // Force the angle to be between 0 and 360
   if (_angle < 0.0)
   {
      // Negative angle
      LOG_DEBUG() << "Wrapping negative angle" << _angle << " fmod=" << fmod(_angle, 360.0);

      _angle *= -1;
      _angle = fmod(_angle, 360.0);
      _angle = 360.0 - _angle;

      LOG_DEBUG() << "The long way=" << _angle ;

   }
   else if (_angle > 360.0)
   {
      LOG_DEBUG() << "Wrapping angle around from" << _angle << "to" << fmod(_angle, 360.0);
      _angle = fmod(_angle, 360.0);
   }

   if (_image != NULL)
   {
      _image->SetAngle(_angle);
   }
}

void GraphicEntity::AddAngle(float addDegrees)
{
   SetAngle(_angle + addDegrees);
}

void GraphicEntity::SetPosition(XYPair pos)
{
   _position = pos;
   _fullscreen = false;
}

XYPair GraphicEntity::GetPosition() const
{
   return _position;
}

std::vector<CollisionRect> GraphicEntity::GetCollisionBoxes() const
{
   // Basic collision box is the size of the image
   CollisionRect retVal;
   retVal._size = _image->GetSize();


   retVal._topLeft = _position - retVal._size * 0.5; //._x = _position._x - retVal._size._x / 2;
   //retVal._topLeft._y = _position._y - retVal._size._y / 2;

   std::vector<CollisionRect> retVec;
   retVec.push_back(retVal);
}

void GraphicEntity::SetFullscreen(bool fullscreenEnable)
{
   _fullscreen = fullscreenEnable;
   if (_fullscreen)
   {
      _position[0] = 0;
      _position[1] = 0;
   }
}

void GraphicEntity::SetFrameNumber(int fn)
{
   _image->SetFrameNumber(fn);
}
