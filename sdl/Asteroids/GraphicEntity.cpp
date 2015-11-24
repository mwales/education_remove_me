#include "GraphicEntity.h"
#include "ImageInfo.h"
#include "Logger.h"

GraphicEntity::GraphicEntity():
   _image(NULL),
   _fullscreen(false)
{
   _position = XYPair(0,0);
   _angle = 0.0;
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
   // Do stuff

   GameEntity::Update();
}

void GraphicEntity::Draw()
{
   if (_image == NULL)
   {
      LOG_WARNING() << "Drawing graphical entity with no image information";
      return;
   }

   LOG_DEBUG() << "Drawing image at" << _position;
   _image->Draw(_position);
}

void GraphicEntity::SetImageInfo(char const * filename, SDL_Renderer* r)
{
   _image = new ImageInfo(filename, r);
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

void GraphicEntity::SetFullscreen(bool fullscreenEnable)
{
   _fullscreen = fullscreenEnable;
   if (_fullscreen)
   {
      _position[0] = 0;
      _position[1] = 0;
   }
}
