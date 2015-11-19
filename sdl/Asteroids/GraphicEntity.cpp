#include "GraphicEntity.h"
#include "ImageInfo.h"

GraphicEntity::GraphicEntity():
   _image(NULL),
   _fullscreen(false)
{
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
   _image->Draw(_position);
}

void GraphicEntity::SetImageInfo(char const * filename, SDL_Renderer* r)
{
   _image = new ImageInfo(filename, r);
}


void GraphicEntity::SetAngle(float degrees)
{
   _angle = degrees;
}

void GraphicEntity::AddAngle(float addDegrees)
{
   _angle += addDegrees;
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
