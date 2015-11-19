#pragma once

#include "GameEntity.h"

#include <SDL2/SDL.h>
#include "GameMath.h"

class ImageInfo;

class GraphicEntity: public GameEntity
{
public:
   GraphicEntity();

   ~GraphicEntity();

   virtual void SetImageInfo(char const * filename, SDL_Renderer* r);

   virtual void Update();

   virtual void Draw();

   void SetAngle(float degrees);

   void AddAngle(float addDegrees);

   void SetPosition(XYPair pos);

   void SetFullscreen(bool fullscreenEnable);

   inline ImageInfo* GetImageInfo() { return _image; }

protected:

   XYPair _position;

   ImageInfo* _image;

   bool _fullscreen;

   float _angle;
};
