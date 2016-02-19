#pragma once

#include "GameEntity.h"
#include "ICollidable.h"

#include <SDL2/SDL.h>
#include "GameMath.h"
#include "TiledImage.h"

class ImageInfo;

class GraphicEntity: public GameEntity, public ICollidable
{
public:
   GraphicEntity(XYPair mapBounds);

   virtual ~GraphicEntity();

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

   virtual void SetUpdateRate(int updateHz);

   void SetVelocity(XYPair vel);

   void SetAcceleration(XYPair acc);

   void SetAngularVelocity(float degPerSec);

   void SetAngularAcceleration(float degPerSecSquared);

   void SetTranslationalFriction(float scalar);

   void SetRotationalFriction(float scalar);

   // ICollidable interface

   virtual XYPair GetPosition() const;

   virtual std::vector<SDL_Rect> GetCollisionBoxes() const;

   //virtual bool CollisionDetected(std::string collidingObject) const;

   //virtual std::string GetName() const;


protected:

   XYPair _position;

   ImageInfo* _image;

   bool _fullscreen;

   float _angle;

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
