#pragma once

#include "GameEntity.h"

#include <SDL2/SDL.h>
#include "GameMath.h"
#include "TiledImage.h"

class ImageInfo;

class GraphicEntity: public GameEntity
{
public:
   GraphicEntity();

   ~GraphicEntity();

   virtual void SetImageInfo(char const * filename, SDL_Renderer* r);

   virtual void SetTextImageInfo(std::string text, SDL_Color color, SDL_Renderer* r);

   virtual void SetImageInfo(ImageInfo* ii);

   virtual void SetTiledImageInfo(char const * filename,
                                  SDL_Renderer* renderer,
                                  int width,
                                  int height,
                                  int spacing,
                                  TiledImage::TilingMode mode);

   virtual void Update();

   virtual void Draw();

   void SetAngle(float degrees);

   void AddAngle(float addDegrees);

   void SetPosition(XYPair pos);

   void SetFullscreen(bool fullscreenEnable);

   void SetFrameNumber(int fn);

   inline ImageInfo* GetImageInfo() { return _image; }

protected:

   XYPair _position;

   ImageInfo* _image;

   bool _fullscreen;

   float _angle;
};
